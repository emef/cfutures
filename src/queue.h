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

#ifndef QUEUE_H_INCLUDED
#define QUEUE_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include "internal.h"

//  @interface
//  Create a new queue
CFUTURES_EXPORT queue_t *
    queue_new (void);

//  Destroy the queue
CFUTURES_EXPORT void
    queue_destroy (queue_t **self_p);

//  Enqueue an item.
CFUTURES_EXPORT void
    queue_enqueue (queue_t *self, void *value);

// Pop an item (removes it).
CFUTURES_EXPORT void *
    queue_pop (queue_t *self);

//  Self test of this class
CFUTURES_EXPORT void
    queue_test (bool verbose);
//  @end

#ifdef __cplusplus
}
#endif

#endif
