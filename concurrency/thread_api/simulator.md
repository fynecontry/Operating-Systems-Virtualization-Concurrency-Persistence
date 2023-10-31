 # Simulator

In this section, we’ll write some simple multi-threaded programs and use a specific tool, called [helgrind](https://valgrind.org/docs/manual/hg-manual.html), to find problems in these programs.

## Questions

**1. First, build main-race.c by running make main-race. Examine the code so you can see the (hopefully obvious) data race in the code. Now run helgrind (by typing valgrind --tool=helgrind ./main-race) to see how it reports the race. Does it point to the right lines of code? What other information does it give to you?**
> Highlights critcal section with race conditions
```
# valgrind --tool=helgrind ./main-race
==51== Helgrind, a thread error detector
==51== Copyright (C) 2007-2015, and GNU GPL'd, by OpenWorks LLP et al.
==51== Using Valgrind-3.11.0 and LibVEX; rerun with -h for copyright info
==51== Command: ./main-race
==51== 
==51== ---Thread-Announcement------------------------------------------
==51== 
==51== Thread #1 is the program's root thread
==51== 
==51== ---Thread-Announcement------------------------------------------
==51== 
==51== Thread #2 was created
==51==    at 0x516449E: clone (clone.S:74)
==51==    by 0x4E46149: create_thread (createthread.c:102)
==51==    by 0x4E47E83: pthread_create@@GLIBC_2.2.5 (pthread_create.c:679)
==51==    by 0x4C34BB7: ??? (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==51==    by 0x400BAE: Pthread_create (mythreads.h:51)
==51==    by 0x400C72: main (main-race.c:14)
==51== 
==51== ----------------------------------------------------------------
==51== 
==51== Possible data race during read of size 4 at 0x602088 by thread #1
==51== Locks held: none
==51==    at 0x400C73: main (main-race.c:15)
==51== 
==51== This conflicts with a previous write of size 4 by thread #2
==51== Locks held: none
==51==    at 0x400C2D: worker (main-race.c:8)
==51==    by 0x4C34DB6: ??? (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==51==    by 0x4E476B9: start_thread (pthread_create.c:333)
==51==  Address 0x602088 is 0 bytes inside data symbol "balance"
==51== 
==51== ----------------------------------------------------------------
==51== 
==51== Possible data race during write of size 4 at 0x602088 by thread #1
==51== Locks held: none
==51==    at 0x400C7C: main (main-race.c:15)
==51== 
==51== This conflicts with a previous write of size 4 by thread #2
==51== Locks held: none
==51==    at 0x400C2D: worker (main-race.c:8)
==51==    by 0x4C34DB6: ??? (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==51==    by 0x4E476B9: start_thread (pthread_create.c:333)
==51==  Address 0x602088 is 0 bytes inside data symbol "balance"
==51== 
==51== 
==51== For counts of detected and suppressed errors, rerun with: -v
==51== Use --history-level=approx or =none to gain increased speed, at
==51== the cost of reduced accuracy of conflicting-access information
==51== ERROR SUMMARY: 2 errors from 2 contexts (suppressed: 0 from 0)
```

**2. What happens when you remove one of the offending lines of code? Now add a lock around one of the updates to the shared variable, and then around both. What does helgrind report in each of these cases?**
> // balance++ main (main-race.c:15)
```
# valgrind --tool=helgrind ./main-race
==67== Helgrind, a thread error detector
==67== Copyright (C) 2007-2015, and GNU GPL'd, by OpenWorks LLP et al.
==67== Using Valgrind-3.11.0 and LibVEX; rerun with -h for copyright info
==67== Command: ./main-race
==67== 
==67== 
==67== For counts of detected and suppressed errors, rerun with: -v
==67== Use --history-level=approx or =none to gain increased speed, at
==67== the cost of reduced accuracy of conflicting-access information
==67== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
```
> lock m1 acquired by thread 2 on routine worker (main-race.c:10)
```
# valgrind --tool=helgrind ./main-race
==76== Helgrind, a thread error detector
==76== Copyright (C) 2007-2015, and GNU GPL'd, by OpenWorks LLP et al.
==76== Using Valgrind-3.11.0 and LibVEX; rerun with -h for copyright info
==76== Command: ./main-race
==76== 
==76== ---Thread-Announcement------------------------------------------
==76== 
==76== Thread #1 is the program's root thread
==76== 
==76== ---Thread-Announcement------------------------------------------
==76== 
==76== Thread #2 was created
==76==    at 0x516449E: clone (clone.S:74)
==76==    by 0x4E46149: create_thread (createthread.c:102)
==76==    by 0x4E47E83: pthread_create@@GLIBC_2.2.5 (pthread_create.c:679)
==76==    by 0x4C34BB7: ??? (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==76==    by 0x400BAE: Pthread_create (mythreads.h:51)
==76==    by 0x400C8A: main (main-race.c:18)
==76== 
==76== ----------------------------------------------------------------
==76== 
==76==  Lock at 0x6020C0 was first observed
==76==    at 0x4C321BC: ??? (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==76==    by 0x400A4F: Pthread_mutex_lock (mythreads.h:23)
==76==    by 0x400C31: worker (main-race.c:10)
==76==    by 0x4C34DB6: ??? (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==76==    by 0x4E476B9: start_thread (pthread_create.c:333)
==76==  Address 0x6020c0 is 0 bytes inside data symbol "m1"
==76== 
==76== Possible data race during read of size 4 at 0x602088 by thread #1
==76== Locks held: none
==76==    at 0x400C8B: main (main-race.c:19)
==76== 
==76== This conflicts with a previous write of size 4 by thread #2
==76== Locks held: 1, at address 0x6020C0
==76==    at 0x400C3B: worker (main-race.c:11)
==76==    by 0x4C34DB6: ??? (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==76==    by 0x4E476B9: start_thread (pthread_create.c:333)
==76==  Address 0x602088 is 0 bytes inside data symbol "balance"
==76== 
==76== ----------------------------------------------------------------
==76== 
==76==  Lock at 0x6020C0 was first observed
==76==    at 0x4C321BC: ??? (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==76==    by 0x400A4F: Pthread_mutex_lock (mythreads.h:23)
==76==    by 0x400C31: worker (main-race.c:10)
==76==    by 0x4C34DB6: ??? (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==76==    by 0x4E476B9: start_thread (pthread_create.c:333)
==76==  Address 0x6020c0 is 0 bytes inside data symbol "m1"
==76== 
==76== Possible data race during write of size 4 at 0x602088 by thread #1
==76== Locks held: none
==76==    at 0x400C94: main (main-race.c:19)
==76== 
==76== This conflicts with a previous write of size 4 by thread #2
==76== Locks held: 1, at address 0x6020C0
==76==    at 0x400C3B: worker (main-race.c:11)
==76==    by 0x4C34DB6: ??? (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==76==    by 0x4E476B9: start_thread (pthread_create.c:333)
==76==  Address 0x602088 is 0 bytes inside data symbol "balance"
==76== 
==76== 
==76== For counts of detected and suppressed errors, rerun with: -v
==76== Use --history-level=approx or =none to gain increased speed, at
==76== the cost of reduced accuracy of conflicting-access information
==76== ERROR SUMMARY: 2 errors from 2 contexts (suppressed: 1 from 1)
```
> Add lock m1 on both threads before updating shared variable prevents race condition
```
# valgrind --tool=helgrind ./main-race
==52== Helgrind, a thread error detector
==52== Copyright (C) 2007-2015, and GNU GPL'd, by OpenWorks LLP et al.
==52== Using Valgrind-3.11.0 and LibVEX; rerun with -h for copyright info
==52== Command: ./main-race
==52== 
==52== 
==52== For counts of detected and suppressed errors, rerun with: -v
==52== Use --history-level=approx or =none to gain increased speed, at
==52== the cost of reduced accuracy of conflicting-access information
==52== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 7 from 7)
```

**3. Now let’s look at `main-deadlock.c`. Examine the code. This code has a problem known as deadlock (which we discuss in much more depth in a forthcoming chapter). Can you see what problem it might have?**
> If threads p1 & p2 acquire locks m1 & m2 respectively at the same time both thread will be on each other to release their locks causing a deadlock

**4. Now run helgrind on this code. What does helgrind report?**
```
# valgrind --tool=helgrind ./main-deadlock==52== Helgrind, a thread error detector
==52== Copyright (C) 2007-2015, and GNU GPL'd, by OpenWorks LLP et al.
==52== Using Valgrind-3.11.0 and LibVEX; rerun with -h for copyright info
==52== Command: ./main-deadlock
==52== 
==52== ---Thread-Announcement------------------------------------------
==52== 
==52== Thread #3 was created
==52==    at 0x516449E: clone (clone.S:74)
==52==    by 0x4E46149: create_thread (createthread.c:102)
==52==    by 0x4E47E83: pthread_create@@GLIBC_2.2.5 (pthread_create.c:679)
==52==    by 0x4C34BB7: ??? (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==52==    by 0x400BAE: Pthread_create (mythreads.h:51)
==52==    by 0x400CC9: main (main-deadlock.c:24)
==52== 
==52== ----------------------------------------------------------------
==52== 
==52== Thread #3: lock order "0x6020C0 before 0x602100" violated
==52== 
==52== Observed (incorrect) order is: acquisition of lock at 0x602100
==52==    at 0x4C321BC: ??? (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==52==    by 0x400A4F: Pthread_mutex_lock (mythreads.h:23)
==52==    by 0x400C50: worker (main-deadlock.c:13)
==52==    by 0x4C34DB6: ??? (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==52==    by 0x4E476B9: start_thread (pthread_create.c:333)
==52== 
==52==  followed by a later acquisition of lock at 0x6020C0
==52==    at 0x4C321BC: ??? (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==52==    by 0x400A4F: Pthread_mutex_lock (mythreads.h:23)
==52==    by 0x400C5A: worker (main-deadlock.c:14)
==52==    by 0x4C34DB6: ??? (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==52==    by 0x4E476B9: start_thread (pthread_create.c:333)
==52== 
==52== Required order was established by acquisition of lock at 0x6020C0
==52==    at 0x4C321BC: ??? (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==52==    by 0x400A4F: Pthread_mutex_lock (mythreads.h:23)
==52==    by 0x400C3A: worker (main-deadlock.c:10)
==52==    by 0x4C34DB6: ??? (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==52==    by 0x4E476B9: start_thread (pthread_create.c:333)
==52== 
==52==  followed by a later acquisition of lock at 0x602100
==52==    at 0x4C321BC: ??? (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==52==    by 0x400A4F: Pthread_mutex_lock (mythreads.h:23)
==52==    by 0x400C44: worker (main-deadlock.c:11)
==52==    by 0x4C34DB6: ??? (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==52==    by 0x4E476B9: start_thread (pthread_create.c:333)
==52== 
==52==  Lock at 0x6020C0 was first observed
==52==    at 0x4C321BC: ??? (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==52==    by 0x400A4F: Pthread_mutex_lock (mythreads.h:23)
==52==    by 0x400C3A: worker (main-deadlock.c:10)
==52==    by 0x4C34DB6: ??? (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==52==    by 0x4E476B9: start_thread (pthread_create.c:333)
==52==  Address 0x6020c0 is 0 bytes inside data symbol "m1"
==52== 
==52==  Lock at 0x602100 was first observed
==52==    at 0x4C321BC: ??? (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==52==    by 0x400A4F: Pthread_mutex_lock (mythreads.h:23)
==52==    by 0x400C44: worker (main-deadlock.c:11)
==52==    by 0x4C34DB6: ??? (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==52==    by 0x4E476B9: start_thread (pthread_create.c:333)
==52==  Address 0x602100 is 0 bytes inside data symbol "m2"
==52== 
==52== 
==52== 
==52== For counts of detected and suppressed errors, rerun with: -v
==52== Use --history-level=approx or =none to gain increased speed, at
==52== the cost of reduced accuracy of conflicting-access information
==52== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 9 from 9)
```

**5. Now run helgrind on main-deadlock-global.c. Examine the code; does it have the same problem that main-deadlock.c has? Should helgrind be reporting the same error? What does this tell you about tools like helgrind?**
> Interestingly helgrind detects deadlock but **I think this might be a bug??** as [global mutex g](https://github.com/fynecontry/Operating-Systems-Virtualization-Concurrency-Persistence/blob/4a8427982971e131fd5a0dcd25810cc7f05584ce/concurrency/thread_api/main-deadlock-global.c#L10) is meant to prevent this no? hmmm.
```
# valgrind --tool=helgrind ./main-deadlock-global
==104== Helgrind, a thread error detector
==104== Copyright (C) 2007-2015, and GNU GPL'd, by OpenWorks LLP et al.
==104== Using Valgrind-3.11.0 and LibVEX; rerun with -h for copyright info
==104== Command: ./main-deadlock-global
==104== 
==104== ---Thread-Announcement------------------------------------------
==104== 
==104== Thread #3 was created
==104==    at 0x516449E: clone (clone.S:74)
==104==    by 0x4E46149: create_thread (createthread.c:102)
==104==    by 0x4E47E83: pthread_create@@GLIBC_2.2.5 (pthread_create.c:679)
==104==    by 0x4C34BB7: ??? (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==104==    by 0x400BAE: Pthread_create (mythreads.h:51)
==104==    by 0x400CDD: main (main-deadlock-global.c:27)
==104== 
==104== ----------------------------------------------------------------
==104== 
==104== Thread #3: lock order "0x602100 before 0x602140" violated
==104== 
==104== Observed (incorrect) order is: acquisition of lock at 0x602140
==104==    at 0x4C321BC: ??? (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==104==    by 0x400A4F: Pthread_mutex_lock (mythreads.h:23)
==104==    by 0x400C5A: worker (main-deadlock-global.c:15)
==104==    by 0x4C34DB6: ??? (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==104==    by 0x4E476B9: start_thread (pthread_create.c:333)
==104== 
==104==  followed by a later acquisition of lock at 0x602100
==104==    at 0x4C321BC: ??? (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==104==    by 0x400A4F: Pthread_mutex_lock (mythreads.h:23)
==104==    by 0x400C64: worker (main-deadlock-global.c:16)
==104==    by 0x4C34DB6: ??? (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==104==    by 0x4E476B9: start_thread (pthread_create.c:333)
==104== 
==104== Required order was established by acquisition of lock at 0x602100
==104==    at 0x4C321BC: ??? (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==104==    by 0x400A4F: Pthread_mutex_lock (mythreads.h:23)
==104==    by 0x400C44: worker (main-deadlock-global.c:12)
==104==    by 0x4C34DB6: ??? (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==104==    by 0x4E476B9: start_thread (pthread_create.c:333)
==104== 
==104==  followed by a later acquisition of lock at 0x602140
==104==    at 0x4C321BC: ??? (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==104==    by 0x400A4F: Pthread_mutex_lock (mythreads.h:23)
==104==    by 0x400C4E: worker (main-deadlock-global.c:13)
==104==    by 0x4C34DB6: ??? (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==104==    by 0x4E476B9: start_thread (pthread_create.c:333)
==104== 
==104==  Lock at 0x602100 was first observed
==104==    at 0x4C321BC: ??? (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==104==    by 0x400A4F: Pthread_mutex_lock (mythreads.h:23)
==104==    by 0x400C44: worker (main-deadlock-global.c:12)
==104==    by 0x4C34DB6: ??? (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==104==    by 0x4E476B9: start_thread (pthread_create.c:333)
==104==  Address 0x602100 is 0 bytes inside data symbol "m1"
==104== 
==104==  Lock at 0x602140 was first observed
==104==    at 0x4C321BC: ??? (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==104==    by 0x400A4F: Pthread_mutex_lock (mythreads.h:23)
==104==    by 0x400C4E: worker (main-deadlock-global.c:13)
==104==    by 0x4C34DB6: ??? (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==104==    by 0x4E476B9: start_thread (pthread_create.c:333)
==104==  Address 0x602140 is 0 bytes inside data symbol "m2"
==104== 
==104== 
==104== 
==104== For counts of detected and suppressed errors, rerun with: -v
==104== Use --history-level=approx or =none to gain increased speed, at
==104== the cost of reduced accuracy of conflicting-access information
==104== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 9 from 9)
```

**6. Let’s next look at main-signal.c. This code uses a variable (done) to signal that the child is done and that the parent can now continue. Why is this code inefficient? (what does the parent end up spending its time doing, particularly if the child thread takes a long time to complete?)**
> The code is inefficient because it spends/wastes CPU cyles waiting on child thread to complete this is known as spin waiting or busy waiting.

**7. Now run helgrind on this program. What does it report? Is the code correct?**
```
# valgrind --tool=helgrind ./main-signal
==59== Helgrind, a thread error detector
==59== Copyright (C) 2007-2015, and GNU GPL'd, by OpenWorks LLP et al.
==59== Using Valgrind-3.11.0 and LibVEX; rerun with -h for copyright info
==59== Command: ./main-signal
==59== 
this should print first
==59== ---Thread-Announcement------------------------------------------
==59== 
==59== Thread #2 was created
==59==    at 0x516449E: clone (clone.S:74)
==59==    by 0x4E46149: create_thread (createthread.c:102)
==59==    by 0x4E47E83: pthread_create@@GLIBC_2.2.5 (pthread_create.c:679)
==59==    by 0x4C34BB7: ??? (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==59==    by 0x400BFE: Pthread_create (mythreads.h:51)
==59==    by 0x400CCB: main (main-signal.c:15)
==59== 
==59== ---Thread-Announcement------------------------------------------
==59== 
==59== Thread #1 is the program's root thread
==59== 
==59== ----------------------------------------------------------------
==59== 
==59== Possible data race during write of size 4 at 0x602094 by thread #2
==59== Locks held: none
==59==    at 0x400C82: worker (main-signal.c:9)
==59==    by 0x4C34DB6: ??? (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==59==    by 0x4E476B9: start_thread (pthread_create.c:333)
==59== 
==59== This conflicts with a previous read of size 4 by thread #1
==59== Locks held: none
==59==    at 0x400CCD: main (main-signal.c:16)
==59==  Address 0x602094 is 0 bytes inside data symbol "done"
==59== 
==59== ----------------------------------------------------------------
==59== 
==59== Possible data race during read of size 4 at 0x602094 by thread #1
==59== Locks held: none
==59==    at 0x400CCD: main (main-signal.c:16)
==59== 
==59== This conflicts with a previous write of size 4 by thread #2
==59== Locks held: none
==59==    at 0x400C82: worker (main-signal.c:9)
==59==    by 0x4C34DB6: ??? (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==59==    by 0x4E476B9: start_thread (pthread_create.c:333)
==59==  Address 0x602094 is 0 bytes inside data symbol "done"
==59== 
this should print last
==59== 
==59== For counts of detected and suppressed errors, rerun with: -v
==59== Use --history-level=approx or =none to gain increased speed, at
==59== the cost of reduced accuracy of conflicting-access information
==59== ERROR SUMMARY: 2 errors from 2 contexts (suppressed: 67 from 44)
```

**8. Now, look at a slightly modified version of the code, which is found in main-signal-cv.c. This version uses a condition variable to do the signaling (and associated lock). Why is this code preferred to the previous version? Is it correctness, or performance, or both?**
> Both. Correctness as it the synchronization between threads acquiring the lock and the cond. variable signalling prevents race conditions. Performance because the waiting thread releases the lock and goes to sleep which reduces CPU usage and prevents the busy waiting we saw in last approach.

**9. Once again run helgrind on main-signal-cv. Does it report any errors?**
```
# valgrind --tool=helgrind ./main-signal-cv
==58== Helgrind, a thread error detector
==58== Copyright (C) 2007-2015, and GNU GPL'd, by OpenWorks LLP et al.
==58== Using Valgrind-3.11.0 and LibVEX; rerun with -h for copyright info
==58== Command: ./main-signal-cv
==58== 
this should print first
this should print last
==58== 
==58== For counts of detected and suppressed errors, rerun with: -v
==58== Use --history-level=approx or =none to gain increased speed, at
==58== the cost of reduced accuracy of conflicting-access information
==58== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 20 from 19)
```