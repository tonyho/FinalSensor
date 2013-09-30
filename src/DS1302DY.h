#ifndef _DS1302DY_H_
#define _DS1302DY_H_

extern unsigned char sec,min,hr,day,we,mn,ys;

extern void EnableWR();
extern void DisableWR();
extern void	DS1308_init(void);	

extern void DS1302_Write_Ini_Data(yea,mon,day,hou,min,sec,wee);

extern void  read_DS1308(void);

extern unsigned char ReadYs();
extern unsigned char ReadMn();
extern unsigned char ReadDay();
extern unsigned char ReadHr();
extern unsigned char ReadMin();
extern unsigned char ReadSec();
extern unsigned char ReadWe();

extern void WriteYs(unsigned char num);
extern void WriteMn(unsigned char num);
extern void WriteDay(unsigned char num);
extern void WriteHr(unsigned char num);
extern void WriteMin(unsigned char num);
extern void WriteSec(unsigned char num);
extern void WriteWe(unsigned char num);


//extern void WriteYs(unsigned char num);

#endif							   