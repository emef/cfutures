# cfutures
Promise / Future library in c

###Installation
```sh
git clone https://github.com/emef/cfutures
./autogen.sh
./configure
make
sudo make install
```

### Future executor pool example usage (from selftests)
```c
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
```

### Promise/future standalone example usage (from self tests)
```c
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
```
