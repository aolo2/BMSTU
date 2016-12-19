#include "lock.h"

lock::lock(pthread_mutex_t *mutex){
    this->mutex = mutex;
    pthread_mutex_lock(this->mutex);
}

lock::~lock() {
    pthread_mutex_unlock(this->mutex);
}
