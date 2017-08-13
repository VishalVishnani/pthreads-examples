#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define BUF_SIZE 3

int buffer[BUF_SIZE];
int add=0;
int rem=0;
int num=0;

pthread_mutex_t m;
pthread_cond_t c_cons;
pthread_cond_t c_prod;


void* producer(void);
void* consumer(void);

int main()
{
  pthread_t tid1,tid2;
  pthread_mutex_init(&m,NULL);
  pthread_cond_init(&c_cons,NULL);
  pthread_cond_init(&c_prod,NULL);
  int i,ret;

  ret=pthread_create(&tid1,NULL,(void*)producer,NULL);
  if(ret!=0){
    fprintf(stderr,"Unable to create producer thread\n");
    exit(1);
  }

  ret=pthread_create(&tid2,NULL,(void*)consumer,NULL);
  if(ret!=0){
    fprintf(stderr,"Unable to create consumer thread\n");
    exit(1);
  }
  pthread_join(tid1,NULL);
  pthread_join(tid2,NULL);

  return 0;
}

void *producer(void){
  int i;
  for(i=1;i<=20;i++){
    pthread_mutex_lock(&m);
    if(num>BUF_SIZE)
      exit(1);
    while(num==BUF_SIZE){
    //  pthread_cond_signal(&c_cons);
      pthread_cond_wait(&c_prod,&m);

    }
    buffer[add]=i;
    add=(add+1)%BUF_SIZE;
    num++;
    pthread_mutex_unlock(&m);
    pthread_cond_signal(&c_cons);
    printf("Producer Inserted:  %d\n",i);
  }
  printf("Producer Quitting\n");
}

void* consumer(void){
  int i;
  
  while(1){
    pthread_mutex_lock(&m);
    if(num<0)
      exit(1);
    while(num==0){
  //    pthread_cond_signal(&c_prod);
      pthread_cond_wait(&c_cons,&m);

    }
    i=buffer[rem];
    rem=(rem+1)%BUF_SIZE;
    num--;
    pthread_mutex_unlock(&m);
    pthread_cond_signal(&c_prod);
    printf("Consume Value: %d\n",i);
  }
}
    
    


