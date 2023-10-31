# Summary

This lesson summarizes the chapter with some guidelines to use thread API.

You have been introduced the basics of the pthread library, including thread creation, building mutual exclusion via locks, and signaling and waiting via condition variables in this chapter. You don’t need much else to write robust and efficient multi-threaded code, except patience and a great deal of care!

Let’s end the chapter with a set of tips that might be useful to you when you write multi-threaded code (see the aside section below). There are other aspects of the API that are interesting; if you want more information, type man -k pthread on a Linux system to see over one hundred APIs that make up the entire interface. However, the basics discussed herein should enable you to build sophisticated (and hopefully, correct and performant) multi-threaded programs. The hard part with threads is not the APIs, but rather the tricky logic of how you build concurrent programs. Read on to learn more.


```
ASIDE: THREAD API GUIDELINES

There are a number of small but important things to remember when you use the POSIX thread library (or really, any thread library) to build a multi-threaded program. They are:

Keep it simple. Above all else, any code to lock or signal between threads should be as simple as possible. Tricky thread interactions lead to bugs.

Minimize thread interactions. Try to keep the number of ways in which threads interact to a minimum. Each interaction should be carefully thought out and constructed with tried and true approaches (many of which you will learn about in the coming chapters).

Initialize locks and condition variables. Failure to do so will lead to code that sometimes works and sometimes fails in very strange ways.

Check your return codes. Of course, in any C and UNIX programming you do, you should be checking each and every return code, and it’s true here as well. Failure to do so will lead to bizarre and hard to understand behavior, making you likely to (a) scream, (b) pull some of your hair out, or (c) both.

Be careful with how you pass arguments to, and return values from, threads. In particular, any time you are passing a reference to a variable allocated on the stack, you are probably doing something wrong.

Each thread has its own stack. As related to the point above, please remember that each thread has its own stack. Thus, if you have a locally-allocated variable inside of some function a thread is executing, it is essentially private to that thread; no other thread can (easily) access it. To share data between threads, the values must be in the heap or otherwise some locale that is globally accessible.

Use the manual pages. On Linux, in particular, the pthread man pages are highly informative and discuss much of the nuances presented here, often in even more detail. Read them carefully!
```

**NOTE** - Always use condition variables to signal between threads. While it is often tempting to use a simple flag, don’t do it. It performs poorly in many cases and error prone in most cases. [Ad Hoc Synchronization Considered Harmful](https://www.usenix.org/legacy/events/osdi10/tech/full_papers/Xiong.pdf)



## Condition Variables vs. Signals

### Conditional Variables:
  - **Use Case - Coordination and Synchronization:** Conditional variables are typically used when threads need to coordinate their activities and synchronize their execution based on certain conditions.
  - **Example Scenario - Producer-Consumer Problem:** In scenarios like the producer-consumer problem, where one thread produces data and places it in a buffer, and the other thread consumes it, a conditional variable wait until the buffer is not empty might be used to signal the consumer thread to start consuming when there is data available and signal the producer thread to produce when space is available.
  - **Functions:**
    - **pthread_cond_wait:** Atomically unlocks the mutex and waits for the condition variable to be signaled.
    - **pthread_cond_signal or pthread_cond_broadcast:** Signals one or all waiting threads that the condition has changed.

### Signals:
  - **Use Case - Interprocess Communication (IPC):** Signals are often used for interprocess communication, where processes, rather than threads, need to communicate with each other.
  - **Example Scenario - UNIX Signals:** In Unix-like systems, signals (e.g., `SIGINT`, `SIGTERM`) are used to notify a process that a specific event has occurred. For example, sending a SIGTERM signal to terminate a process.
  - **Functions:**
    - **kill:** Sends a signal to a process.
    - **signal (or sigaction):** Installs a signal handler to handle a particular signal.
    - **raise:** Generates a signal for the calling process.

### Choosing Between Them:
 - **Threads within a Process:** If you are dealing with threads within a process and need coordination, use conditional variables. They are designed for intra-process synchronization.
 - **Interprocess Communication:** If you are dealing with communication between different processes, you may need to use signals (or other IPC mechanisms like message queues or sockets) since conditional variables are typically limited to a single process.