#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <semaphore.h>


typedef struct _chopset_struct{
    int chop;
    pthread_mutex_t protector;
}chopset_struct_T;

chopset_struct_T** chopset_generator(int number_idiots){
    int i;
    chopset_struct_T** chopset = (chopset_struct_T**)malloc(sizeof(chopset_struct_T*)*number_idiots);
    for(i=0;i<number_idiots;i++){
        chopset[i] = (chopset_struct_T*)malloc(sizeof(chopset_struct_T));
        chopset[i]->chop = 1;
        pthread_mutex_init(&chopset[i]->protector,NULL);
    }
    return chopset;
}

typedef struct _arg_struct{
    int id;
    int number_idiots;
    sem_t* lobby;
    chopset_struct_T** chopset;
}arg_struct_T;

arg_struct_T* arg_generator(int id, int number_idiots, chopset_struct_T** chopset, sem_t* lobby){
    arg_struct_T* argument = (arg_struct_T*) malloc(sizeof(arg_struct_T));
    argument->id = id;
    argument->number_idiots = number_idiots;
    argument->chopset = chopset;
    argument->lobby = lobby;
    return argument;
}


void get_chop_left(int left, chopset_struct_T** chopset){
    int l_left = left;
    pthread_mutex_lock(&chopset[l_left]->protector);
}

void get_chop_right(int right, chopset_struct_T** chopset){
    int l_right = right;
    pthread_mutex_lock(&chopset[l_right]->protector);
}

int put_chop(int left, int right, chopset_struct_T** chopset){
    int l_left,l_right;
    int return_val;
    l_left = left;
    l_right = right;

    pthread_mutex_unlock(&chopset[l_right]->protector);
    pthread_mutex_unlock(&chopset[l_left]->protector);
    return return_val;
}


void* idiots(void* argument){
    int interval;
    int left,right;
    arg_struct_T* l_argument = (arg_struct_T*)argument;
    left = (l_argument->id - 1)<0 ? l_argument->number_idiots-1 : l_argument->id - 1;
    right = l_argument->id;
    printf("%d left is %d\n",right,left);
    while(1){
        srand(time(NULL));
        interval = rand()%1000000;
        sleep(1);
        sem_wait(l_argument->lobby);
        get_chop_left(left, l_argument->chopset);
        printf("%d get left\n",right);
        get_chop_right(right, l_argument->chopset);
        printf("%d get right\n",right);
        printf("%d eating\n",right);
        interval = rand()%1000000;
        sleep(2);
        put_chop(left,right,l_argument->chopset);
        printf("%d finish\n\n",right);
        sem_post(l_argument->lobby);
    }
}

int main(int argc, char** argv){
    int number_idiots = 5;
    int i;
    sem_t lobby;
    sem_init(&lobby,0,number_idiots-1);
    chopset_struct_T** chopset = chopset_generator(number_idiots);
    pthread_t *thread_handles;
    thread_handles = (pthread_t *)malloc(sizeof(pthread_t)*number_idiots);
    arg_struct_T** arguments = (arg_struct_T**)malloc(sizeof(arg_struct_T*)*number_idiots);

    for(i=0;i<number_idiots;i++){
        arguments[i] = arg_generator(i,number_idiots,chopset,&lobby);
        pthread_create(&thread_handles[i],NULL,idiots,arguments[i]);
    }

    for(i=0;i<number_idiots;i++){
        pthread_join(thread_handles[i],NULL);
    }
}
