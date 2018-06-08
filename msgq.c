#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/time.h>
#include <string.h>
#include "util.c"

struct mymesg {
    long mtype;         /* positive message type */
    char mtext[];       /* message data, of length nbytes */
};

int main(int argc, char *argv[]) {
    int i, size, count, msgid;
    struct timeval begin, end;
    key_t key;

    if (argc != 3) {
        printf("usage: ./msgq <size> <count>\n");
        return 1;
    }

    size = atoi(argv[1]);
    count = atoi(argv[2]);

    struct mymesg* buf;

    buf = malloc(sizeof(*buf) + size * sizeof(buf->mtext[0]));
    if (buf == NULL) {
        perror("malloc");
        return 1;
    }
 
    // ftok to generate unique key
    key = ftok(__FILE__, 65);
    if (key == -1) {
        perror("ftok");
        return 1;
    }
 
    // msgget creates a message queue
    // and returns identifier
    msgid = msgget(IPC_PRIVATE, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("msgget");
        return 1;
    }

    if (fork() == 0) { /* Child process */
        gettimeofday(&begin, NULL);
        for (i = 0; i < count; i++) {
            if (msgrcv(msgid, buf, size, 0, 0) != size) {
                perror("read");
                return 1;
            }
        }
        gettimeofday(&end, NULL);

        printf("%f\n", get_delta_timeofday(&begin, &end));
        fflush(stdout);

        // print_result(&begin, &end, size, count);

        // Remove the queue
        if (msgctl(msgid, IPC_RMID, NULL) == -1) {
            perror("Error removing message queue");
        }
    } else { /* Parent process */
        buf->mtype = 1;
        for (i = 0; i < count; i++) {
            // memset(buf->mtext, i + '0', size);
             // msgsnd to send message
            if (msgsnd(msgid, buf, size, 0) == -1) {
                perror("wirte");
                return 1;
            }
        }
    }
    free(buf);
    return 0;
}
