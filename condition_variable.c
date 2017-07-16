#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NTHREADS 3
#define TCOUNT 10
#define COUNT_LIMIT 12

int count=0;
int thread_ids[3]={0,1,2};
pthread_mutex_t count_mutex;
pthread_cond_t count_threshold_cv;

void * watch_count(void *t){
  long my_id=(long)t;
  printf("Starting watch count() : thread %ld\n",my_id);
  pthread_mutex_lock(&count_mutex);
  
  while(count<COUNT_LIMIT){
    pthread_cond_wait(&count_threshold_cv,&count_mutex);
    printf("watch count(): thread %ld condition signal received\n",my_id);

  }
  count+=125;
  printf("watch count(); thread %ld count now %d\n",my_id,count);
  pthread_mutex_unlock(&count_mutex);
  pthread_exit(NULL);

}

void* inc_count(void *t){
  int i;
  long my_id=(long)t;
  
  for(i=0;i<TCOUNT;i++){
    pthread_mutex_lock(&count_mutex);
    count++;
    
    if(count==COUNT_LIMIT){
      pthread_cond_signal(&count_threshold_cv);
      printf("inc count(); thread %ld , count = %d , threshold reached.\n",my_id,count);
    }
    
    printf("inc count(): thread %ld , count = %d, unlocking mutex\n",my_id,count);
    pthread_mutex_unlock(&count_mutex);
    sleep(1);
  }
  pthread_exit(NULL);
}

int main()
{
  int i;
  long t1=1,t2=2,t3=3;
  pthread_t threads[3];
  pthread_attr_t attr;
 
  pthread_mutex_init(&count_mutex,NULL);
  pthread_cond_init(&count_threshold_cv,NULL);
  
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);

  pthread_create(&threads[0],&attr,watch_count,(void*)t1);
  pthread_create(&threads[1],&attr,inc_count,(void*)t2);
  pthread_create(&threads[2],&attr,inc_count,(void*)t3);

  for(i=0;i<NTHREADS;i++){
    pthread_join(threads[i],NULL);

  }
  printf("Done waiting\n");
  pthread_attr_destroy(&attr);
  pthread_mutex_destroy(&count_mutex);
  pthread_cond_destroy(&count_threshold_cv);
  pthread_exit(NULL);
}

