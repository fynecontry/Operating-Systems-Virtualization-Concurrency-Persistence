# Summary

In multi-threaded programs, it is **often useful for a thread to wait for some condition to become true before proceeding**. The simple approach, of just spinning until the condition becomes true, is grossly inefficient and wastes CPU cycles, and in some cases, can be incorrect. Thus, how should a thread wait for a condition?

The answer to wait for a condition to become true, a thread can make use of what is known as a condition variable. **A condition variable** is an explicit queue that threads can put themselves on when some state of execution, i.e., some condition, which is not as desired by waiting on the condition. Some other thread, when it changes said state, can then wake one or more of those waiting threads and thus allow them to continue by signaling on the condition.



    TIP: ALWAYS HOLD THE LOCK WHILE SIGNALING

    Although it is strictly not necessary in all cases, it is likely simplest and best to hold the lock 
    while signaling when using condition variables. 
    The example above shows a case where you must hold the lock for correctness; 
    however, there are some other cases where it is likely OK not to, 
    but probably is something you should avoid. Thus, for simplicity, hold the lock when calling signal.

    ```
    Pthread_mutex_lock(&m);
    done = 1;
    Pthread_cond_signal(&c);
    Pthread_mutex_unlock(&m);
    ```
    Example of Signalling with locks acquired

    The converse of this tip, i.e., hold the lock when calling wait, is not just a tip, 
    but rather mandated by the semantics of wait, because wait always 
    (a) assumes the lock is held when you call it, 
    (b) releases said lock when putting the caller to sleep, and (c) re-acquires the lock just before returning. 
    Thus, the generalization of this tip is correct: hold the lock when calling signal or wait, 
    and you will always be in good shape.

### Producer/Consumer (Bounded Buffer)
The producer-consumer problem, also known as the bounded buffer problem, is a classic synchronization problem in computer science. It describes the interaction between two processes, a producer and a consumer, that share a common buffer. The producer generates data and places it in the buffer, while the consumer takes data from the buffer and processes it.

The problem arises because the producer and consumer may try to access the buffer at the same time,  which can lead to data corruption or race conditions. Or the producer/consumer tries to access the buffer when it is empty or full.

To prevent this, the producer and consumer must be 
- Synchronized so that only one process can access the buffer at a time.
- Use two condition variables, instead of one, in order to properly signal which type of thread should wake up when the state of the system changes.
- Use while (not if) for conditions to [spurious wake ups](https://linux.die.net/man/3/pthread_cond_signal)
- Having large buffer slots with just a single producer/consumer is more efficient as it reduces context switches as it reduces context switches; with multiple producers or consumers (or both), it even allows concurrent producing or consuming to take place, thus increasing concurrency.
- In the case of multiple threads are waiting for a specific event or resource, and not knowing which thread to wake. We could use covering conditions such as replacing [pthread_cond_signal](https://linux.die.net/man/3/pthread_cond_signal) with [pthread_cond_broadcast](https://linux.die.net/man/3/pthread_cond_broadcast) which effectively wakes all thread waiting on that resource. (Albeit: with some performance hit).