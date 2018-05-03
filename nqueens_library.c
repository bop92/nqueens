//nqueens_library.c

/*This file contains functions which do the actual calculations
  to solve the nqueens problem*/

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <pthread.h>
#include <unistd.h>
#include "queens.h"
#include "uici.h"


/*this function converts an integer list into a string which is returned via s*/

void get_array_string(int *list, int n, char *s){
   int i;   
   memset(s, 0, strlen(s)*sizeof(char));
   for(i = 0; i <= n; i++){
      sprintf(s + strlen(s), "%4d", list[i]);
   }
   sprintf(s + strlen(s), "\n");
}  

/*this function recieves a permutation and finds the next viable permutation
  after it. It is an implementation of a well know algorithm*/

int next_permutation(int *list, int n){
   int big_index = 0;
   int count = 0;
   int foo, i, rev[n];
   int swap_index = -1;
   for(i = 0; i < n; i++){
      if(list[i] < list[i+1]){
         swap_index = i;  
      }
   }
   if(swap_index >= 0){
      for(i = swap_index; i < n; i++){
         if((list[swap_index] < list[i+1])){
               big_index = i+1;  
         }
      }
      foo = list[swap_index];
      list[swap_index] = list[big_index];
      list[big_index] = foo;
      count = 0;

      for(i = swap_index+1; i <= n; i++){
         rev[i] = list[i];
      }
      for(i = swap_index+1; i <= n; i++){
          list[n-count] = rev[i];
          count++;
      }
   }
   else{
      return 1;
   }
   return 0;
}

/*this checks each number in the permutation to make sure that none of them
  have a slope of 1 (otherwise the queens would threaten each other)*/
int check_diagonal(int *list, int n){
   int i, j;
   double x;
   int ret = 1;
   for(i = 0; i < n; i++){
      for(j = i+1; j <= n; j++){
         x = (double)(abs(list[i] - list[j]))/(double)(j-i);
         if(x == 1){
             ret = 0;
         }
      }
   }
   return ret;
}

/*this code is called by a thread so that it can solve a single part of the
  nqueens problem on success it returns the number of valid permutations it
  found*/
int generate_n_queens_server_one(int n, int outfd, int first){
   int i;
   int * list = malloc(sizeof(int)*n);
   char * s = malloc(5*n);
   int shift_flag = 0;
   n = n-1;

   list[0] = first;
   for(i = 1; i <= n; i++){
      if(i-1 == first){
         shift_flag = 1;
      }
      if(shift_flag){
         list[i] = i;
      }
      else{
         list[i] = i-1;
      }
   }
   i = 0;

   /*this is the loop which iterates through the valid permutations and
     converts them to strings*/
   do{
      if(check_diagonal(list, n) == 1){
         get_array_string(list, n, s);
         if(dprintf(outfd, "%s", s) < 0){
	    printf("write error:fd = %d\n", outfd);
            //set i so that a negative number is returned
	    i = -100;
	    break;
         }
         i++;
      }
   }
   while((next_permutation(list, n) == 0) && (list[0] == first));

   free(list);
   free(s);
   return i;
}


