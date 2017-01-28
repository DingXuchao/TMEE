#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>


int counter = 0;

typedef struct _s_string{
    char* s_string;
    int cnt;
}T_s_str;

T_s_str* T_s_str_init(int size){
    int i;
    T_s_str* s_str = (T_s_str*) malloc(sizeof(T_s_str));
    s_str->s_string = (char*)malloc((sizeof(char)*size));
    for(i=0; i< size; i++){
        s_str->s_string[i] = (char)0;
    }
    s_str->cnt = 0;
    return s_str;
}

void T_s_str_log(T_s_str* str){
    int i;
    for(i=0;i<str->cnt;i++){
        printf("%c ",str->s_string[i]);
    }
    printf("\n");
    printf("str current cnt is %d\n",str->cnt);
}

int T_s_str_append(T_s_str* str, char data, int limit){
    int state = -1; //-1 failed length constrain 1 success;
    if(str->cnt >= limit){
        return state;
    }
    else{
        str->s_string[str->cnt] = data;
        str->cnt ++;
        state = 1;
    }
    return state;
}

typedef struct _job_pool{
    char** job;
    int cnt;
}T_job_pool;

T_job_pool* T_job_pool_init(T_s_str* str,int segcnt, int seglen){
    int i,j;
    T_job_pool* job_pool = (T_job_pool*)malloc(sizeof(T_job_pool));
    job_pool->job = malloc(sizeof(char*)*(segcnt));
    for(i=0;i<segcnt;i++){
        job_pool->job[i] = malloc(sizeof(char)*seglen);
    }
    for(i=0;i<segcnt;i++){
        for(j=0;j<seglen;j++){
            job_pool->job[i][j]=str->s_string[(i*seglen)+j];
        }
    }
    job_pool->cnt = 0;
    return job_pool;
}

void T_job_pool_log(T_job_pool* job_pool,int segcnt){
    int i;
    for(i=0;i<segcnt;i++){
        printf("%s\n",job_pool->job[i]);
    }
    printf("job pool log cnt is %d\n\n",job_pool->cnt);
}

char* T_job_pool_get(T_job_pool* job_pool,int limit){
    char* temp;
    if(job_pool->cnt >= limit){
//        *temp = (char)0;
        return NULL;
    }
    else {
        temp = job_pool->job[job_pool->cnt];
        job_pool->cnt++;
    }
    return temp;
}

int F0_verify(char* str,int limit,char c0, char c1, char c2){
    int i,c0_cnt,c1_cnt,c2_cnt;
    c0_cnt = 0;
    c1_cnt = 0;
    c2_cnt = 0;
    printf("checking with %s\n",str);
    printf("c0 %c c1 %c c2 %c\n",c0,c1,c2);
    for(i=0;i<limit;i++){
        printf("str[%d] %c\n",i,str[i]);
        if(str[i] == c0 || str[i] == c1 || str[i] == c2){
            if(str[i] == c0) c0_cnt++;
            else if(str[i] == c1) c1_cnt++;
            else c2_cnt++;
        }
    }
    printf("c0_cnt %d c1_cnt %d c2_cnt %d\n",c0_cnt,c1_cnt,c2_cnt);
    if((c0_cnt + c1_cnt) == c2_cnt) return 1;
    else return 0;
}

int F1_verify(char* str,int limit,char c0, char c1, char c2){
    int i,c0_cnt,c1_cnt,c2_cnt;
    c0_cnt = 0;
    c1_cnt = 0;
    c2_cnt = 0;
    for(i=0;i<limit;i++){
        if(str[i] == c0 || str[i] == c1 || str[i] == c2){
            if(str[i] == c0) c0_cnt++;
            else if(str[i] == c1) c1_cnt++;
            else c2_cnt++;
        }
    }
    printf("c0_cnt %d c1_cnt %d c2_cnt %d\n",c0_cnt,c1_cnt,c2_cnt);
    if((c0_cnt + 2*c1_cnt) == c2_cnt) return 1;
    else return 0;
}

int F2_verify(char* str,int limit,char c0, char c1, char c2){
    int i,c0_cnt,c1_cnt,c2_cnt;
    c0_cnt = 0;
    c1_cnt = 0;
    c2_cnt = 0;
    for(i=0;i<limit;i++){
        if(str[i] == c0 || str[i] == c1 || str[i] == c2){
            if(str[i] == c0) c0_cnt++;
            else if(str[i] == c1) c1_cnt++;
            else c2_cnt++;
        }
    }
    printf("c0_cnt %d c1_cnt %d c2_cnt %d\n",c0_cnt,c1_cnt,c2_cnt);
    if((c0_cnt * c1_cnt) == c2_cnt) return 1;
    else return 0;
}

int F3_verify(char* str,int limit,char c0, char c1, char c2){
    int i,c0_cnt,c1_cnt,c2_cnt;
    c0_cnt = 0;
    c1_cnt = 0;
    c2_cnt = 0;
    for(i=0;i<limit;i++){
        if(str[i] == c0 || str[i] == c1 || str[i] == c2){
            if(str[i] == c0) c0_cnt++;
            else if(str[i] == c1) c1_cnt++;
            else c2_cnt++;
        }
    }
    printf("c0_cnt %d c1_cnt %d c2_cnt %d\n",c0_cnt,c1_cnt,c2_cnt);
    if((c0_cnt - c1_cnt) == c2_cnt) return 1;
    else return 0;
}


typedef struct _str_gen_arg{
    int id;
    T_s_str* shared_str;
    int limit;
    pthread_mutex_t* str_mutex;
}T_str_gen_arg;

T_str_gen_arg* str_gen_arg_generate(int id,T_s_str* shared_str, int limit, pthread_mutex_t* str_mutex){
    T_str_gen_arg* str_gen_arg= (T_str_gen_arg*)malloc(sizeof(T_str_gen_arg));
    str_gen_arg->id = id;
    str_gen_arg->shared_str = shared_str;
    str_gen_arg->limit = limit;
    str_gen_arg->str_mutex = str_mutex;
    return str_gen_arg;
}

void *str_generator(void* str_arg){
    T_str_gen_arg* str_argument = (T_str_gen_arg*) str_arg;
    int my_rank = str_argument->id;
    char element = (char) ((int)my_rank + 'a');
    int status = 1;
    int interval;

    do{
        pthread_mutex_lock(str_argument->str_mutex);
        status = T_s_str_append(str_argument->shared_str,element,str_argument->limit);
        //debug
        printf("thread id is %d\n",my_rank);
        printf("current S is \n");
        T_s_str_log(str_argument->shared_str);
        pthread_mutex_unlock(str_argument->str_mutex);
        srand(time(NULL));
        interval = rand()%401;
        interval = 100+ interval;
        usleep(interval);
    }
    while(status != -1);
    return 0;
}

typedef struct _verify_arg{
    int (*verification)(char* str,int limit,char c0, char c1, char c2);
    T_job_pool* job_pool;
    int limit;
    char c0;
    char c1;
    char c2;
    int ans;
    int id;
    pthread_mutex_t* verify_mutex;
}T_verify_arg;

T_verify_arg* T_verify_arg_generater(int id,T_job_pool* pool, int limit, char c0, char c1, char c2,int (*f)(char* str,int limit,char c0, char c1, char c2),pthread_mutex_t* mutex){
    T_verify_arg* verify_arg = (T_verify_arg*)malloc(sizeof(T_verify_arg));
    verify_arg->id = id;
    verify_arg->job_pool = pool;
    verify_arg->verification = f;
    verify_arg->limit = limit;
    verify_arg->c0 = c0;
    verify_arg->c1 = c1;
    verify_arg->c2 = c2;
    verify_arg->ans = 0;
    verify_arg->verify_mutex = mutex;
    return verify_arg;
}

void *str_verification(void* verify_argument){
    T_verify_arg* verify_arg = (T_verify_arg*) verify_argument;
    char* check_str;
    pthread_mutex_lock(verify_arg->verify_mutex);
    check_str = T_job_pool_get(verify_arg->job_pool,verify_arg->limit);
    pthread_mutex_unlock(verify_arg->verify_mutex);
    if(check_str==(char*)NULL){
        return 0;
    }
    while(check_str!=(char*)NULL){
      printf("thread %d\n",verify_arg->id);
      verify_arg->ans += (*verify_arg->verification)(check_str,verify_arg->limit,verify_arg->c0,verify_arg->c1,verify_arg->c2);
      pthread_mutex_lock(verify_arg->verify_mutex);
      check_str = T_job_pool_get(verify_arg->job_pool,verify_arg->limit-1);
      T_job_pool_log(verify_arg->job_pool,verify_arg->limit);
      pthread_mutex_unlock(verify_arg->verify_mutex);
    }
    return 0;
}

typedef struct _total_arg{
    T_verify_arg * verify_arg;
    T_str_gen_arg* str_argument;
    int M;
    int L;
    int N;
    int counter;
    pthread_mutex_t* condition_mutex;
    pthread_cond_t * cond_var;
}T_total_arg;

T_total_arg* T_arg_generater(int id,T_job_pool* pool, int limit, char c0, char c1, char c2,
    int (*f)(char* str,int limit,char c0, char c1, char c2),
    pthread_mutex_t* mutex, T_s_str* shared_str, int total_length, pthread_mutex_t* str_mutex,
    int M, int L,int N, pthread_mutex_t* condition_mutex, pthread_cond_t * cond_var){
        T_total_arg* arguments = (T_total_arg*)malloc(sizeof(T_total_arg));
        arguments->str_argument = str_gen_arg_generate(id,shared_str,total_length,str_mutex);
        arguments->verify_arg = T_verify_arg_generater(id,pool,L,c0,c1,c2,f,mutex);
        arguments->M = M;
        arguments->L = L;
        arguments->N = N;
        arguments->condition_mutex = condition_mutex;
        arguments->cond_var = cond_var;
        return arguments;
    }

void *thread_dowork(void* arguments){
    T_total_arg* total_arg = (T_total_arg*)arguments;
    T_verify_arg* verify_arg = total_arg->verify_arg;
    T_str_gen_arg* str_argument = total_arg->str_argument;

    int my_rank = str_argument->id;
    char element = (char) ((int)my_rank + 'a');
    int status = 1;
    int interval;

    do{
        pthread_mutex_lock(str_argument->str_mutex);
        status = T_s_str_append(str_argument->shared_str,element,str_argument->limit);
        //debug
        pthread_mutex_unlock(str_argument->str_mutex);
        srand(time(NULL));
        interval = rand()%401;
        interval = 100+ interval;
        usleep(interval);
    }
    while(status != -1);
    pthread_mutex_lock(total_arg->condition_mutex);
    counter++;
    if(counter == total_arg->N){
        verify_arg->job_pool = T_job_pool_init(str_argument->shared_str,total_arg->M,total_arg->L);
        T_s_str_log(str_argument->shared_str);
        T_job_pool_log(verify_arg->job_pool,total_arg->M);
        pthread_cond_broadcast(total_arg->cond_var);
    }
    else{
        while(pthread_cond_wait(total_arg->cond_var,total_arg->condition_mutex)!=0);
    }
    pthread_mutex_unlock(total_arg->condition_mutex);


    char* check_str;
    pthread_mutex_lock(verify_arg->verify_mutex);
    check_str = T_job_pool_get(verify_arg->job_pool,verify_arg->limit);
    pthread_mutex_unlock(verify_arg->verify_mutex);
    if(check_str==(char*)NULL){
        return 0;
    }
    if(strcmp(check_str,"")==0) return 0;
    while(check_str!=(char*)NULL){
      if(strcmp(check_str,"")==0) break;
      verify_arg->ans += (*verify_arg->verification)(check_str,verify_arg->limit,verify_arg->c0,verify_arg->c1,verify_arg->c2);
      pthread_mutex_lock(verify_arg->verify_mutex);
      check_str = T_job_pool_get(verify_arg->job_pool,verify_arg->limit);
//      T_job_pool_log(verify_arg->job_pool,verify_arg->limit);
      pthread_mutex_unlock(verify_arg->verify_mutex);
    }
    return 0;
}

int main (int argc, char* argv[]){
    int temp;
    char* check;
    int i;
    int F; // verification rules
    int N; //# of threads
    int L; //length of segement
    int M; //number of segement
    char C0;
    char C1;
    char C2;
    int ans = 0;
    int (*verification)(char* str,int limit,char c0, char c1, char c2);
    T_s_str* shared_str;
    T_job_pool* job_pool;
    T_total_arg** arguments;
    T_str_gen_arg** str_gen_arg;
    T_verify_arg** verify_arg;

//    long thread;
    pthread_mutex_t str_mutex;
    pthread_mutex_t verify_mutex;
    pthread_mutex_t cond_mutex;
    pthread_cond_t  cond_var;
    pthread_t *thread_handles_p1;
    pthread_t *thread_handles_p2;

    if(argc != 8){
        printf("fq\n");
        return -1;
    }
    else{
        F = (int)(*argv[1]-48);
        N = (int)(*argv[2]-48);
        L = (int)(*argv[3]-48);
        M = (int)(*argv[4]-48);
        C0 = *argv[5];
        C1 = *argv[6];
        C2 = *argv[7];
//        thread_count = N;
        pthread_mutex_init(&str_mutex, NULL);
        pthread_mutex_init(&verify_mutex,NULL);
        pthread_mutex_init(&cond_mutex,NULL);
        pthread_cond_init(&cond_var,NULL);
        shared_str = T_s_str_init(L*M);
        job_pool = T_job_pool_init(shared_str,M,L);
        thread_handles_p1 = malloc(N*sizeof(pthread_t));
        thread_handles_p2 = malloc(N*sizeof(pthread_t));
        str_gen_arg = (T_str_gen_arg**)malloc(sizeof(T_str_gen_arg*)*N);
        verify_arg = (T_verify_arg**)malloc(sizeof(T_verify_arg*)*N);
        arguments = (T_total_arg**)malloc(sizeof(T_total_arg*)*N);



        if(F==0) verification = &F0_verify;
        else if(F==1) verification = &F1_verify;
        else if(F==2) verification = &F2_verify;
        else verification = &F3_verify;

        for(i=0;i<N;i++){
            arguments[i] = T_arg_generater(i,job_pool,M,C0,C1,C2,verification,&verify_mutex,
                shared_str,L*M, &str_mutex,M,L,N,&cond_mutex,&cond_var);
            pthread_create(&thread_handles_p1[i],NULL,thread_dowork,arguments[i]);
        }

        for(i=0;i<N;i++){
            pthread_join(thread_handles_p1[i],NULL);
            ans+=arguments[i]->verify_arg->ans;
        }
        printf("ans is %d\n",ans);
        return 0;
    }

}
