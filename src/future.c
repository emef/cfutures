/*  =========================================================================
    future -

    The MIT License (MIT)

    Copyright (c) 2016 Matt Forbes

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
    =========================================================================
*/

/*
@header
    future -
@discuss
@end
*/
#include "cfutures_classes.h"

typedef struct _callback_t callback_t;
struct _callback_t {
    callback_t *next;
    future_callback_fn *callback_fn;
    void *args;
};

struct _future_t {
    // Synchronization
    pthread_mutex_t lock_mutex;
    pthread_cond_t wait_cv;

    // State
    bool is_resolved;
    bool is_fulfilled;
    int rejected_rc;
    void *value;
    callback_t *callbacks;
};


// Callback constructor/destructor
// ------------------------------
callback_t *
callback_new (future_callback_fn *callback_fn, void *args)
{
    callback_t *self = (callback_t *) malloc (sizeof (callback_t));
    assert (self);

    self->next = NULL;
    self->args = args;
    self->callback_fn = callback_fn;

    return self;
}

void
callback_destroy (callback_t **self_p)
{
    assert (self_p);
    if (*self_p) {
        callback_t *self = *self_p;
        *self_p = NULL;

        //  Free entire list
        callback_t *next;
        while (self) {
            next = self->next;
            free (self);
            self = next;
        }
    }
}

void
callback_execute (callback_t *self, future_t *future)
{
    assert (self);
    assert (self->callback_fn);
    assert (self->args);
    self->callback_fn (future, self->args);
}

// Future constructor/destructor
// ------------------------------
future_t *
future_new (void)
{
    future_t *self = (future_t *) malloc (sizeof (future_t));
    assert (self);

    self->value = NULL;
    self->callbacks = NULL;
    self->rejected_rc = 0;
    self->is_resolved = self->is_fulfilled = 0;
    pthread_mutex_init(&self->lock_mutex, NULL);
    pthread_cond_init (&self->wait_cv, NULL);

    return self;
}

//  Destroy the future
void
future_destroy (future_t **self_p)
{
    assert (self_p);
    if (*self_p) {
        future_t *self = *self_p;

        pthread_mutex_destroy(&self->lock_mutex);
        pthread_cond_destroy(&self->wait_cv);

        //  Free object itself
        free (self);
        *self_p = NULL;
    }
}

void
future_wait (future_t *self)
{
    pthread_mutex_lock (&self->lock_mutex);
    while (!self->is_resolved)
        pthread_cond_wait (&self->wait_cv, &self->lock_mutex);
    pthread_mutex_unlock (&self->lock_mutex);
}

void *
future_get (future_t *self)
{
    future_wait (self);

    assert (self->is_resolved);
    assert (self->is_fulfilled);
    return self->value;
}

void
future_execute_callbacks (future_t *self)
{
    assert (future_is_resolved (self));

    callback_t *callback = self->callbacks;
    while (callback) {
        callback_execute (callback, self);
        callback = callback->next;
    }
}

void
future_set_value (future_t *self, void *value)
{
    pthread_mutex_lock (&self->lock_mutex);

    self->value = value;
    self->is_resolved = self->is_fulfilled = true;

    // Signal all waiting threads.
    pthread_cond_broadcast (&self->wait_cv);

    pthread_mutex_unlock (&self->lock_mutex);

    // Execute all callbacks now that future is resolved.
    future_execute_callbacks (self);
}

void
future_set_rejected (future_t *self, int rc)
{
    pthread_mutex_lock (&self->lock_mutex);

    self->rejected_rc = rc;
    self->is_resolved = true;

    // Signal all waiting threads.
    pthread_cond_broadcast (&self->wait_cv);

    pthread_mutex_unlock (&self->lock_mutex);

    // Execute all callbacks now that future is resolved.
    future_execute_callbacks (self);
}

int
future_rejected_code (future_t *self)
{
    return self->rejected_rc;
}

void
future_add_callback (
    future_t *self, future_callback_fn *callback_fn, void *args)
{
    pthread_mutex_lock (&self->lock_mutex);

    callback_t *link = callback_new (callback_fn, args);
    link->next = self->callbacks;
    self->callbacks = link;

    pthread_mutex_unlock (&self->lock_mutex);
}

//  Returns true if future has been resolved (either fulfilled or rejected).
bool
future_is_resolved (future_t *self)
{
    return self->is_resolved;
}

//  Returns true if future has been fulfilled.
bool
future_is_fulfilled (future_t *self)
{
    return self->is_fulfilled;
}

//  --------------------------------------------------------------------------
//  Self test of this class.

void *
test_future_fulfill (void *thread_args)
{
    promise_t *promise = (promise_t *) thread_args;
    sleep (1);

    long value = 12345;
    promise_fulfill (promise, (void *) value);

    pthread_exit (NULL);
}

void
test_callback (future_t *future, void *args)
{
    assert (future);
    assert (future_is_resolved (future));

    long *result = (long *) args;
    *result = (long) future_get (future);
}

void
future_test (bool verbose)
{
    printf (" * future: ");

    //  @selftest
    long result_1 = 0, result_2 = 0;
    promise_t *promise = promise_new ();
    future_t *future = promise_get_future (promise);
    pthread_t thread;

    // Add 2 callbacks to the future.
    future_add_callback (future, test_callback, (void *) &result_1);
    future_add_callback (future, test_callback, (void *) &result_2);

    // Create the thread which will resolve the future in 1 second.
    pthread_create (&thread, NULL, test_future_fulfill, (void *) promise);

    // Wait for future completion.
    future_wait (future);
    pthread_join (thread, NULL);

    assert (result_1 == 12345);
    assert (result_2 == 12345);

    promise_destroy (&promise);
    future_destroy (&future);

    //  @end

    printf ("OK\n");
}
