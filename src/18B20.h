#ifndef _18B20_H_
#define _18B20_H_

extern bit f;             //负温度标志
extern bit flag;          //DS1820初始化成功标志
extern unsigned char tempint,tempdf;	          //温度整数部分和小数部分

extern void get_temperature(void);

#endif
