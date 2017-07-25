//Peterson's Algorithm

/*Includes*/
#include <stdio.h>
#include <pthread.h>

const int MAX=10000;
int flag[2];
int turn;
int ans=0;

/*Initialize lock by reseting the desire of both threads to acquire 
*lock to 0 and giving turn to 1 of them*/
void lock_init()
{
  flag[0]=flag[1]=0;
  turn=0;
}

/*This function is executed before entering the critical section*/
void lock(int self){

  /*Set value to 1 to acquire lock*/
  flag[self]=1;

  /*But first give chance to the other thread to acquire lock*/
  turn=1-self;
  
  /*wait until other thread has no desire to acquire lock
  *or it is your turn*/
  while(flag[1-self]==1 && turn==1-self);

}

/*This function is executed after the critical section*/
void unlock(int self){

  /*set the desire value to 0 after completing*/
  flag[self]=0;
}

/*function executed by threads*/
void* func(void* s){
  int self=(int*)s;
  printf("Thread Entered : %d\n",self);

  int i=0;
  
  lock(self);
  for(i=0;i<MAX;i++)
    ans++;

  unlock(self);

}  

int main()
{

  pthread_t p1,p2;
  
  lock_init();

  pthread_create(&p1,NULL,func,(void*)0);
  pthread_create(&p2,NULL,func,(void*)1);


  pthread_join(p1,NULL);
  pthread_join(p2,NULL);

  printf("Actual : %d || Expected : %d\n",ans,2*MAX);
  
  return 0;
}

