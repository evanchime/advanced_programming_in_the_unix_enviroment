struct {
    pthread_mutex_t balock;
    int tcount;
}

int pthread_barrier_wait(pthread_barrier_t *barrier){

    pthread_mutex_lock(&qlock); 
    while (workq == NULL) pthread_cond_wait(&qready, &qlock); 
}


int pthread_cond_wait(pthread_cond_t *restrict cond, pthread_mutex_t *restrict mutex){

}