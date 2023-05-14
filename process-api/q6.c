#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

// Function prototypes
void check_write_operation(int fd, int rc, char *buffer);

int main(int argc, char *argv[]) {

    // printf("Main process: (%d)\n", (int) getpid());
    // fflush(stdout);
    pid_t fd = open("/tmp/q6.txt", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    assert(fd >= 0);
    // fork child process 
    pid_t cpid = fork();
    if (cpid < 0) {
        // Failed to create child process
        fprintf(stderr, "Fork failed Error: %s.\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    else if (cpid == 0) {
        char *buffer = malloc(sizeof(char) * 6);
        sprintf(buffer, "hello\n");
        int write_rc = write(fd, buffer, strlen(buffer));
        check_write_operation(fd, write_rc, buffer);
        fsync(fd);
        free(buffer);
    }
    else {
        int wstatus;
        pid_t wait_rc = waitpid(cpid, &wstatus, WUNTRACED);
        if (WIFEXITED(wstatus)) {
            // Runs as soon as child process exits
            char *buffer = malloc(sizeof(char) * 60);
            sprintf(buffer, "Child process (%d) exited with status: %d\nGoodbye\n", cpid, WEXITSTATUS(wstatus));
            int write_rc = write(fd, buffer, strlen(buffer));
            check_write_operation(fd, write_rc, buffer);
            fsync(fd);
            free(buffer);
        }
    }
    // Close access to file
    close(fd);
    return 0;
}

// Checks if write was successful
void check_write_operation(int fd, int rc, char *buffer)
{
    if (rc != strlen(buffer)) {
        fprintf(stderr, "Write failed.\n");
        free(buffer);
        close(fd);
        exit(1);
    }
}