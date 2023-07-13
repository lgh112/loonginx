#ifndef __UART_H
#define __UART_H

static void set_speed(int fd,int speed);
static int set_parity(int fd,int speed,int databits,char *parity,int stopbits);
static void uart_init(char *pserial_dev,int speed,int databits,char *parity,int stopbits);
static void uart_send(char *send_data);
static void uart_receive(void);
void print_info(void);

#endif