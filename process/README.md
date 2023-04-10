[Program's Overview](https://github.com/fynecontry/Operating-Systems-Virtualization-Concurrency-Persistence/blob/main/process/README-process-run.md)

# Questions

- Run `process-run.py with` the following flags: `-l 5:100,5:100.` What should the CPU utilization be (e.g., the percent of time the CPU is in use?)
  ```
  # ./process-run.py -l 5:100,5:100 -cp
  Time     PID: 0     PID: 1        CPU        IOs 
    1     RUN:cpu      READY          1            
    2     RUN:cpu      READY          1            
    3     RUN:cpu      READY          1            
    4     RUN:cpu      READY          1            
    5     RUN:cpu      READY          1            
    6        DONE    RUN:cpu          1            
    7        DONE    RUN:cpu          1            
    8        DONE    RUN:cpu          1            
    9        DONE    RUN:cpu          1            
   10        DONE    RUN:cpu          1            

  Stats: Total Time 10
  Stats: CPU Busy 10 (100.00%)
  Stats: IO Busy  0 (0.00%)
  ```
  
- Now run with these flags: ./process-run.py -l 4:100,1:0. These flags specify one process with 4 instructions (all to use the CPU), and one that simply issues an I/O and waits for it to be done. How long does it take to complete both processes?
  ```
  # ./process-run.py -l 4:100,1:0 -cp
  Time     PID: 0     PID: 1        CPU        IOs 
    1     RUN:cpu      READY          1            
    2     RUN:cpu      READY          1            
    3     RUN:cpu      READY          1            
    4     RUN:cpu      READY          1            
    5        DONE     RUN:io          1            
    6        DONE    WAITING                     1 
    7        DONE    WAITING                     1 
    8        DONE    WAITING                     1 
    9        DONE    WAITING                     1 
   10*       DONE       DONE                       

  Stats: Total Time 10
  Stats: CPU Busy 5 (50.00%)
  Stats: IO Busy  4 (40.00%)
  ```
  
- Switch the order of the processes: `-l 1:0,4:100`. What happens now? Does switching the order matter? Why?
  ```
  # ./process-run.py -l 1:0,4:100 -cp
  Time     PID: 0     PID: 1        CPU        IOs 
    1      RUN:io      READY          1            
    2     WAITING    RUN:cpu          1          1 
    3     WAITING    RUN:cpu          1          1 
    4     WAITING    RUN:cpu          1          1 
    5     WAITING    RUN:cpu          1          1 
    6*       DONE       DONE                       

  Stats: Total Time 6
  Stats: CPU Busy 5 (83.33%)
  Stats: IO Busy  4 (66.67%)
  ```
  
 - We’ll now explore some of the other flags. One important flag is `-S`, which determines how the system reacts when a process issues an I/O. With the flag set to `SWITCH_ON_END`, the system will NOT switch to another process while one is doing I/O, instead of waiting until the process is completely finished. What happens when you run the following two processes (`-l 1:0,4:100 -c -S SWITCH_ON_END`), one doing I/O and the other doing CPU work?
   ```
     # ./process-run.py -l 1:0,4:100 -c -S SWITCH_ON_END -p
    Time     PID: 0     PID: 1        CPU        IOs 
      1      RUN:io      READY          1            
      2     WAITING      READY                     1 
      3     WAITING      READY                     1 
      4     WAITING      READY                     1 
      5     WAITING      READY                     1 
      6*       DONE    RUN:cpu          1            
      7        DONE    RUN:cpu          1            
      8        DONE    RUN:cpu          1            
      9        DONE    RUN:cpu          1            

    Stats: Total Time 9
    Stats: CPU Busy 5 (55.56%)
    Stats: IO Busy  4 (44.44%)
    ```
- Now, run the same processes, but with the switching behavior set to switch to another process whenever one is `WAITING` for I/O (`-l 1:0,4:100 -c -S SWITCH_ON_IO`).
  ```
    # ./process-run.py -l 1:0,4:100 -c -S SWITCH_ON_IO -p
    Time     PID: 0     PID: 1        CPU        IOs 
      1      RUN:io      READY          1            
      2     WAITING    RUN:cpu          1          1 
      3     WAITING    RUN:cpu          1          1 
      4     WAITING    RUN:cpu          1          1 
      5     WAITING    RUN:cpu          1          1 
      6*       DONE       DONE                       

    Stats: Total Time 6
    Stats: CPU Busy 5 (83.33%)
    Stats: IO Busy  4 (66.67%)
  ```
 
- One other important behavior is what to do when an I/O completes. With `-I IO_RUN_LATER`, when an I/O completes, the process that issued it does not necessarily run right away; rather, whatever was running at the time keeps running. What happens when you run this combination of processes? (Run `./process-run.py -l 3:0,5:100,5:100,5:100 -S SWITCH_ON_IO -I IO_RUN_LATER -c -p`) Are system resources being effectively utilized?
  ```
  # ./process-run.py -l 3:0,5:100,5:100,5:100 -S SWITCH_ON_IO -I IO_RUN_LATER -cp
  Time     PID: 0     PID: 1     PID: 2     PID: 3        CPU        IOs 
    1      RUN:io      READY      READY      READY          1            
    2     WAITING    RUN:cpu      READY      READY          1          1 
    3     WAITING    RUN:cpu      READY      READY          1          1 
    4     WAITING    RUN:cpu      READY      READY          1          1 
    5     WAITING    RUN:cpu      READY      READY          1          1 
    6*      READY    RUN:cpu      READY      READY          1            
    7       READY       DONE    RUN:cpu      READY          1            
    8       READY       DONE    RUN:cpu      READY          1            
    9       READY       DONE    RUN:cpu      READY          1            
   10       READY       DONE    RUN:cpu      READY          1            
   11       READY       DONE    RUN:cpu      READY          1            
   12       READY       DONE       DONE    RUN:cpu          1            
   13       READY       DONE       DONE    RUN:cpu          1            
   14       READY       DONE       DONE    RUN:cpu          1            
   15       READY       DONE       DONE    RUN:cpu          1            
   16       READY       DONE       DONE    RUN:cpu          1            
   17      RUN:io       DONE       DONE       DONE          1            
   18     WAITING       DONE       DONE       DONE                     1 
   19     WAITING       DONE       DONE       DONE                     1 
   20     WAITING       DONE       DONE       DONE                     1 
   21     WAITING       DONE       DONE       DONE                     1 
   22*     RUN:io       DONE       DONE       DONE          1            
   23     WAITING       DONE       DONE       DONE                     1 
   24     WAITING       DONE       DONE       DONE                     1 
   25     WAITING       DONE       DONE       DONE                     1 
   26     WAITING       DONE       DONE       DONE                     1 
   27*       DONE       DONE       DONE       DONE                       

  Stats: Total Time 27
  Stats: CPU Busy 18 (66.67%)
  Stats: IO Busy  12 (44.44%)
  ```
- Now run the same processes, but with `-I IO_RUN_IMMEDIATE` set, which immediately runs the process that issued the I/O. How does this behavior differ? Why might running a process that just completed an I/O again be a good idea?
  ```
  # ./process-run.py -l 3:0,5:100,5:100,5:100 -S SWITCH_ON_IO -I IO_RUN_IMMEDIATE -cp
  Time     PID: 0     PID: 1     PID: 2     PID: 3        CPU        IOs 
    1      RUN:io      READY      READY      READY          1            
    2     WAITING    RUN:cpu      READY      READY          1          1 
    3     WAITING    RUN:cpu      READY      READY          1          1 
    4     WAITING    RUN:cpu      READY      READY          1          1 
    5     WAITING    RUN:cpu      READY      READY          1          1 
    6*     RUN:io      READY      READY      READY          1            
    7     WAITING    RUN:cpu      READY      READY          1          1 
    8     WAITING       DONE    RUN:cpu      READY          1          1 
    9     WAITING       DONE    RUN:cpu      READY          1          1 
   10     WAITING       DONE    RUN:cpu      READY          1          1 
   11*     RUN:io       DONE      READY      READY          1            
   12     WAITING       DONE    RUN:cpu      READY          1          1 
   13     WAITING       DONE    RUN:cpu      READY          1          1 
   14     WAITING       DONE       DONE    RUN:cpu          1          1 
   15     WAITING       DONE       DONE    RUN:cpu          1          1 
   16*       DONE       DONE       DONE    RUN:cpu          1            
   17        DONE       DONE       DONE    RUN:cpu          1            
   18        DONE       DONE       DONE    RUN:cpu          1            

  Stats: Total Time 18
  Stats: CPU Busy 18 (100.00%)
  Stats: IO Busy  12 (66.67%)
  ```
- Now run with some randomly generated processes: `-s 1 -l 3:50,3:50` or `-s 2 -l 3:50,3:50` or `-s 3 -l 3:50,3:50`. See if you can predict how the trace will turn out. What happens when you use the flag `-I IO_RUN_IMMEDIATE` vs. `-I_IO RUN_LATER`? What happens when you use `-S SWITCH_ON_IO` vs.`-S SWITCH_ON_END`?
  ```
  # ./process-run.py -s 2 -l 3:50,3:50 -cp         
  Time     PID: 0     PID: 1        CPU        IOs 
    1      RUN:io      READY          1            
    2     WAITING    RUN:cpu          1          1 
    3     WAITING     RUN:io          1          1 
    4     WAITING    WAITING                     2 
    5     WAITING    WAITING                     2 
    6*     RUN:io    WAITING          1          1 
    7     WAITING    WAITING                     2 
    8*    WAITING     RUN:io          1          1 
    9     WAITING    WAITING                     2 
   10     WAITING    WAITING                     2 
   11*    RUN:cpu    WAITING          1          1 
   12        DONE    WAITING                     1 
   13*       DONE       DONE                       

  Stats: Total Time 13
  Stats: CPU Busy 6 (46.15%)
  Stats: IO Busy  11 (84.62%)
  ```

  ```
  # ./process-run.py -s 2 -l 3:50,3:50 -I IO_RUN_IMMEDIATE -cp
  Time     PID: 0     PID: 1        CPU        IOs 
    1      RUN:io      READY          1            
    2     WAITING    RUN:cpu          1          1 
    3     WAITING     RUN:io          1          1 
    4     WAITING    WAITING                     2 
    5     WAITING    WAITING                     2 
    6*     RUN:io    WAITING          1          1 
    7     WAITING    WAITING                     2 
    8*    WAITING     RUN:io          1          1 
    9     WAITING    WAITING                     2 
   10     WAITING    WAITING                     2 
   11*    RUN:cpu    WAITING          1          1 
   12        DONE    WAITING                     1 
   13*       DONE       DONE                       

  Stats: Total Time 13
  Stats: CPU Busy 6 (46.15%)
  Stats: IO Busy  11 (84.62%)
  ```

  ```
  # ./process-run.py -s 2 -l 3:50,3:50 -I IO_RUN_LATER -cp
  Time     PID: 0     PID: 1        CPU        IOs 
    1      RUN:io      READY          1            
    2     WAITING    RUN:cpu          1          1 
    3     WAITING     RUN:io          1          1 
    4     WAITING    WAITING                     2 
    5     WAITING    WAITING                     2 
    6*     RUN:io    WAITING          1          1 
    7     WAITING    WAITING                     2 
    8*    WAITING     RUN:io          1          1 
    9     WAITING    WAITING                     2 
   10     WAITING    WAITING                     2 
   11*    RUN:cpu    WAITING          1          1 
   12        DONE    WAITING                     1 
   13*       DONE       DONE                       

  Stats: Total Time 13
  Stats: CPU Busy 6 (46.15%)
  Stats: IO Busy  11 (84.62%
  ```

  ```
  # ./process-run.py -s 2 -l 3:50,3:50 -S SWITCH_ON_IO -cp
  Time     PID: 0     PID: 1        CPU        IOs 
    1      RUN:io      READY          1            
    2     WAITING    RUN:cpu          1          1 
    3     WAITING     RUN:io          1          1 
    4     WAITING    WAITING                     2 
    5     WAITING    WAITING                     2 
    6*     RUN:io    WAITING          1          1 
    7     WAITING    WAITING                     2 
    8*    WAITING     RUN:io          1          1 
    9     WAITING    WAITING                     2 
   10     WAITING    WAITING                     2 
   11*    RUN:cpu    WAITING          1          1 
   12        DONE    WAITING                     1 
   13*       DONE       DONE                       

  Stats: Total Time 13
  Stats: CPU Busy 6 (46.15%)
  Stats: IO Busy  11 (84.62%)
  ```

  ```
  # ./process-run.py -s 2 -l 3:50,3:50 -S SWITCH_ON_END -cp
  Time     PID: 0     PID: 1        CPU        IOs 
    1      RUN:io      READY          1            
    2     WAITING      READY                     1 
    3     WAITING      READY                     1 
    4     WAITING      READY                     1 
    5     WAITING      READY                     1 
    6*     RUN:io      READY          1            
    7     WAITING      READY                     1 
    8     WAITING      READY                     1 
    9     WAITING      READY                     1 
   10     WAITING      READY                     1 
   11*    RUN:cpu      READY          1            
   12        DONE    RUN:cpu          1            
   13        DONE     RUN:io          1            
   14        DONE    WAITING                     1 
   15        DONE    WAITING                     1 
   16        DONE    WAITING                     1 
   17        DONE    WAITING                     1 
   18*       DONE     RUN:io          1            
   19        DONE    WAITING                     1 
   20        DONE    WAITING                     1 
   21        DONE    WAITING                     1 
   22        DONE    WAITING                     1 
   23*       DONE       DONE                       

  Stats: Total Time 23
  Stats: CPU Busy 6 (26.09%)
  Stats: IO Busy  16 (69.57%)
  ```