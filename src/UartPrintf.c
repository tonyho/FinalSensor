#include <reg52.h>
#include "UartPrintf.h"

/****************************************************************
初始化串口，11.0592MHz晶振时，支持波特率：1200，2400,4800 9600
****************************************************************/
void Uart_Init(unsigned int Rate)
{
	SCON= 0x40;                //串口方式1
	REN=1;                     //允许接收
	TMOD= 0x20;                //定时器1定时方式2
	TR1 = 1;
	switch(Rate)
	{
		case 1200:
		PCON &=0x7f;                     //SMOD=0	
		TH1= 0xe8;                //11.0592MHz 1200波特率
		TL1= 0xe8; 
		break;
	
		case 2400:
		PCON |= 0x80;                     //SMOD=1
		TH1= 0xe8;                //11.0592MHz 2400波特率
		TL1= 0xe8;
		break;

		case 4800:
		PCON |= 0x80;                     //SMOD=1
		TH1= 0xf4;                //11.0592MHz 4800波特率
		TL1= 0xf4;
		break;
		
		case 9600:
		PCON |= 0x80;                     //SMOD=1
		TH1= 0xfa;                //11.0592MHz 4800波特率
		TL1= 0xfa;
		break;

		default:
		PCON |= 0x80;             
		TH1= 0xf4;                //默认4800波特率
		TL1= 0xf4;
		break;
	}
	EA = 1;
//	ES = 1;
	ET1= 0;	
	
}


/********************************************************************************
 串口输出函数，调用前要先调用Uart_Init(unsigned int Rate)，用法基本和printf一样。
 例如输出"I Love You"，Uart_Printf("I Love You");
 
 %c  输出字符
 %d  输出十进制
 %b  输出8位二进制
 %n  换行(不是\n)
********************************************************************************/ 
void Uart_Printf(unsigned char *s,unsigned char n,...)
{
	unsigned char *ps=s;
	unsigned char *pn=&n;
	unsigned char t3,t2,t1;

//	TR1= 1;  // 因为是TR1  作为波特率发生器
	TR1 = 1;
	EA = 0;
	ES = 0;//	 
	while(*ps!=0) 	
	{
		if(*ps=='%' && *(ps+1)=='c')  //输出字符
		{
			SBUF=*pn;
			pn++;
			ps+=2;
			while(TI==0);
			TI=0;
		}

		else if(*ps=='%' && *(ps+1)=='d') //输出十进制
		{
			t3=*pn/100;
			t2=(*pn%100)/10;
			t1=(*pn%100)%10;
		
			if(t3>0)
			{
				SBUF=t3+48;
				while(TI==0);
				TI=0;
			}
			if(t3>0 || t2>0)
			{
			   	SBUF=t2+48;
				while(TI==0);
				TI=0;
			} 	
	
			SBUF=t1+48;
			pn++;
			ps+=2;
			while(TI==0);
			TI=0;
			  
		}

		else if(*ps=='%' && *(ps+1)=='b')  //输出二进制
		{
			for(t1=7;t1 >= 0;t1--)
			{
				SBUF=((*pn >> t1) & 0x01)+48; 
				while(TI==0);
				TI=0;
			}
			pn++;
			ps+=2;
		}

		else if(*ps=='%' && *(ps+1)=='n')   //输出换行符
		{
		 	SBUF=0x0d;
			while(TI==0);
			TI=0;

			SBUF=0x0a;
			while(TI==0);
			TI=0;

			ps+=2;
		}

	  	else
	  	{
			SBUF=*ps;
			ps++;
			while(TI==0);
			TI=0;
		}
		
	}
		 EA = 1;
		ES = 1;
//	TR1= 0;

}