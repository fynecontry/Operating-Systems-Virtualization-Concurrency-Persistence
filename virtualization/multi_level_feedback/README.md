# Summary
We have described a scheduling approach known as the **Multi-Level Feedback Queue (MLFQ)**. Hopefully, you can now see why it is called that: it has multiple levels of queues, and uses feedback to determine the priority of a given job. History is its guide: pay attention to how jobs behave over time and treat them accordingly.

The refined set of MLFQ rules, spread throughout the chapter, are reproduced here for your viewing pleasure:
```
    Rule 1: If Priority(A)>Priority(B)Priority(A)>Priority(B), AA runs (BB doesn’t).

    Rule 2: If Priority(A)=Priority(B)Priority(A)=Priority(B), AA & BB run in round-robin fashion using the time slice (quantum length) of the given queue.

    Rule 3: When a job enters the system, it is placed at the highest priority (the topmost queue).

    Rule 4: Once a job uses up its time allotment at a given level (regardless of how many times it has given up the CPU), its priority is reduced (i.e., it moves down one queue).

    Rule 5: After some time period S, move all the jobs in the system to the topmost queue.
```
MLFQ is interesting for the following reason: instead of demanding a priori knowledge of the nature of a job, it observes the execution of a job and prioritizes it accordingly. In this way, it manages to achieve the best of both worlds: it can deliver excellent overall performance (similar to SJF/STCF) for short-running interactive jobs, and is fair and makes progress for long-running CPU-intensive workloads. For this reason, many systems, including BSD UNIX derivatives, Solaris, and Windows NT and subsequent Windows operating systems use a form of MLFQ as their base scheduler.



## Simulator
Using the provided [mlfq.py](https://github.com/fynecontry/Operating-Systems-Virtualization-Concurrency-Persistence/blob/main/virtualization/multi_level_feedback/mlfq.py) answer the following questions.

### 1. Run a few randomly-generated problems with just two jobs and two queues; compute the MLFQ execution trace for each. Make your life easier by limiting the length of each job and turning off I/Os.
```
# ./mlfq.py -j 2 -n 2 --jlist=0,57,0:30,62,0 -c
Here is the list of inputs:
OPTIONS jobs 2
OPTIONS queues 2
OPTIONS allotments for queue  1 is   1
OPTIONS quantum length for queue  1 is  10
OPTIONS allotments for queue  0 is   1
OPTIONS quantum length for queue  0 is  10
OPTIONS boost 0
OPTIONS ioTime 5
OPTIONS stayAfterIO False
OPTIONS iobump False


For each job, three defining characteristics are given:
  startTime : at what time does the job enter the system
  runTime   : the total CPU time needed by the job to finish
  ioFreq    : every ioFreq time units, the job issues an I/O
              (the I/O takes ioTime units to complete)

Job List:
  Job  0: startTime   0 - runTime  57 - ioFreq   0
  Job  1: startTime  30 - runTime  62 - ioFreq   0


Execution Trace:

[ time 0 ] JOB BEGINS by JOB 0
[ time 0 ] Run JOB 0 at PRIORITY 1 [ TICKS 9 ALLOT 1 TIME 56 (of 57) ]
[ time 1 ] Run JOB 0 at PRIORITY 1 [ TICKS 8 ALLOT 1 TIME 55 (of 57) ]
[ time 2 ] Run JOB 0 at PRIORITY 1 [ TICKS 7 ALLOT 1 TIME 54 (of 57) ]
[ time 3 ] Run JOB 0 at PRIORITY 1 [ TICKS 6 ALLOT 1 TIME 53 (of 57) ]
[ time 4 ] Run JOB 0 at PRIORITY 1 [ TICKS 5 ALLOT 1 TIME 52 (of 57) ]
[ time 5 ] Run JOB 0 at PRIORITY 1 [ TICKS 4 ALLOT 1 TIME 51 (of 57) ]
[ time 6 ] Run JOB 0 at PRIORITY 1 [ TICKS 3 ALLOT 1 TIME 50 (of 57) ]
[ time 7 ] Run JOB 0 at PRIORITY 1 [ TICKS 2 ALLOT 1 TIME 49 (of 57) ]
[ time 8 ] Run JOB 0 at PRIORITY 1 [ TICKS 1 ALLOT 1 TIME 48 (of 57) ]
[ time 9 ] Run JOB 0 at PRIORITY 1 [ TICKS 0 ALLOT 1 TIME 47 (of 57) ]
[ time 10 ] Run JOB 0 at PRIORITY 0 [ TICKS 9 ALLOT 1 TIME 46 (of 57) ]
[ time 11 ] Run JOB 0 at PRIORITY 0 [ TICKS 8 ALLOT 1 TIME 45 (of 57) ]
[ time 12 ] Run JOB 0 at PRIORITY 0 [ TICKS 7 ALLOT 1 TIME 44 (of 57) ]
[ time 13 ] Run JOB 0 at PRIORITY 0 [ TICKS 6 ALLOT 1 TIME 43 (of 57) ]
[ time 14 ] Run JOB 0 at PRIORITY 0 [ TICKS 5 ALLOT 1 TIME 42 (of 57) ]
[ time 15 ] Run JOB 0 at PRIORITY 0 [ TICKS 4 ALLOT 1 TIME 41 (of 57) ]
[ time 16 ] Run JOB 0 at PRIORITY 0 [ TICKS 3 ALLOT 1 TIME 40 (of 57) ]
[ time 17 ] Run JOB 0 at PRIORITY 0 [ TICKS 2 ALLOT 1 TIME 39 (of 57) ]
[ time 18 ] Run JOB 0 at PRIORITY 0 [ TICKS 1 ALLOT 1 TIME 38 (of 57) ]
[ time 19 ] Run JOB 0 at PRIORITY 0 [ TICKS 0 ALLOT 1 TIME 37 (of 57) ]
[ time 20 ] Run JOB 0 at PRIORITY 0 [ TICKS 9 ALLOT 1 TIME 36 (of 57) ]
[ time 21 ] Run JOB 0 at PRIORITY 0 [ TICKS 8 ALLOT 1 TIME 35 (of 57) ]
[ time 22 ] Run JOB 0 at PRIORITY 0 [ TICKS 7 ALLOT 1 TIME 34 (of 57) ]
[ time 23 ] Run JOB 0 at PRIORITY 0 [ TICKS 6 ALLOT 1 TIME 33 (of 57) ]
[ time 24 ] Run JOB 0 at PRIORITY 0 [ TICKS 5 ALLOT 1 TIME 32 (of 57) ]
[ time 25 ] Run JOB 0 at PRIORITY 0 [ TICKS 4 ALLOT 1 TIME 31 (of 57) ]
[ time 26 ] Run JOB 0 at PRIORITY 0 [ TICKS 3 ALLOT 1 TIME 30 (of 57) ]
[ time 27 ] Run JOB 0 at PRIORITY 0 [ TICKS 2 ALLOT 1 TIME 29 (of 57) ]
[ time 28 ] Run JOB 0 at PRIORITY 0 [ TICKS 1 ALLOT 1 TIME 28 (of 57) ]
[ time 29 ] Run JOB 0 at PRIORITY 0 [ TICKS 0 ALLOT 1 TIME 27 (of 57) ]
[ time 30 ] JOB BEGINS by JOB 1
[ time 30 ] Run JOB 1 at PRIORITY 1 [ TICKS 9 ALLOT 1 TIME 61 (of 62) ]
[ time 31 ] Run JOB 1 at PRIORITY 1 [ TICKS 8 ALLOT 1 TIME 60 (of 62) ]
[ time 32 ] Run JOB 1 at PRIORITY 1 [ TICKS 7 ALLOT 1 TIME 59 (of 62) ]
[ time 33 ] Run JOB 1 at PRIORITY 1 [ TICKS 6 ALLOT 1 TIME 58 (of 62) ]
[ time 34 ] Run JOB 1 at PRIORITY 1 [ TICKS 5 ALLOT 1 TIME 57 (of 62) ]
[ time 35 ] Run JOB 1 at PRIORITY 1 [ TICKS 4 ALLOT 1 TIME 56 (of 62) ]
[ time 36 ] Run JOB 1 at PRIORITY 1 [ TICKS 3 ALLOT 1 TIME 55 (of 62) ]
[ time 37 ] Run JOB 1 at PRIORITY 1 [ TICKS 2 ALLOT 1 TIME 54 (of 62) ]
[ time 38 ] Run JOB 1 at PRIORITY 1 [ TICKS 1 ALLOT 1 TIME 53 (of 62) ]
[ time 39 ] Run JOB 1 at PRIORITY 1 [ TICKS 0 ALLOT 1 TIME 52 (of 62) ]
[ time 40 ] Run JOB 0 at PRIORITY 0 [ TICKS 9 ALLOT 1 TIME 26 (of 57) ]
[ time 41 ] Run JOB 0 at PRIORITY 0 [ TICKS 8 ALLOT 1 TIME 25 (of 57) ]
[ time 42 ] Run JOB 0 at PRIORITY 0 [ TICKS 7 ALLOT 1 TIME 24 (of 57) ]
[ time 43 ] Run JOB 0 at PRIORITY 0 [ TICKS 6 ALLOT 1 TIME 23 (of 57) ]
[ time 44 ] Run JOB 0 at PRIORITY 0 [ TICKS 5 ALLOT 1 TIME 22 (of 57) ]
[ time 45 ] Run JOB 0 at PRIORITY 0 [ TICKS 4 ALLOT 1 TIME 21 (of 57) ]
[ time 46 ] Run JOB 0 at PRIORITY 0 [ TICKS 3 ALLOT 1 TIME 20 (of 57) ]
[ time 47 ] Run JOB 0 at PRIORITY 0 [ TICKS 2 ALLOT 1 TIME 19 (of 57) ]
[ time 48 ] Run JOB 0 at PRIORITY 0 [ TICKS 1 ALLOT 1 TIME 18 (of 57) ]
[ time 49 ] Run JOB 0 at PRIORITY 0 [ TICKS 0 ALLOT 1 TIME 17 (of 57) ]
[ time 50 ] Run JOB 1 at PRIORITY 0 [ TICKS 9 ALLOT 1 TIME 51 (of 62) ]
[ time 51 ] Run JOB 1 at PRIORITY 0 [ TICKS 8 ALLOT 1 TIME 50 (of 62) ]
[ time 52 ] Run JOB 1 at PRIORITY 0 [ TICKS 7 ALLOT 1 TIME 49 (of 62) ]
[ time 53 ] Run JOB 1 at PRIORITY 0 [ TICKS 6 ALLOT 1 TIME 48 (of 62) ]
[ time 54 ] Run JOB 1 at PRIORITY 0 [ TICKS 5 ALLOT 1 TIME 47 (of 62) ]
[ time 55 ] Run JOB 1 at PRIORITY 0 [ TICKS 4 ALLOT 1 TIME 46 (of 62) ]
[ time 56 ] Run JOB 1 at PRIORITY 0 [ TICKS 3 ALLOT 1 TIME 45 (of 62) ]
[ time 57 ] Run JOB 1 at PRIORITY 0 [ TICKS 2 ALLOT 1 TIME 44 (of 62) ]
[ time 58 ] Run JOB 1 at PRIORITY 0 [ TICKS 1 ALLOT 1 TIME 43 (of 62) ]
[ time 59 ] Run JOB 1 at PRIORITY 0 [ TICKS 0 ALLOT 1 TIME 42 (of 62) ]
[ time 60 ] Run JOB 0 at PRIORITY 0 [ TICKS 9 ALLOT 1 TIME 16 (of 57) ]
[ time 61 ] Run JOB 0 at PRIORITY 0 [ TICKS 8 ALLOT 1 TIME 15 (of 57) ]
[ time 62 ] Run JOB 0 at PRIORITY 0 [ TICKS 7 ALLOT 1 TIME 14 (of 57) ]
[ time 63 ] Run JOB 0 at PRIORITY 0 [ TICKS 6 ALLOT 1 TIME 13 (of 57) ]
[ time 64 ] Run JOB 0 at PRIORITY 0 [ TICKS 5 ALLOT 1 TIME 12 (of 57) ]
[ time 65 ] Run JOB 0 at PRIORITY 0 [ TICKS 4 ALLOT 1 TIME 11 (of 57) ]
[ time 66 ] Run JOB 0 at PRIORITY 0 [ TICKS 3 ALLOT 1 TIME 10 (of 57) ]
[ time 67 ] Run JOB 0 at PRIORITY 0 [ TICKS 2 ALLOT 1 TIME 9 (of 57) ]
[ time 68 ] Run JOB 0 at PRIORITY 0 [ TICKS 1 ALLOT 1 TIME 8 (of 57) ]
[ time 69 ] Run JOB 0 at PRIORITY 0 [ TICKS 0 ALLOT 1 TIME 7 (of 57) ]
[ time 70 ] Run JOB 1 at PRIORITY 0 [ TICKS 9 ALLOT 1 TIME 41 (of 62) ]
[ time 71 ] Run JOB 1 at PRIORITY 0 [ TICKS 8 ALLOT 1 TIME 40 (of 62) ]
[ time 72 ] Run JOB 1 at PRIORITY 0 [ TICKS 7 ALLOT 1 TIME 39 (of 62) ]
[ time 73 ] Run JOB 1 at PRIORITY 0 [ TICKS 6 ALLOT 1 TIME 38 (of 62) ]
[ time 74 ] Run JOB 1 at PRIORITY 0 [ TICKS 5 ALLOT 1 TIME 37 (of 62) ]
[ time 75 ] Run JOB 1 at PRIORITY 0 [ TICKS 4 ALLOT 1 TIME 36 (of 62) ]
[ time 76 ] Run JOB 1 at PRIORITY 0 [ TICKS 3 ALLOT 1 TIME 35 (of 62) ]
[ time 77 ] Run JOB 1 at PRIORITY 0 [ TICKS 2 ALLOT 1 TIME 34 (of 62) ]
[ time 78 ] Run JOB 1 at PRIORITY 0 [ TICKS 1 ALLOT 1 TIME 33 (of 62) ]
[ time 79 ] Run JOB 1 at PRIORITY 0 [ TICKS 0 ALLOT 1 TIME 32 (of 62) ]
[ time 80 ] Run JOB 0 at PRIORITY 0 [ TICKS 9 ALLOT 1 TIME 6 (of 57) ]
[ time 81 ] Run JOB 0 at PRIORITY 0 [ TICKS 8 ALLOT 1 TIME 5 (of 57) ]
[ time 82 ] Run JOB 0 at PRIORITY 0 [ TICKS 7 ALLOT 1 TIME 4 (of 57) ]
[ time 83 ] Run JOB 0 at PRIORITY 0 [ TICKS 6 ALLOT 1 TIME 3 (of 57) ]
[ time 84 ] Run JOB 0 at PRIORITY 0 [ TICKS 5 ALLOT 1 TIME 2 (of 57) ]
[ time 85 ] Run JOB 0 at PRIORITY 0 [ TICKS 4 ALLOT 1 TIME 1 (of 57) ]
[ time 86 ] Run JOB 0 at PRIORITY 0 [ TICKS 3 ALLOT 1 TIME 0 (of 57) ]
[ time 87 ] FINISHED JOB 0
[ time 87 ] Run JOB 1 at PRIORITY 0 [ TICKS 9 ALLOT 1 TIME 31 (of 62) ]
[ time 88 ] Run JOB 1 at PRIORITY 0 [ TICKS 8 ALLOT 1 TIME 30 (of 62) ]
[ time 89 ] Run JOB 1 at PRIORITY 0 [ TICKS 7 ALLOT 1 TIME 29 (of 62) ]
[ time 90 ] Run JOB 1 at PRIORITY 0 [ TICKS 6 ALLOT 1 TIME 28 (of 62) ]
[ time 91 ] Run JOB 1 at PRIORITY 0 [ TICKS 5 ALLOT 1 TIME 27 (of 62) ]
[ time 92 ] Run JOB 1 at PRIORITY 0 [ TICKS 4 ALLOT 1 TIME 26 (of 62) ]
[ time 93 ] Run JOB 1 at PRIORITY 0 [ TICKS 3 ALLOT 1 TIME 25 (of 62) ]
[ time 94 ] Run JOB 1 at PRIORITY 0 [ TICKS 2 ALLOT 1 TIME 24 (of 62) ]
[ time 95 ] Run JOB 1 at PRIORITY 0 [ TICKS 1 ALLOT 1 TIME 23 (of 62) ]
[ time 96 ] Run JOB 1 at PRIORITY 0 [ TICKS 0 ALLOT 1 TIME 22 (of 62) ]
[ time 97 ] Run JOB 1 at PRIORITY 0 [ TICKS 9 ALLOT 1 TIME 21 (of 62) ]
[ time 98 ] Run JOB 1 at PRIORITY 0 [ TICKS 8 ALLOT 1 TIME 20 (of 62) ]
[ time 99 ] Run JOB 1 at PRIORITY 0 [ TICKS 7 ALLOT 1 TIME 19 (of 62) ]
[ time 100 ] Run JOB 1 at PRIORITY 0 [ TICKS 6 ALLOT 1 TIME 18 (of 62) ]
[ time 101 ] Run JOB 1 at PRIORITY 0 [ TICKS 5 ALLOT 1 TIME 17 (of 62) ]
[ time 102 ] Run JOB 1 at PRIORITY 0 [ TICKS 4 ALLOT 1 TIME 16 (of 62) ]
[ time 103 ] Run JOB 1 at PRIORITY 0 [ TICKS 3 ALLOT 1 TIME 15 (of 62) ]
[ time 104 ] Run JOB 1 at PRIORITY 0 [ TICKS 2 ALLOT 1 TIME 14 (of 62) ]
[ time 105 ] Run JOB 1 at PRIORITY 0 [ TICKS 1 ALLOT 1 TIME 13 (of 62) ]
[ time 106 ] Run JOB 1 at PRIORITY 0 [ TICKS 0 ALLOT 1 TIME 12 (of 62) ]
[ time 107 ] Run JOB 1 at PRIORITY 0 [ TICKS 9 ALLOT 1 TIME 11 (of 62) ]
[ time 108 ] Run JOB 1 at PRIORITY 0 [ TICKS 8 ALLOT 1 TIME 10 (of 62) ]
[ time 109 ] Run JOB 1 at PRIORITY 0 [ TICKS 7 ALLOT 1 TIME 9 (of 62) ]
[ time 110 ] Run JOB 1 at PRIORITY 0 [ TICKS 6 ALLOT 1 TIME 8 (of 62) ]
[ time 111 ] Run JOB 1 at PRIORITY 0 [ TICKS 5 ALLOT 1 TIME 7 (of 62) ]
[ time 112 ] Run JOB 1 at PRIORITY 0 [ TICKS 4 ALLOT 1 TIME 6 (of 62) ]
[ time 113 ] Run JOB 1 at PRIORITY 0 [ TICKS 3 ALLOT 1 TIME 5 (of 62) ]
[ time 114 ] Run JOB 1 at PRIORITY 0 [ TICKS 2 ALLOT 1 TIME 4 (of 62) ]
[ time 115 ] Run JOB 1 at PRIORITY 0 [ TICKS 1 ALLOT 1 TIME 3 (of 62) ]
[ time 116 ] Run JOB 1 at PRIORITY 0 [ TICKS 0 ALLOT 1 TIME 2 (of 62) ]
[ time 117 ] Run JOB 1 at PRIORITY 0 [ TICKS 9 ALLOT 1 TIME 1 (of 62) ]
[ time 118 ] Run JOB 1 at PRIORITY 0 [ TICKS 8 ALLOT 1 TIME 0 (of 62) ]
[ time 119 ] FINISHED JOB 1

Final statistics:
  Job  0: startTime   0 - response   0 - turnaround  87
  Job  1: startTime  30 - response   0 - turnaround  89

  Avg  1: startTime n/a - response 0.00 - turnaround 88.00
```

### 2. How would you run the scheduler to reproduce each of the examples in the chapter?



**Long-running Job Over Time**
>./mlfq.py -n 3 -q 10 --jlist 0,200,0 -c

![image](https://github.com/fynecontry/Operating-Systems-Virtualization-Concurrency-Persistence/assets/27024731/35d48641-b509-40d8-bace-96ddfc18a31c)

**Two jobs: A, which is a long-running CPU-intensive job, and B, which is a short-running interactive job**
>./mlfq.py -n 3 -q 10 --jlist 0,200,0:100,20,0 -c

![image](https://github.com/fynecontry/Operating-Systems-Virtualization-Concurrency-Persistence/assets/27024731/e2f2977e-2edd-45ca-a69e-b078103f07e8)

**A Mixed I/O-intensive and CPU-intensive Workload**
>./mlfq.py -n 3 -q 10 --jlist 0,200,0:50,25,1 -i 5 -c

![image](https://github.com/fynecontry/Operating-Systems-Virtualization-Concurrency-Persistence/assets/27024731/40b74006-86bc-40e4-98a9-89f6acc0e79e)

**long-running job starved by two short-running interactive jobs when competing for CPU**
> ./mlfq.py -n 3 -q 10 --jlist 0,200,0:100,25,1:101,25,1 -i 1 -c -S

![image](https://github.com/fynecontry/Operating-Systems-Virtualization-Concurrency-Persistence/assets/27024731/26358bc4-1621-4fe3-a1a9-2dc389c5e89a)

**Two jobs running without Gaming Tolerance**
> ./mlfq.py -n 3 -q 10 --jlist 0,200,0:90,100,10 -i 1 -c -S -I

![image](https://github.com/fynecontry/Operating-Systems-Virtualization-Concurrency-Persistence/assets/27024731/4767205e-1a50-427f-9585-f0f51e810ddb)


**Two jobs running With Gaming Tolerance**
> ./mlfq.py -n 3 -q 10 --jlist 0,200,0:90,100,10 -i 1 -c -S

![image](https://github.com/fynecontry/Operating-Systems-Virtualization-Concurrency-Persistence/assets/27024731/8aa940e2-3b60-4e00-b139-2a5f6a8e655a)


**Parameterizing the scheduler: Lower Priority, Longer Quanta**
>./mlfq.py --jlist 0,140,0:0,140,0 -Q 10,20,40 -a 2 -c

![image](https://github.com/fynecontry/Operating-Systems-Virtualization-Concurrency-Persistence/assets/27024731/56644fda-0985-44f6-8eba-f24b291fc93b)


### 3. How would you configure the scheduler parameters to behave just like a round-robin scheduler?
> round-robin scheduling can be achieved if `Priority(A)=Priority(B)` and are in the same queue eg: `./mlfq.py -q 10 -M 0 -c`

### 4. Craft a workload with two jobs and scheduler parameters so that one job takes advantage of the older Rules 4a and 4b (turned on with the -S flag) to game the scheduler and obtain 99% of the CPU over a particular time interval.
> ./mlfq.py -n 3 -q 10 --jlist 0,200,0:90,100,10 -i 1 -c -S -I

### 5. Given a system with a quantum length of 10ms in its highest queue, how often would you have to boost jobs back to the highest priority level (with the -B flag) in order to guarantee that a single long-running (and potentially-starving) job gets at least 5% of the CPU?
> -B (5/100) * 10 = 0.5ms

### 6. TODO: One question that arises in scheduling is which end of a queue to add a job that just finished I/O; the -I flag changes this behavior for this scheduling simulator. Play around with some workloads and see if you can see the effect of this flag.