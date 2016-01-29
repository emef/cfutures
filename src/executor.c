/*  =========================================================================
    executor -

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
    executor -
@discuss
@end
*/

#include "cfutures_classes.h"

struct _executor_t {
    pthread_t thread;
    executor_t *next;
};

void *
executor_thread (void *args)
{
    int rc = pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    assert (rc == 0);

    queue_t *queue = (queue_t *) args;

    while (1) {
        closure_t *closure = (closure_t *) queue_pop (queue);

        void *value = closure->fn(closure->args);
        promise_fulfill (closure->promise, value);
        free (closure);
    }

    return NULL;
}

executor_t *
executor_new (queue_t *work_queue)
{
    executor_t *self = (executor_t *) malloc (sizeof (executor_t));
    assert (self);

    pthread_create (&self->thread, NULL, executor_thread, (void *) work_queue);
    self->next = NULL;

    return self;
}

//  Destroy the executor
void
executor_destroy (executor_t **self_p)
{
    assert (self_p);
    if (*self_p) {
        executor_t *self = *self_p;

        int rc = pthread_cancel(self->thread);
        assert (rc == 0);

        void *res = NULL;
        rc = pthread_join (self->thread, &res);
        assert (rc == 0);
        assert (res == PTHREAD_CANCELED);

        //  Free object itself
        free (self);
        *self_p = NULL;
    }
}

executor_t *
executor_next (executor_t *self)
{
    return self->next;
}

void
executor_set_next (executor_t *self, executor_t *next)
{
    self->next = next;
}

//  --------------------------------------------------------------------------
//  Self test of this class.

void*
executor_work_identity_thread (void *args)
{
    sleep (1);
    return args;
}

void
executor_test (bool verbose)
{
    printf (" * executor: ");

    //  @selftest

    queue_t *queue = queue_new ();
    executor_t *executor = executor_new (queue);

    promise_t *promise = promise_new ();
    closure_t *closure = (closure_t *) malloc (sizeof (closure_t));
    *closure = (closure_t) {
        promise,
        executor_work_identity_thread,
        (void *) 10L
    };

    queue_enqueue (queue, (void *) closure);

    future_t *future = promise_get_future (promise);
    long result = (long) future_get (future);
    assert (result == 10);

    executor_destroy (&executor);
    queue_destroy (&queue);

    //  @end

    printf ("OK\n");
}
