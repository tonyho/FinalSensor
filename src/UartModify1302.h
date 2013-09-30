#ifndef _UARTMODIFY1302_H_
#define _UARTMODIFY1302_H_

#define 	ERROR 0
#define		SUCC  1

//---------------------------------------------------
#define        END_NUM     4

extern unsigned char code END_CODE[4] ;	 
extern unsigned char  end_num ;
extern unsigned char  Data_num;		// 接收数据个数记录
extern unsigned char  commd[30];			// 接收数据缓存
extern unsigned char  D_add;				// 数据地址
extern bit	 flag_deal ;		// 处理标志


extern void UartChangeDS1302(void);

#endif