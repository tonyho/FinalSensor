#include<reg52.h>
#include<intrins.h>
#include"MacroAndConst.h"

#define DB P1

sbit LCDRS = P2^0;
sbit LCDRW = P2^1;
sbit LCDE = P2^2;
sbit DB7 = DB^7;

uint8 code SelfFont[ 40 ] =			  // 最多到40
{	
	0x08, 0x0f, 0x12, 0x0f, 0x0a, 0x1f, 0x02, 0x00,			//字模"年"	0	   
	0x0f, 0x11, 0x1f, 0x11, 0x1f, 0x11, 0x13, 0x00,			//字模"月"	1
	0x1f, 0x11, 0x11, 0x1f, 0x11, 0x11, 0x1f, 0x00,			//字模"日"	2
	0x05, 0x0e, 0x14, 0x17, 0x14, 0x0e, 0x05, 0x00,			//闹钟标志	3
   // 0x10, 0x18, 0x1C, 0x1E, 0x1C, 0x18, 0x10, 0x00,		//闹钟标志占两行(两个格子)	4 这个是播放的箭头符号

	0x19, 0x1b, 0x04, 0x08, 0x08, 0x04, 0x03, 0x01			//℃的字符	   
};

//==================================================================//
//							延时函数								//
//==================================================================//
//空操作
void lnop()
{
	;
}

// 短延时5MS
void Delay5(void)
{
	word i=5552;
	while(i--);
}

//长延时400MS
void Delay400(void)
{
	byte i=5;
	word j;
	while(i--)
	{
		j=7269;
		while(j--);
	};
}

/***************************************
检测忙信号方法:
***************************************/
//忙信号等待
void Busy()
{
 do
 {
	DB = 0xff;
	LCDRS = 0;
	LCDRW = 1;
	LCDE = 1;
	lnop();
 }while( DB7 );
}

/******************************************************
				读写函数
[注:	RS	寄存器选择	1-数据寄存器	0-指令寄存器
		RW	读/写选择	1-读			0-写
		E	使能信号	1-读取			下降沿-写入
*******************************************************/
//写入指令	（RS=0	RW=0	E下降沿）
void WriteC( byte dd , byte jc )
{
	if(jc)	Busy();
	LCDRS = 0;
	LCDRW = 0;
	DB = dd;
	LCDE = 1;
	lnop();
	LCDE = 0;
}

//写入数据	（RS=1	RW=0	E下降沿）
void WriteD( byte dd)
{
	Busy();
	LCDRS = 1;
	LCDRW = 0;
	DB = dd;
	LCDE = 1;
	lnop();
	LCDE = 0;
}

void LcdSetXY( uint8 X, uint8 Y )		 //设置显示坐标位置
{
	uint8 Address ;
	if( Y == 0 )
	{
		Address = 0x80 + X ;                //第一行
	}
	else
	{
		Address = 0xc0 + X ;                //第二行
	}
	WriteC( Address,0 ) ;
}

//指定位置显示一个字符
void DisC(byte x,byte y,byte dd)
{
	y &= 0x1;
	x &= 0xF; //限制X不能大于15，Y不能大于1
	if (y)
	{
		x |= 0x40; //当要显示第二行时地址码+0x40;
	}
	x |= 0x80; // 算出指令码	=> 80H + 真实地址码
	WriteC(x, 0); //这里不检测忙信号，发送地址码
	WriteD(dd);
}


//指定位置显示一串字符

void DisS(byte x,byte y,byte code *dd)
{
	byte i,l=0,j;
	while (dd[l] >31){l++;};	//判断字符串长度
	for (i=0;i<l;i++) 
	{
		DisC(x++,y,dd[i]);
		if (x==16)
		{
			x=0;	//列起始
		 	y ^= 1;	//行切换
			if(y == 0)	//当显示满一屏后，停4S继续显示
			{
				for(j=0; j<10; j++)
					Delay400();
				WriteC( 0x01,1 );
			}
		}
	}
}


void v_LcdWriteSelfFont_f( void )          //将自定义字符写入CGRAM(最多8个)
{
	uint8 i ;
	for( i = 0 ; i < 40 ;i++ )
	{
		WriteC( 0x40 + i,0 ) ;   //CGRAM起始地址从0x40开始
		WriteD( SelfFont[ i ] ) ;
	}
}

void v_LcdDisplaySelfFont_f( uint8 Address )
{
	 	WriteD( Address ) ;         //Address取值范围0~7,分别对应8个自定义字符
}


/*****************************************************************
                   复位函数
*****************************************************************/
void LCDRst()
{
	Delay5();	//延时15ms
	Delay5();
	Delay5();
	WriteC( 0x38,0 );	//置显示模式
	Delay5();			//连续三次
	WriteC( 0x38,0 );	//不检测忙信号
	Delay5();
	WriteC( 0x38,0 );
//----------------------//以后每次都要检测忙信号
	WriteC( 0x38,1 );	//写指令（指令号，1-检测忙信号	0-不检测）
	WriteC( 0x08,1 );	//关显示
	WriteC( 0x01,1 );	//显显示屏
	WriteC( 0x06,1 );	//读/写一字符后，地址/光标均加一
	WriteC( 0x0c,1 );	//开显示
}

