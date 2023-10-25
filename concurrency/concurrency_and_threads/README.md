# Summary

The context switch between threads is quite similar to the context switch between processes, as the register state of T1 must be saved and the register state of T2 restored before running T2. With processes, you save state to a process control block (PCB); now, you’ll need one or more thread control blocks (TCBs) to store the state of each thread of a process.

1. In the context switch performed between threads as compared to processes: **the address space remains the same** (i.e., there is no need to switch which page table we are using).

2. Concerns the stack - In our simple model of the address space of a classic process (which you can now call a single-threaded process), there is a single stack, usually residing at the bottom of the address space (See left of the below figure).
![image](https://github.com/fynecontry/Operating-Systems-Virtualization-Concurrency-Persistence/assets/27024731/37363174-6d7a-4a4a-8017-be1714f49be2)

3. Sleeping/waking a thread - where one thread must wait for another to complete some action before it continues. This interaction arises, for example, when a process performs a disk I/O and is put to sleep; when the I/O completes, the process needs to be roused from its slumber so it can continue.

Instead of a single stack in the address space, there will be one per thread.

In this figure, you can see two stacks spread throughout the address space of the process. Thus, any stack-allocated variables, parameters, return values, and other things that we put on the stack will be placed in what is sometimes called **thread-local storage**, i.e., the stack of the relevant thread.

## Why use threads
1. **Parallelism** - The task of transforming your standard single-threaded program into a program that does this sort of work on multiple CPUs is called **parallelization**, and using a thread per CPU to do this work is a natural and typical way to make programs run faster on modern hardware.

2. **To avoid blocking** - Using threads is a natural way to avoid getting stuck; while one thread in your program waits (i.e., is blocked waiting for I/O), the CPU scheduler can switch to other threads, which are ready to run and do something useful. Threading enables overlap of I/O with other activities within a single program, much like multiprogramming did for processes across programs;


> ASIDE: KEY CONCURRENCY TERMS CRITICAL SECTION, RACE CONDITION, INDETERMINATE, MUTUAL EXCLUSION
> These four terms are so central to concurrent code that it is worthwhile to call them out explicitly. See some of Dijkstra’s early work for more details.
>
> A critical section is a piece of code that accesses a shared resource, usually a variable or data structure.
>
> A race condition (or data race [NM92]) arises if multiple threads of execution enter the critical section at roughly the same time; both attempt to update the shared data structure, leading to a surprising (and perhaps undesirable) outcome.
>
> An indeterminate program consists of one or more race conditions; the output of the program varies from run to run, depending on which threads ran when. The outcome is thus not deterministic, something we usually expect from computer systems.
>
> To avoid these problems, threads should use some kind of mutual exclusion primitives; doing so guarantees that only a single thread ever enters a critical section, thus avoiding races, and resulting in deterministic program outputs.


## Common synchronization primitives:
1. **Mutex (Mutual Exclusion)**:
   - Purpose: Ensures that only one thread or process can access a critical section of code at a time.
   - Operation: A thread acquires the mutex before entering the critical section and releases it when done. Other threads must wait if the mutex is already held.
2. **Semaphore**:
   - Purpose: Controls access to a shared resource with a fixed number of slots or permits.
   - Operation: A semaphore maintains a count, and threads or processes acquire and release permits. It can be used to limit the number of concurrent accesses to a resource.
3. **Condition Variable**:
   - Purpose: Enables threads to wait for a particular condition to become true.
   - Operation: Threads can wait on a condition variable until another thread signals or broadcasts that the condition is met. This is often used in combination with a mutex.
4. **Barrier**:
   - Purpose: Synchronizes a group of threads by forcing them to wait until all of them have reached a certain point in their execution.
   - Operation: Threads increment a counter when they reach the barrier and wait if the required number of threads haven't reached the barrier yet.
5. **Read-Write Lock**:
   - Purpose: Differentiates between read-only and write access to a resource, allowing multiple threads to read simultaneously but only one to write.
   - Operation: Multiple threads can acquire the read lock simultaneously, but the write lock is exclusive.
6. **Atomic Operations**:
   - Purpose: Provides atomicity for specific operations without the need for explicit locks.
   - Operation: Operations like compare-and-swap (CAS) or test-and-set are executed atomically, making them useful for low-level synchronization without full locks.
7. **Event or Signal**:
   - Purpose: Allows one thread to signal an event to notify other waiting threads.
   - Operation: One thread signals the event, and waiting threads are awakened.
8. **Monitor**:
   - Purpose: Combines a mutex, condition variables, and other synchronization tools into a higher-level abstraction.
   - Operation: Provides a way to ensure that only one thread can execute a specific section of code at a time and allows other threads to wait for certain conditions.
9. **Futex (Fast User-space Mutex)**:
   - Purpose: Optimized mutex implementation for user-space synchronization.
   - Operation: Allows for efficient user-space synchronization and can fall back to kernel-level operations when necessary.