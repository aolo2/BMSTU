#include "mutex_wrapper.h"

mutex_wrapper::mutex_wrapper(pthread_mutex_t *mutex){
    this->mutex = mutex;
    pthread_mutex_lock(this->mutex);
}

mutex_wrapper::~mutex_wrapper() {
    pthread_mutex_unlock(this->mutex);
}
