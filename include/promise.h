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

#ifndef PROMISE_H_INCLUDED
#define PROMISE_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif


//  @interface
//  Create a new promise
CFUTURES_EXPORT promise_t *
    promise_new (void);

//  Destroy the promise
CFUTURES_EXPORT void
    promise_destroy (promise_t **self_p);

// Get the promise's owned future.
CFUTURES_EXPORT future_t *
    promise_get_future (promise_t *self);

// Fulfill the promise with a value.
CFUTURES_EXPORT void
    promise_fulfill (promise_t *self, void *value);

// Reject the promise with a return code.
CFUTURES_EXPORT void
    promise_reject (promise_t *self, int rc);

//  Self test of this class
CFUTURES_EXPORT void
    promise_test (bool verbose);
//  @end

#ifdef __cplusplus
}
#endif

#endif
