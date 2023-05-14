#include <assert.h>
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

    printf("Main process: (%d)\n", (int) getpid());
    fflush(stdout);
    int fd = open("/tmp/q5.txt", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    assert(fd >= 0);
    // fork child process 
    int rc = fork();
    if (rc < 0) {
        // Failed to create child process
        fprintf(stderr, "Fork failed.\n");
        exit(1);
    }
    else if (rc == 0) {
        int wait_rc = wait(NULL);
        char *buffer = malloc(sizeof(char) * 6);
        sprintf(buffer, "hello\n");
        int write_rc = write(fd, buffer, strlen(buffer));
        fsync(fd);
        check_write_operation(fd, write_rc, buffer);
        free(buffer);
    }
    else {
        pid_t wait_rc = wait(NULL);
        char *buffer = malloc(sizeof(char) * 9);
        sprintf(buffer, "goodbye\n");
        int write_rc = write(fd, buffer, strlen(buffer));
        fsync(fd);
        check_write_operation(fd, write_rc, buffer);
        free(buffer);
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