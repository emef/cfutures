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

#ifndef FUTURE_H_INCLUDED
#define FUTURE_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

// Callback executed when a future is resolved. The future_t
// in the argument is guaranteed to be resolved.
typedef void (future_callback_fn) (future_t *future, void *args);

//  Destroy the future
CFUTURES_EXPORT void
    future_destroy (future_t **self_p);

//  Wait for the future to be resolved.
CFUTURES_EXPORT void
    future_wait (future_t *self);

//  Returns true if future has been resolved by either
//  being fulfilled or rejected (non-blocking)
CFUTURES_EXPORT bool
    future_is_resolved (future_t *self);

//  Returns true if future has been fulfilled. (non-blocking)
CFUTURES_EXPORT bool
    future_is_fulfilled (future_t *self);

//  Blocks for future resolution and returns the value. Asserts that
//  the future was not rejected. (blocking)
CFUTURES_EXPORT void *
    future_get (future_t *self);

//  Returns the future's rejection return code. (non-blocking)
CFUTURES_EXPORT int
    future_rejected_code (future_t *self);

//  Add a callback to be executed when this future completes.
CFUTURES_EXPORT void
    future_add_callback (
        future_t *self, future_callback_fn *callback_fn, void *args);

//  Self test of this class
CFUTURES_EXPORT void
    future_test (bool verbose);
//  @end

#ifdef __cplusplus
}
#endif

#endif
