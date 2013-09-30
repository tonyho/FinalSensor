#include <reg52.h>
#include "MacroAndConst.h"
#include "uart.h"
#include "DS1302DY.h"
#include "UartModify1302.h"

unsigned char code END_CODE[4] = "#end";
unsigned char  end_num = 0;
unsigned char  Data_num = 0;		// 接收数据个数记录
unsigned char  commd[30];			// 接收数据缓存
unsigned char  D_add;				// 数据地址
bit		       flag_deal = 0;		// 处理标志
//---------------------------------------------------

//用来储存Uart收到的各个值  共7个   年 月 日 时 分 秒 星期 也就是储存每个state量  注意因为要保持两位数故要用uint16 而不是8
uint8 Uart_1302_Temp_Val[7];  

//**************************数据转换*************************************
unsigned char DEC_conver(void)
{
	unsigned char state,i;
	state = 0;  	//储存字符的中间变量
	i = 2; 			// 从commd[2] 即第三个开始  因为第一个是W或者R 第二个是空格 所以从第三个开始
//-----------------------计算地址-------------------------------
	while(1)
	{
		if( (commd[i]>47)&&(commd[i]<58) ) 			// 是‘0’-‘9’？ 从48到57共10个 分别是'0'到'9'
			state = (state*10)+(commd[i]-48);		// 计算对应的ASCII码值	 + state*10 指的是

		else if( commd[i]==' ' )
			break;			// 是空格,跳出	 什么也不储存 所以跳出

		else 													// 非'0'-'9'和' '
		{
			UART_TXDSTRING("add格式错误！请重新输入",1);
			return ERROR;									// 返回错误
		}

		if( i>4 )	// i表示Address的位数 这句用来限制地址的位数如果大约4，那么地址就是3位了,这超出了AT24C02的最大地址
		{
			UART_TXDSTRING("Address不能超过三位！请重新输入",1);
			return ERROR;
		}

		i++;
	}
 Uart_1302_Temp_Val[0] = state;
//	commd[0] = state;										// 地址存放在commd[0]中
//-------------------------计算数据个数--------------------------------
	i++;
	state = 0;
	while(1)
	{
		if( (commd[i]>47)&&(commd[i]<58) )
			state = (state*10)+(commd[i]-48);
		else if( commd[i]==' ' )	break;
		else 
		{
			UART_TXDSTRING("number格式错误！请重新输入",1);
			return ERROR;
		}
		if( i>8 )
		{
			UART_TXDSTRING("number太大！请重新输入",1);
			return ERROR;
		}
		i++;
	}
//	commd[1] = state;					// 操作个数存放在commd[1]中
 Uart_1302_Temp_Val[1] = state;

 //###############第三个数据#####################//
		i++;
	state = 0;
	while(1)
	{
		if( (commd[i]>47)&&(commd[i]<58) )
			state = (state*10)+(commd[i]-48);
		else if( commd[i]==' ' )	break;
		else 
		{
			UART_TXDSTRING("number格式错误！请重新输入",1);
			return ERROR;
		}
		if( i>8 )
		{
			UART_TXDSTRING("number太大！请重新输入",1);
			return ERROR;
		}
		i++;
	}
//	commd[2] = state;					// 操作个数存放在commd[1]中
 Uart_1302_Temp_Val[2] = state;

//---------------------------------------------------------------------
	
	
 //###############第四个数据#####################//
		i++;
	state = 0;
	while(1)
	{
		if( (commd[i]>47)&&(commd[i]<58) )
			state = (state*10)+(commd[i]-48);
		else if( commd[i]==' ' )	break;
		else 
		{
			UART_TXDSTRING("number格式错误！请重新输入",1);
			return ERROR;
		}
		if( i>8 )
		{
			UART_TXDSTRING("number太大！请重新输入",1);
			return ERROR;
		}
		i++;
	}
	commd[3] = state;					// 操作个数存放在commd[1]中
 Uart_1302_Temp_Val[3] = state;
//---------------------------------------------------------------------	


 //###############第五个数据#####################//
		i++;
	state = 0;
	while(1)
	{
		if( (commd[i]>47)&&(commd[i]<58) )
			state = (state*10)+(commd[i]-48);
		else if( commd[i]==' ' )	break;
		else 
		{
			UART_TXDSTRING("number格式错误！请重新输入",1);
			return ERROR;
		}
		if( i>8 )
		{
			UART_TXDSTRING("number太大！请重新输入",1);
			return ERROR;
		}
		i++;
	}
//	commd[4] = state;					// 操作个数存放在commd[1]中
 Uart_1302_Temp_Val[4] = state;
//---------------------------------------------------------------------



 //###############第六个数据#####################//
		i++;
	state = 0;
	while(1)
	{
		if( (commd[i]>47)&&(commd[i]<58) )
			state = (state*10)+(commd[i]-48);
		else if( commd[i]==' ' )	break;
		else 
		{
			UART_TXDSTRING("number格式错误！请重新输入",1);
			return ERROR;
		}
		if( i>8 )
		{
			UART_TXDSTRING("number太大！请重新输入",1);
			return ERROR;
		}
		i++;
	}
	//commd[5] = state;					// 操作个数存放在commd[1]中
	 Uart_1302_Temp_Val[5] = state;
//---------------------------------------------------------------------


 //###############第七个数据#####################//
		i++;
	state = 0;
	while(1)
	{
		if( (commd[i]>47)&&(commd[i]<58) )
			state = (state*10)+(commd[i]-48);  
			// 第一次进入后state=0 得到的是十位的数值  (如果是两位数,eg:12)然后第二次运行时state=1 这样就得到了完整的两位数
		else if( commd[i]==' ' )	break;	 // 只有这个才能跳出while  
		else 
		{
			UART_TXDSTRING("number格式错误！请重新输入",1);
			return ERROR;
		}
		if( i>8 )
		{
			UART_TXDSTRING("number太大！请重新输入",1);
			return ERROR;
		}
		i++;
	}
//	commd[6] = state;					// 操作个数存放在commd[1]中
 Uart_1302_Temp_Val[6] = state;
//---------------------------------------------------------------------

	
	D_add = ++i;					 	// 记录数据地址
	return SUCC;						// 正确	
}


void UartChangeDS1302()
{
	
	if( DEC_conver()==ERROR )	return;			// 错误，返回

	else
	{	
		ys  = Uart_1302_Temp_Val[0]	;
		mn  = Uart_1302_Temp_Val[1]	;
		day = Uart_1302_Temp_Val[2]	;
		hr  = Uart_1302_Temp_Val[3]	;
		min = Uart_1302_Temp_Val[4]	;
		sec = Uart_1302_Temp_Val[5]	;
		we  = Uart_1302_Temp_Val[6]	;
    }
    DS1302_Write_Ini_Data(ys,mn,day,hr,min,sec,we);
}