/*  =========================================================================
    future_pool -

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
    future_pool -
@discuss
@end
*/

#include "cfutures_classes.h"

struct _future_pool_t {
    queue_t *work_queue;
    executor_t *executors;
};

future_pool_t *
future_pool_new (int n_threads)
{
    assert (n_threads > 0);

    future_pool_t *self = (future_pool_t *) malloc (sizeof (future_pool_t));
    assert (self);

    self->work_queue = queue_new ();
    self->executors = NULL;

    for (int i = 0; i < n_threads; i++) {
        executor_t *next = executor_new (self->work_queue);
        executor_set_next (next, self->executors);
        self->executors = next;
    }

    return self;
}

void
cleanup_execute (future_t *future, void *args)
{
    promise_t *promise = (promise_t *) args;
    promise_destroy (&promise);
}

future_t *
future_pool_execute (future_pool_t *self, future_fn *fn, void *args)
{
    promise_t *promise = promise_new ();
    future_t *future = promise_get_future (promise);

    closure_t *closure = (closure_t *) malloc (sizeof (closure_t));
    assert (closure);
    *closure = (closure_t) { promise, fn, args };

    queue_enqueue (self->work_queue, closure);
    future_add_callback (future, cleanup_execute, (void *) promise);

    return future;
}

//  Destroy the future_pool
void
future_pool_destroy (future_pool_t **self_p)
{
    assert (self_p);
    if (*self_p) {
        future_pool_t *self = *self_p;

        executor_t *next = self->executors;
        while (next) {
            executor_t *tofree = next;
            next = executor_next (next);
            executor_destroy (&tofree);
        }

        queue_destroy (&self->work_queue);

        // Free object itself
        free (self);
        *self_p = NULL;
    }
}


//  --------------------------------------------------------------------------
//  Self test of this class.

typedef struct {
    int a, b;
} test_args_t;

void *
test_future_fn (void *args)
{
    test_args_t *test_args = (test_args_t *) args;
    sleep (1);
    long result = test_args->a * test_args->b;
    return (void *) result;
}

void
future_pool_test (bool verbose)
{
    printf (" * future_pool: ");

    //  @selftest
    future_pool_t *pool = future_pool_new (4);

    test_args_t args1 = {5, 3};
    test_args_t args2 = {10, 2};
    future_t *future1 = future_pool_execute (
        pool, test_future_fn, (void *) &args1);
    future_t *future2 = future_pool_execute (
        pool, test_future_fn, (void *) &args2);

    assert ((long) future_get (future1) == 15);
    assert ((long) future_get (future2) == 20);

    future_destroy (&future1);
    future_destroy (&future2);
    future_pool_destroy (&pool);
    //  @end

    printf ("OK\n");
}
