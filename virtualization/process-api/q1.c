/*
Program forks a process and then updates value in parent and child processes.
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[]){
    int x = 100;
    printf("Value of x in main (%d): (%d)\n", getpid(), x);
    
    // Create child process
    pid_t rc = fork();
    if (rc < 0) {
        // Failed to create child process
        fprintf(stderr, "fork failed\n");
        exit(1);
    }
    else if (rc == 0){
        // Child process created
        x += 10;
        printf("Value of x updated in child (%d): (%d)\n", getpid(), x);
    }
    else {
        x += 5;
        printf("Value of x updated in main (%d): (%d)\n", getpid(), x);
    }
    return 0;
}