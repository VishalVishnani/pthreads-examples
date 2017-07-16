/*Includes*/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*Function executed by threads*/
void *BusyWork(void *t){
  long int i;
  int tid;
  double result=0.0;
  tid=(int)t;
  printf("Thread %d starting\n",tid);
  for(i=0;i<1000000;i++){
    result=result+i;
  }
  printf("Thread %d done. Result=%e\n",tid,result);
  pthread_exit(NULL);

}


int main()
{

  pthread_t threads[5];

  /*Variable to define attribute joinable*/
  pthread_attr_t attr;
  int rc;
  int t;
  void *status;
  
  /*Initialize attribute*/
  pthread_attr_init(&attr);

  /*Set attribute to be used during thread creation*/
  pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);
  
  for(t=0;t<5;t++){
    printf("Mian creating thread %d\n",t);
    
    /*Thread creation*/
    rc=pthread_create(&threads[t],&attr,BusyWork,(void*)t);
    if(rc){
      printf("Error in creating thread %d\n",t);
    }
  }

  pthread_attr_destroy(&attr);
  for(t=0;t<5;t++){

    /*Thread join waits for a thread to arrive*/
    rc=pthread_join(threads[t],&status);
    if(rc){
      printf("Error in joining thread %d\n",t);
    }
    printf("Main completed join with thread %d having status %ld\n",t,(long)status);
  }
  printf("Main completed\n");
  pthread_exit(NULL);
}




