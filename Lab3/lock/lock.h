#ifndef MUTEX_WRAPPER_H
#define MUTEX_WRAPPER_H

#include <pthread.h>

class lock {
public:
    lock(pthread_mutex_t *mutex);
    virtual ~lock();
private:
    pthread_mutex_t *mutex;
};

#endif
