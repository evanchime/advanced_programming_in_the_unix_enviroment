pthread_cond_t bready = PTHREAD_COND_INITIALIZER; 
pthread_mutex_t balock = PTHREAD_MUTEX_INITIALIZER;

struct {
    pthread_mutex_t balock;
    int tcount;
}x;

int pthread_barrier_wait(pthread_barrier_t *barrier){
    static tcount = 0;
    pthread_mutex_lock(&x.balock); 
    while (++tcount != x.tcount) 
        pthread_cond_wait(bready, &balock); 
}


int pthread_cond_wait(pthread_cond_t *restrict cond, pthread_mutex_t *restrict mutex){

}