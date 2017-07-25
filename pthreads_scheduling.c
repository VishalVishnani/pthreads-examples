#include <stdio.h>
#include <pthread.h>

int main()
{
  pthread_t threads[3];
  pthread_attr_t main_sched_attr;
  struct sched_param main_param;



  int policy,ret;
  int concurrency_level;
  int inherit;
  
  pthread_attr_init(&main_sched_attr);

  /*If concurrency level is not set befor get, the value returned by get will be 0*/

  /*set concurrency level*/
  pthread_setconcurrency(2);

  /*get concurrency level*/
  concurrency_level=pthread_getconcurrency();

  printf("The concurrency level is %d\n",concurrency_level);


  /*If scheduling policy is not set, by defualt the scheduling policy is SCHED_OTHER*/
  /*set scheduling policy*/
  pthread_attr_setschedpolicy(&main_sched_attr,SCHED_RR);

  /*get scheduling policy*/
  ret=pthread_attr_getschedpolicy(&main_sched_attr,&policy);

  if(policy==0)
    printf("The scheduling policy is SCHED_OTHER \n");
  else if(policy==1)
    printf("The scheduling policy is SCHED_FIFO \n");
  else
    printf("The scheduling policy is SCHED_RR \n");


  /*INHERIT->Scheduling polices in creating thread are to be used,
  *attribute in pthread_create() is to be ignored.
  *EXPLICIT->Scheduling policy of the attribute in pthread create
  *is to be used
  *Default: INHERIT*/

  /*set inheritance value*/
  ret=pthread_attr_setinheritsched(&main_sched_attr,PTHREAD_EXPLICIT_SCHED);

  /*get inheritance value*/
  ret=pthread_attr_getinheritsched(&main_sched_attr,&inherit);

  if(inherit==0)
    printf("The inheritance is set to inherit\n");
  else
   printf("The inheritance is set to explicit\n");

  /*To set the priority for a thread, first set the priority in the 
  *Sched_param structure, and then create the thread*/

  /*Get the maximum and minimum value of a scheduling policy*/
  int max_priority=sched_get_priority_max(SCHED_FIFO);
  int min_priority=sched_get_priority_min(SCHED_FIFO);

  printf("The maximum and minimum priorities for FIFO are %d and %d\n",max_priority,min_priority);

  /*Set priority in the structure*/
  main_param.sched_priority=max_priority;
  
  /*Set the priority in the attribute*/
  ret=pthread_attr_setschedparam(&main_sched_attr,&main_param); 

  /*Get the priority from the attribute*/
  ret=pthread_attr_getschedparam(&main_sched_attr,&main_param);
  int priority=main_param.sched_priority;
  printf("The priority set is %d\n",priority);

  //Create threads

}

