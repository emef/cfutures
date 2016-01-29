/*  =========================================================================
    queue -

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
    queue -
@discuss
@end
*/

#include "cfutures_classes.h"

typedef struct _item_t item_t;
struct _item_t {
    item_t *next;
    item_t *prev;
    void *value;
};

struct _queue_t {
    // Synchronization
    pthread_mutex_t lock_mutex;
    pthread_cond_t wait_cv;

    // Doubly-linked list
    item_t *head, *tail;
};

void
thread_relinquish_lock (void *arg)
{
    pthread_mutex_t *lock = (pthread_mutex_t *) arg;
    pthread_mutex_unlock (lock);
}

queue_t *
queue_new (void)
{
    queue_t *self = (queue_t *) malloc (sizeof (queue_t));
    assert (self);

    self->head = self->tail = NULL;
    pthread_mutex_init(&self->lock_mutex, NULL);
    pthread_cond_init (&self->wait_cv, NULL);

    return self;
}

void
queue_destroy (queue_t **self_p)
{
    assert (self_p);
    if (*self_p) {
        queue_t *self = *self_p;

        pthread_mutex_destroy(&self->lock_mutex);
        pthread_cond_destroy(&self->wait_cv);

        item_t *head = self->head;
        while (head) {
            free (head);
            head = head->next;
        }

        //  Free object itself
        free (self);
        *self_p = NULL;
    }
}

void
queue_enqueue (queue_t *self, void *value)
{
    item_t *item = (item_t *) malloc (sizeof (item_t));
    assert (item);

    pthread_mutex_lock (&self->lock_mutex);
    pthread_cleanup_push (thread_relinquish_lock, &self->lock_mutex);
    // begin critical section

    item->value = value;
    item->next = NULL;
    item->prev = self->tail;
    if (self->tail != NULL)
        self->tail->next = item;
    if (self->head == NULL)
        self->head = item;
    self->tail = item;

    // signal that there is an item available.
    pthread_cond_signal (&self->wait_cv);

    // end critical section
    pthread_cleanup_pop (1);
}

void *
queue_pop (queue_t *self)
{
    // eventual return value.
    item_t *item;

    pthread_mutex_lock (&self->lock_mutex);
    pthread_cleanup_push (thread_relinquish_lock, &self->lock_mutex);
    // begin critical section

    while (!self->head)
        pthread_cond_wait (&self->wait_cv, &self->lock_mutex);

    item = self->head;
    if (!item->next) {
        self->tail = self->head = NULL;
    } else {
        self->head = item->next;
        if (self->head->next == NULL)
            self->tail = NULL;
    }

    // end critical section
    pthread_cleanup_pop (1);

    void *value = item->value;
    free (item);

    return value;
}

//  --------------------------------------------------------------------------
//  Self test of this class.

void *
test_queue_thread (void *args)
{
    sleep (1);

    queue_t *queue = (queue_t *) args;
    for (long i=0; i < 10; i++) {
        void *v = (void *) i;
        queue_enqueue (queue, v);
    }

    pthread_exit (NULL);
}

void
queue_test (bool verbose)
{
    printf (" * queue: ");

    //  @selftest
    queue_t *queue = queue_new ();
    pthread_t thread;
    pthread_create (&thread, NULL, test_queue_thread, (void *) queue);

    for (long i = 0; i < 10; i++)
    {
        long v = (long) queue_pop (queue);
        printf ("got %lu, wanted %lu\n", v, i);
        assert(v == i);
    }

    //  @end

    printf ("OK\n");
}
