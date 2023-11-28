#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <signal.h>
#include <stdint.h>
#include <linux/stat.h>

#define FIFO_NAME "myfifo"
#define BUFFER_SIZE 300
#define SIZE 306


char outputBuffer[SIZE];
char tempBuffer[BUFFER_SIZE];
uint32_t bytesWrote;
int32_t returnCode, fd;

volatile sig_atomic_t signalReceived = 0;

void recibiSignal(int sig) {
    if (sig == SIGUSR2) {
        signalReceived = SIGUSR2;
    } else if (sig == SIGUSR1) {
        signalReceived = SIGUSR1;
    }
}

void writeData(char* data) {
    if ((bytesWrote = write(fd, data, strlen(data))) == -1) {
        perror("write");
    } else {
        printf("writer: wrote %d bytes\n", bytesWrote);
    }
}

int main(void) {
    struct sigaction sa;

    sa.sa_handler = recibiSignal;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);

    sigaction(SIGUSR2, &sa, NULL);
    sigaction(SIGUSR1, &sa, NULL);

    /* Create named fifo. No action if already exists */
    if ((returnCode = mknod(FIFO_NAME, S_IFIFO | 0666, 0)) != 0) {
        printf("Error creating named fifo: %d\n", returnCode);
        exit(1);
    }

    /* Open named fifo. Blocks until other process opens it */
    printf("waiting for readers...\n");
    if ((fd = open(FIFO_NAME, O_WRONLY)) < 0) {
        printf("Error opening named fifo file: %d\n", fd);
        exit(1);
    }

    /* open syscalls returned without error -> other process attached to named fifo */
    printf("got a reader--type some stuff\n");

    /* Loop forever */
    while (1) {
        /* Check if a signal was received */
        if (signalReceived != 0) {
            // Handle the signal-specific write operations
            if (signalReceived == SIGUSR2) {
                snprintf(outputBuffer, sizeof(outputBuffer), "SIGN:2\n");
                writeData(outputBuffer);
            } else if (signalReceived == SIGUSR1) {
                snprintf(outputBuffer, sizeof(outputBuffer), "SIGN:1\n");
                writeData(outputBuffer);
            }

            signalReceived = 0; // Reset the signal flag
        } else {
            /* Process regular input from stdin */
            if(fgets(tempBuffer, BUFFER_SIZE, stdin)!= NULL)
            {
                snprintf(outputBuffer, sizeof(outputBuffer), "DATA:%s", tempBuffer);
                writeData(outputBuffer);
            }
        }
        // Clear buffers
        memset(outputBuffer, 0, SIZE);
        memset(tempBuffer, 0, BUFFER_SIZE);
    }

    return 0;
}
