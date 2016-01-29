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

#ifndef FUTURE_POOL_H_INCLUDED
#define FUTURE_POOL_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

typedef void * (future_fn) (void *args);

//  @interface
//  Create a new future_pool
CFUTURES_EXPORT future_pool_t *
    future_pool_new (int n_threads);

//  Destroy the future_pool
CFUTURES_EXPORT void
    future_pool_destroy (future_pool_t **self_p);

CFUTURES_EXPORT future_t *
    future_pool_execute (future_pool_t *self, future_fn *fn, void *args);

//  Self test of this class
CFUTURES_EXPORT void
    future_pool_test (bool verbose);
//  @end

#ifdef __cplusplus
}
#endif

#endif
