#ifndef INTERNAL_H_INCLUDED
#define INTERNAL_H_INCLUDED

#define streq(s1,s2) (!strcmp ((s1), (s2)))

// Opaque types declarations.
typedef struct _executor_t executor_t;
typedef struct _queue_t queue_t;

typedef void* (future_fn) (void *args);

typedef struct {
    promise_t *promise;
    future_fn *fn;
    void *args;
} closure_t;

// Project-private constructor for futures.
future_t *
    future_new (void);

// Project-private future set function.
CFUTURES_EXPORT void
    future_set_value (future_t *self, void *value);

// Project-private future set function.
CFUTURES_EXPORT void
    future_set_rejected (future_t *self, int rc);

#define BILLION 1000000000L
#define MILLION 1000000L
inline struct timespec get_abs_future_time (unsigned milli)
{
    struct timespec ts;         /* ns since 1 Jan 1970 to 1500 ms in the future */
    struct timeval  micro = {0, 0}; /* 1 Jan 1970 */

    (void) gettimeofday(&micro, NULL);
    ts.tv_sec = micro.tv_sec;
    ts.tv_nsec = micro.tv_usec * 1000;
    ts.tv_sec += milli / 1000u;
    ts.tv_nsec += (milli % 1000u) * MILLION;
    ts.tv_sec += ts.tv_nsec / BILLION;
    ts.tv_nsec = ts.tv_nsec % BILLION;

    return ts;
}

#endif
