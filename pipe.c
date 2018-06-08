#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include "util.c"

int main(int argc, char *argv[]) {
    int pipefd[2] = {0};

    int i, size, count, sum, n;
    char *buf;
    struct timeval begin, end;

    if (argc != 3) {
        printf("usage: ./pipe <size> <count>\n");
        return 1;
    }

    size = atoi(argv[1]);
    count = atoi(argv[2]);

    buf = malloc(size);
    if (buf == NULL) {
        perror("malloc");
        return 1;
    }

    if (pipe(pipefd) == -1) {
        perror("pipe");
        return 1;
    }

    if (fork() == 0) { /* Child process */
        sum = 0;
        gettimeofday(&begin, NULL);
        for (i = 0; i < count; i++) {
            n = read(pipefd[0], buf, size);
            if (n == -1) {
                perror("read");
                return 1;
            }
            sum += n;
        }
        if (sum != count * size) {
            return 1;
        }
        gettimeofday(&end, NULL);

        print_result(&begin, &end, size, count);
    } else { /* Parent process */
        for (i = 0; i < count; i++) {
            memset(buf, i + '0', size);
            if (write(pipefd[1], buf, size) != size) {
                perror("wirte");
                return 1;
            }
        }
    }

    return 0;
}
