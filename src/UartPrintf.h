#ifndef _UARTPRINTF_H_
#define _UARTPRINTF_H_

extern void Uart_Init(unsigned int Rate);
extern void Uart_Printf(unsigned char *s,unsigned char n,...);//使用的是T1作为波特率发生器

#endif