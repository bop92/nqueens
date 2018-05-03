#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "uici.h"
#include "restart.h"

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

/*these structs will be used to pass arguments to the threads*/

typedef struct ti{
   int n;
   int first;
   int out_flag;
   int result;
}thread_info_t;

typedef struct si{
   int * comfd;
}server_info_t;

typedef struct ci{
   int n;
   int first;
   char host[20];
   u_port_t port;
   int result;
}client_info_t;

void get_array_string(int *list, int n, char *s);

int next_permutation(int *list, int n);

int check_diagonal(int *list, int n);

int generate_n_queens_serial_one(int n, int out_flag, int first);

int generate_n_queens_server_one(int n, int comfd, int first);

void * queens_thread(void *infopointer);

void * server_thread(void *point);

void * client_thread(void *point);
