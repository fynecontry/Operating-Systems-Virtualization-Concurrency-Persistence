# Summary KEY PROCESS API TERMS

Each process has a name; in most systems, that name is a number known as a **process ID (PID)**.

The `fork()` system call is used in UNIX systems to create a new process. The creator is called the parent; the newly created process is called the child. As sometimes occurs in real life, the child process is a nearly identical copy of the parent.

The `wait()` system call allows a parent to wait for its child to complete execution.

The `exec()` family of system calls allows a child to break free from its similarity to its parent and execute an entirely new program.

A UNIX shell commonly uses `fork(), wait(), and exec()` to launch user commands; the separation of fork and exec enables features like input/output redirection, pipes, and other cool features, all without changing anything about the programs being run.

Process control is available in the form of signals, which can cause jobs to stop, continue, or even terminate.

Which processes can be controlled by a particular person is encapsulated in the notion of a user; the operating system allows multiple users onto the system, and ensures users can only control their own processes.

A superuser can control all processes (and indeed do many other things); this role should be assumed infrequently and with caution for security reasons.


## Exercises
### Question 1
Write a program that calls `fork()`. Before calling `fork()`, have the main process access a variable (e.g., x) and set its value to something (e.g., 100). What value is the variable in the child process? What happens to the variable when both the child and parent change the value of x?
```
Value of x in main (8538): (100)
Value of x updated in main (8538): (105)
Value of x updated in child (8539): (110)
```
Each process has its own address space and maintains its own copy

### Question 2
Write a program that opens a file (with the open() system call) and then calls fork() to create a new process. Can both the child and parent access the file descriptor returned by open()? What happens when they are writing to the file concurrently, i.e., at the same time?
```
$ ./q2 && cat /tmp/q2.txt
Main process: (9586)
Hi, parent process: (9586) wrote this.
Hi, child process: (9587) wrote this.
```
Both processes writing concurrently can lead to unpredictable behaviours. 

### Question 3
Write another program using fork(). The child process should print “hello”; the parent process should print “goodbye”. You should try to ensure that the child process always prints first; can you do this without calling wait() in the parent?
```
$ ./q3 && cat /tmp/q3.txt
hello
goodbye
```
We can use the library function `usleep()` instead.

### Question 4
Write a program that calls `fork()` and then calls some form of `exec()` to run the program /bin/ls. See if you can try all of the variants of `exec()`, including (on Linux) `execl(), execle(), execlp(), execv(), execvp(), and execvpe()`. Why do you think there are so many variants of the same basic call?
```
$ ./q4
Main process: (11166) begins
Forked Child process: (11167)
a                            fork.py  q2    q3.c  q5    q6.c  q8    test.c
direct_execution_exercise    q1       q2.c  q4    q5.c  q7    q8.c  testfile.txt
direct_execution_exercise.c  q1.c     q3    q4.c  q6    q7.c  test
Main process: 11166 resumes after child process (11167) finishes.
```

### Question 5
Now write a program that uses `wait()` to wait for the child process to finish in the parent. What does `wait()` return? What happens if you use `wait()` in the child?
```
$ ./q5 && cat /tmp/q5.txt
Main process: (11971)
hello
goodbye
```
`wait()` - on success, returns the process ID of the terminated child; on error, -1 is returned.

### Question 6
Write a slight modification of the previous program, this time using `waitpid()` instead of `wait()`. When would `waitpid()` be useful?
```
$ ./q6 && cat /tmp/q6.txt
hello
Child process (12383) exited with status: 0
Goodbye
```
The `waitpid()` system call suspends execution of the calling thread until a child specified by pid argument has changed state. this would be useful if we wanted to specify specific child processes to wait for or check when a child process changes to a particular state, eg. any child process whose process group ID is equal to the absolute value of `pid`

### Question 7
Write a program that creates a child process, and then in the child closes standard output (`STDOUT_FILENO`). What happens if the child calls `printf()` to print some output after closing the descriptor?
```
$ ./q7
Hi this is parent process!
```
`printf()` takes no effect since `STDOUT_FILENO` is closed.


### Question 8
Write a program that creates two children, and connects the standard output of one to the standard input of the other, using the `pipe()` system call.
```
$ cat testfile.txt 
Rain
John
Operating
Distributed
Classic

$ ./q8
Classic
Distributed
John
Operating
Rain
```
`q8.c` - opens cats the content of `testfile.txt` in on child process and using `pipe()` and `dup2()` sends the output to parent process for sorting