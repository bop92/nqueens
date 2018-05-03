#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "uici.h"
#include "queens.h"

#define READ 0
#define WRITE 1

void *server_thread(void *point){

   struct si * args = point;
   char buf[128];
   int first, my_fd, n, result;
   char *s;
   char *q;
   int * fd = args->comfd;

   /*read the file descriptor from the pipe. Need to lock the mutex while 
     reading, otherwise the thread might read only part of a file descriptor*/
   pthread_mutex_lock(&mutex);
   if(r_read(fd[READ], &my_fd, sizeof(int)) < 0){
      pthread_mutex_unlock(&mutex);
      printf("thread encounterd read error\n");
   }
   else{
      pthread_mutex_unlock(&mutex);
      
      /*readline is used here because the data is being sent across the network
        as strings to avoid problems with endianess. A buffer overflow is 
        possible here if the client is untrustworthy or unreliable. No mutex
        locking is needed here because each thread has it's very own file descriptor*/ 

      readline(my_fd, buf, 10);
      printf("recieved:%s", buf);

      /*need to use a threadsafe strtok to parse the instruction string*/
      s = strtok_r(buf, " ", &q);
      n = atoi(s);
      s = strtok_r(NULL, " ", &q);
      first = atoi(s);

      /*n (the size of the chessboard) and first (the permutation we want the 
        thread to start on) will be passed to the calculation function*/

      if((result = generate_n_queens_server_one(n, my_fd, first)) > 0){
         //send the result back to the client
         dprintf(my_fd, "$first %d result %d\n",first, result);
      }

      //close the connection
      if (r_close(my_fd) == -1)                             
         fprintf(stderr, "[%ld]:failed to close communfd: %s\n",
                          (long)getpid(), strerror(errno));
   }
   printf("Thread done! first = %d\n", first);
   
}

int main(int argc, char *argv[]) {
   pid_t child;
   char client[MAX_CANON];
   int listenfd;
   int communfd;
   int fd[2];
   int i = 0;
   u_port_t portnumber;
   pthread_t t_x;
   struct si args;
   
   //check for input mistakes on initialization
   if (argc != 2) {
      fprintf(stderr, "Usage: %s port\n", argv[0]);
      return 1;   
   }  
   portnumber = (u_port_t) atoi(argv[1]);

   //convert a port number to a usable file descriptor
   if ((listenfd = u_open(portnumber)) == -1) {
      perror("Failed to create listening endpoint");
      return 1;
   }


   //create a pipe to communicate with the threads
   if(pipe(fd) < 0){
      printf("could not make pipe\n");
      return 1;
   }
   //copy the pipe pointer into the args data structure so the threads can use it
   args.comfd = fd;

   //let the user know the port is open 
   fprintf(stderr, "[%ld]: Waiting for connection on port %d\n",
                    (long)getpid(), (int)portnumber);

   //set up an infinite loop to accept connections and create threads
   for ( ; ; ) {
      //u_accept will block until a connection is attempted
      if ((communfd = u_accept(listenfd, client, MAX_CANON)) == -1) {
         perror("Failed to accept connection");
         continue;
      }
      else{
          //create the requested thread
          pthread_create(&t_x, NULL, server_thread, &args);
          /*write the communication file descriptor to the pipe so that a
            thread can use it */
          r_write(fd[WRITE], &communfd, sizeof(int));

          fprintf(stderr, "[%ld]:connected to %s\n", (long)getpid(), client);
      }

   }
}
