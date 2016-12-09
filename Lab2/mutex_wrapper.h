#ifndef MUTEX_WRAPPER_H
#define MUTEX_WRAPPER_H

#include <pthread.h>

class mutex_wrapper {
public:
    mutex_wrapper(pthread_mutex_t *mutex);
    virtual ~mutex_wrapper();
private:
    pthread_mutex_t *mutex;
};

#endif
