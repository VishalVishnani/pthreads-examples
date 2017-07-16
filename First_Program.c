/*Includes*/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define NUM_THREADS 5

void *PrintHello(void *tid){
  int threadid;
  threadid=(int)tid;
  printf("Hello world its me thread %d\n",threadid);
  pthread_exit(NULL);
}


int main()
{

  pthread_t threads[NUM_THREADS];
  int rc;
  int t;
  for(t=0;t<NUM_THREADS;t++){
    /*Thread Creation*/
    printf("In main: creating thread  #%d\n",t);
    rc=pthread_create(&threads[t],NULL,PrintHello, (void*)t);
    if(rc){
      printf("error in thread create %d\n",t);
    }
  }
  pthread_exit(NULL);
}
