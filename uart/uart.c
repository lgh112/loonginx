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

typedef unsigned int uint32_t;
static speed_t speed_arr[] = {B230400,B115200,B57600,B38400,B19200,B9600,B4800,B2400,B1200,B600,B300};
static int     name_arr[] = {230400,115200,57600,38400,19200,9600,4800,2400,1200,600,300};

static int uart_fd;

static void uart_init(char *pserial_dev,int speed,int databits,char *parity,int stopbits)
{
    int ret = 0;
    printf("dev name is:%s \r\n",pserial_dev);
    uart_fd = open(pserial_dev,O_RDWR | O_NOCTTY);
    if (uart_fd < 0)
    {
        printf("open():%s\r\n",strerror(errno));
        exit(1);
    }
    ret = set_parity(uart_fd,speed,databits,parity,stopbits);
    if(ret)
    {
        printf("\noperating error!\r\n");
        close(uart_fd);
        exit(1);
    }

}

static void set_speed(int fd,int speed)
{
    uint32_t i;
    struct termios opt;

    tcgetattr(fd,&opt);
    tcflush(fd,TCIOFLUSH);
    cfmakeraw(&opt);
    #if 1
        for ( i = 0; i < sizeof(speed_arr)/sizeof(speed_t); i++)
        {
            if(speed == name_arr[i])
            {
                printf("serial speed=%d",speed);
                cfsetispeed(&opt,speed_arr[i]);
                cfsetospeed(&opt,speed_arr[i]);
            }
        }
    #else
        cfsetispeed(&opt,speed_arr[i]);
        cfsetospeed(&opt,speed_arr[i]); 
    
    #endif
        if(tcsetattr(fd,TCSANOW,&opt) == -1)
        {
            printf("tcsetattr():%s",strerror(errno));
            return;
        }
        tcflush(fd,TCIOFLUSH);
}

static int set_parity(int fd,int speed,int databits,char *parity,int stopbits)
{
    set_speed(fd,speed);
    struct termios options;
    if (tcgetattr(fd,&options) != 0)
    {
        perror("SetupSerial 1");
        return -1;
    }
    options.c_cflag &= ~CSIZE;
    //setting databit
    switch (databits)
    {
    case 7:
        options.c_cflag |= CS7;
        break;
    case 8:
        options.c_cflag |= CS8;
        break;
    default:
        fprintf(stderr,"Unsupported data size\n");
        return -1;
    }
    printf("databits=%d",databits);
    //setting parity
    switch (parity[0])
    {
    case 'n':
    case 'N':
        options.c_cflag &= ~PARENB;
        options.c_cflag &= ~INPCK;
        break;
    case 'o':
    case 'O':
        options.c_cflag |= (PARODD | PARENB);
        options.c_cflag |= INPCK;
        break;
    case 'e':
    case 'E':
        options.c_cflag |= PARENB;
        options.c_cflag &= ~PARODD;
        options.c_cflag |= INPCK;
        break;
    case 'S':
    case 's':
        options.c_cflag &= ~PARENB;
        options.c_cflag &= ~CSTOPB;
        break;
    default:
        fprintf(stderr,"Unsupport parity\n");
        return -1;
    }
    printf("parity=%c",parity[0]);
    //setting stopbits
    switch (stopbits)
    {
    case 1:
        options.c_cflag &= ~CSTOPB;
        break;
    case 2:
        options.c_cflag |= CSTOPB;
        break;
    default:
        fprintf(stderr,"Unsupported stopbits\n");
        return -1;
    }
    printf("stopbits=%d\n",stopbits);
    //set input parity option
    if (parity[0] != 'n')
        options.c_iflag |= INPCK;
    tcflush(fd,TCIFLUSH);
    if (tcsetattr(fd,TCSANOW,&options) != 0)
    {
        perror("SetupSerial 3");
        return -1;
    }
    return 0; 
}

static void uart_send(char *send_data)
{
    if (uart_fd > 0)
    {
        int ret = write(uart_fd,send_data,sizeof(send_data));
    }
    else
    {
        printf("send error!\n");
    }
}

static void uart_receive(void)
{
    char buf[1024];
    int ret,tem;

    while (1)
    {
        ret = 0;
        tem = 0;
        ret = read(uart_fd,buf,sizeof(buf));
        if(ret <= 0)
        {
            printf("read error!\n");
            break;
        }
        printf("uart read:%s",buf);
        uart_send("OK\n");
    }
}

void print_info(void)
{
    int i;
    printf("\n./uart /dev/ttyx speed databits parity stopbits file-name transport\n\n");
    printf("   /dev/ttyX /dev/ttySn or /dev/ttyUSBn\n");
    printf("   speed   ");
    for (i = 0;name_arr[i];i++)
    {
        printf("%d\n",name_arr[i]);
    }
    printf("  databits   8    7\n");
    printf("  parity     n    o     e\n");
    printf("  stopbits   1    2\n");
    printf("\nE.g# ./uart /dev/ttyS1 115200 8 n 1 \n\n");
    
}