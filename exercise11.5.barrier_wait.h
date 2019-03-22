pthread_cond_t bready = PTHREAD_COND_INITIALIZER; 
pthread_mutex_t balock = PTHREAD_MUTEX_INITIALIZER;

struct {
    pthread_mutex_t balock;
    int tcount;
    int flag;
}x;

int pthread_barrier_wait(pthread_barrier_t *barrier){
    static tcount = 1;
    pthread_mutex_lock(&x.balock); 
    while (tcount != x.tcount){ 
        ++tcount;
        pthread_cond_wait((pthread_cond_t *)barrier, &x.balock);
    }
    if(flag){
        flag = 0;
        pthread_cond_broadcast((pthread_cond_t *)barrier);
    }
    pthread_mutex_unlock(&x.balock);
    
}


int pthread_cond_wait(pthread_cond_t *restrict cond, pthread_mutex_t *restrict mutex){

}
