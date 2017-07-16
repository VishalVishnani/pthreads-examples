/*Includes*/
#include<pthread.h>
#include<stdlib.h>
#include<stdio.h>

#define VECLEN 100
#define NTHREADS 4

/*Structure used for dot product*/
typedef struct{
  double *a;
  double *b;
  double sum;
  int veclen;
} DOTDATA;

DOTDATA dotstr;

pthread_t callthread[NTHREADS];

/*Mutex*/
pthread_mutex_t mutexsum;

/*Function to be executed by thread*/
void* dotprod(void *arg){
  int i,start,end,len;
  long offset;
  double mysum, *x, *y;
  offset = (long)arg;
  
  len=dotstr.veclen;
  start=offset*len;
  end=start+len;
  x=dotstr.a;
  y=dotstr.b;
  mysum=0;
  
  for(i=start;i<end;i++){
    mysum += (x[i] * y[i]);
  }
  
  /*Only 1 thread modifies dotstr.sum at a time*/
  pthread_mutex_lock(&mutexsum);
  dotstr.sum+=mysum;
  pthread_mutex_unlock(&mutexsum);
  
  pthread_exit(NULL);


}



int main()
{
  long i;
  double *a,*b;
  void *status;
  pthread_attr_t attr;
  
  /*Dynamic memory allocation*/
  a=(double*)malloc(NTHREADS*VECLEN*sizeof(double));
  b=(double*)malloc(NTHREADS*VECLEN*sizeof(double));

  for(i=0;i<VECLEN*NTHREADS;i++){
    a[i]=1.0;
    b[i]=a[i];
  }

  dotstr.veclen=VECLEN;
  dotstr.a=a;
  dotstr.b=b;
  dotstr.sum=0;

  /*Initialize mutex*/
  pthread_mutex_init(&mutexsum,NULL);

  /*Initialize and set attribute to create joinable threads*/
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);

  for(i=0;i<NTHREADS;i++){
    pthread_create(&callthread[i],&attr,dotprod,(void*)i);

  }
  
  /*destroy attribute*/
  pthread_attr_destroy(&attr);
  
  for(i=0;i<NTHREADS;i++){
    pthread_join(callthread[i],&status);
  }
  
  printf("sum=%f\n",dotstr.sum);
  free(a);
  free(b);
  
  /*destroy mutex*/
  pthread_mutex_destroy(&mutexsum);
  pthread_exit(NULL);
}





