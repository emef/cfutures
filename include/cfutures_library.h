/*  =========================================================================
    cfutures - CFUTURES wrapper

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

################################################################################
#  THIS FILE IS 100% GENERATED BY ZPROJECT; DO NOT EDIT EXCEPT EXPERIMENTALLY  #
#  Please refer to the README for information about making permanent changes.  #
################################################################################
    =========================================================================
*/

#ifndef CFUTURES_LIBRARY_H_INCLUDED
#define CFUTURES_LIBRARY_H_INCLUDED

//  Set up environment for the application

//  External dependencies

//  CFUTURES version macros for compile-time API detection

#define CFUTURES_VERSION_MAJOR 0
#define CFUTURES_VERSION_MINOR 0
#define CFUTURES_VERSION_PATCH 0

#define CFUTURES_MAKE_VERSION(major, minor, patch) \
    ((major) * 10000 + (minor) * 100 + (patch))
#define CFUTURES_VERSION \
    CFUTURES_MAKE_VERSION(CFUTURES_VERSION_MAJOR, CFUTURES_VERSION_MINOR, CFUTURES_VERSION_PATCH)

#if defined (__WINDOWS__)
#   if defined LIBCFUTURES_STATIC
#       define CFUTURES_EXPORT
#   elif defined LIBCFUTURES_EXPORTS
#       define CFUTURES_EXPORT __declspec(dllexport)
#   else
#       define CFUTURES_EXPORT __declspec(dllimport)
#   endif
#else
#   define CFUTURES_EXPORT
#endif

//  Opaque class structures to allow forward references
typedef struct _future_t future_t;
#define FUTURE_T_DEFINED
typedef struct _future_pool_t future_pool_t;
#define FUTURE_POOL_T_DEFINED
typedef struct _promise_t promise_t;
#define PROMISE_T_DEFINED


//  Public API classes
#include "prelude.h"
#include "future.h"
#include "future_pool.h"
#include "promise.h"

#endif
/*
################################################################################
#  THIS FILE IS 100% GENERATED BY ZPROJECT; DO NOT EDIT EXCEPT EXPERIMENTALLY  #
#  Please refer to the README for information about making permanent changes.  #
################################################################################
*/
