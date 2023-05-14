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
void check_write_operation(int fd, ssize_t rc, char *buffer);

int main(int argc, char *argv[]) {

    int fd = open("/tmp/q3.txt", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    assert(fd >= 0);
    // fork child process 
    pid_t rc = fork();
    if (rc == -1) {
        // Failed to create child process
        fprintf(stderr, "Fork failed.\n");
        exit(1);
    }
    else if (rc == 0) {
        char *buffer = malloc(sizeof(char) * 6);
        sprintf(buffer, "hello\n");
        ssize_t write_rc = write(fd, buffer, strlen(buffer));
        check_write_operation(fd, write_rc, buffer);
        fsync(fd);
        free(buffer);
    }
    else {
        //int wait_rc = wait(NULL);
        usleep(50);
        char *buffer = malloc(sizeof(char) * 9);
        sprintf(buffer, "goodbye\n");
        ssize_t write_rc = write(fd, buffer, strlen(buffer));
        check_write_operation(fd, write_rc, buffer);
        fsync(fd);
        free(buffer);
    }
    // Close access to file
    close(fd);
    return 0;
}

// Checks if write was successful
void check_write_operation(int fd, ssize_t rc, char *buffer)
{
    if (rc != strlen(buffer)) {
        fprintf(stderr, "Write failed.\n");
        free(buffer);
        close(fd);
        exit(1);
    }
}