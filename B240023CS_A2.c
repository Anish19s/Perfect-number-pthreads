#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

struct range{
  int start;
  int end;
};
int N=0;
int sum=0;
pthread_mutex_t lock;


void* worker(void* arg){
    struct range* r=(struct range*)arg;
    for(int i=r->start;i<=r->end;i++){
            if(N%i==0){
                  pthread_mutex_lock(&lock);
                  sum+=i;
                  
                  int j=N/i;
                  
                  if(j!=i && j!=N){
                      sum+=j;
                  }
                  pthread_mutex_unlock(&lock);
            }
    }
    return NULL;
}

int main(int argc,char* arg[]){
    if(argc!=3){
        printf("Usage:%s <N><P>\n",arg[0]);
        return 0;
    }
    N=atoi(arg[1]);
    if(N<=1){
        printf("%d is not a perfect number\n",N);
        return 0;
    }
    int P=atoi(arg[2]);
    if(P<=0){
         printf("Invalid number of threads\n");
          return 0;  
    }
    int limit=(int)sqrt(N);
    if(limit<P){
          P=limit;
    }
    int chunk=limit/P;
    pthread_t threads[P];
    struct range ranges[P];
    pthread_mutex_init(&lock,NULL);
    int start=1;
    for(int i=0;i<P;i++){
          ranges[i].start=start;
          if(i==P-1){
              ranges[i].end=limit;}  
          else{
              ranges[i].end=start+chunk-1;}
        start=ranges[i].end+1;
        pthread_create(&threads[i],NULL,worker,&ranges[i]);
    }
    for(int i=0;i<P;i++){
        pthread_join(threads[i],NULL);
    }
    
    if(sum==N){
        printf("%d is a perfect number\n",N);
        return 0;}
    else{
      printf("%d is not a perfect number\n",N);
        return 0;}
    pthread_mutex_destroy(&lock);
    return 0;
}
