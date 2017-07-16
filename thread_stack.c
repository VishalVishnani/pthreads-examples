/*Includes*/
#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>

#define N 1000
#define MEGSIZE 1000000

/*Variable to define attribute*/
pthread_attr_t attr;

/*Function executed by threads*/
void* dowork(void* threadid){
  double A[1000][1000];
  int i,j;
  int tid;
  size_t mystacksize;
  tid=(int)threadid;

  /*Get stacksize*/
  pthread_attr_getstacksize(&attr,&mystacksize);
  printf("Thread %d   Stacksize %li \n",tid,mystacksize);
  for(i=0;i<1000;i++)
    for(j=0;j<1000;j++)
      A[i][j]=((i*j)/3.452)+(1000-i);

  pthread_exit(NULL);
}


int main()
{
  pthread_t threads[4];
  size_t stacksize;
  int rc;
  int t;

  /*Initialize attribute*/
  pthread_attr_init(&attr);

  /*Get stacksize*/
  pthread_attr_getstacksize(&attr,&stacksize);
  printf("Default stack size is %li\n",stacksize);
  stacksize=sizeof(double)*N*N+MEGSIZE;
  printf("Amount of stack needed per thread is %li\n",stacksize);

  /*Set stacksize*/
  pthread_attr_setstacksize(&attr,stacksize);
  printf("Creating threads with stacksize = %li\n",stacksize);
  for(t=0;t<4;t++){

    /*Create threads of the required stacksize*/
    rc=pthread_create(&threads[t],&attr,dowork,(void*)t);
    if(rc){
      printf("error in creating thread %d\n",t);
    }
  }
  printf("created %d threads\n",t);
  pthread_exit(NULL);
}
