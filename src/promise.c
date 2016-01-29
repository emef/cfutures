/*  =========================================================================
    promise -

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
    promise -
@discuss
@end
*/

#include "cfutures_classes.h"

struct _promise_t {
    future_t *future;  // DOES NOT OWN, MUST BE DESTROYED BY CALLER.
};

promise_t *
promise_new (void)
{
    promise_t *self = (promise_t *) malloc (sizeof (promise_t));
    assert (self);

    self->future = future_new ();

    return self;
}

//  Destroy the promise
void
promise_destroy (promise_t **self_p)
{
    assert (self_p);
    if (*self_p) {
        promise_t *self = *self_p;

        //  Free object itself
        free (self);
        *self_p = NULL;
    }
}

future_t *
promise_get_future (promise_t *self)
{
    return self->future;
}

void
promise_fulfill (promise_t *self, void *value)
{
    future_set_value (self->future, value);
}

void
promise_reject (promise_t *self, int rc)
{
    future_set_rejected (self->future, rc);
}


//  --------------------------------------------------------------------------
//  Self test of this class.
void *
test_fulfill_thread (void *thread_args)
{
    promise_t *promise = (promise_t *) thread_args;
    sleep (1);

    char *value = (char *) malloc (256 * sizeof (char));
    strcpy (value, "value");
    promise_fulfill (promise, (void *) value);

    pthread_exit (NULL);
}

void *
test_reject_thread (void *thread_args)
{
    promise_t *promise = (promise_t *) thread_args;
    sleep (1);

    promise_reject (promise, -1);

    pthread_exit (NULL);
}

void
promise_test (bool verbose)
{
    printf (" * promise: ");

    //  @selftest

    // Test promise is fulfilled.
    pthread_t thread1;
    promise_t *promise1 = promise_new ();
    future_t *future1 = promise_get_future (promise1);

    pthread_create (&thread1, NULL, test_fulfill_thread, (void *) promise1);
    char *value = (char *) future_get (future1);
    assert (streq (value, "value"));
    free (value);

    pthread_join (thread1, NULL);
    promise_destroy (&promise1);
    future_destroy (&future1);

    // Test promise is rejected.
    pthread_t thread2;
    promise_t *promise2 = promise_new ();
    future_t *future2 = promise_get_future (promise2);

    pthread_create (&thread2, NULL, test_reject_thread, (void *) promise2);
    future_wait (future2);
    assert (future_is_resolved (future2));
    assert (future_is_fulfilled (future2) == false);
    assert (future_rejected_code (future2) == -1);

    pthread_join (thread2, NULL);
    promise_destroy (&promise2);
    future_destroy (&future2);
    //  @end

    printf ("OK\n");
}
