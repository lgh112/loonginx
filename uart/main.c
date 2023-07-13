#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <termios.h>
#include "uart.h"

static int uart_fd;

int main(int argc,char *argv[])
{
    char * parity;
    int speed,databits,stopbits;
    if(argc != 6)
    {
        print_info();
        exit(1);
    }
    parity = argv[4];
    speed = atoi(argv[2]);
    databits = atoi(argv[3]);
    stopbits = atoi(argv[5]);

    uart_init(argv[1],speed,databits,parity,stopbits);
    uart_receive();
    close(uart_fd);
    return 0;
}