#include <reg52.h>
#include <intrins.h>
#include <stdio.h>
#include "MacroAndConst.h"
#include "DS1302DY.h"
#include "LCD1602YT.h"
#include "18B20.h"
#include "key44y.h"
#include "UartPrintf.h"

sbit LED = P3^3;
sbit BELL = P3^7;

char i=0; //全局按键SubSw标志  子菜单按键9

char TemF;	// 温度的标志位
//char TimeString[21],TempString[9]; // 分别是时间和温度储存字符串   是字符串  从它们的数值转换过来  在ChangAllValueToString()函数中进行 
//char KeyValMain = 0;  //main函数中4*4键盘扫描值

//uint8 DS1302[7] ; //将DS1302中的年月日时分秒放入数组中  方便与DS1302Ch数组的交流
unsigned char DS1302Ch[7] = {0};
unsigned char DS1302Limit[7] = {99,12,31,23,59,59,7};

uint8 volatile PageF = 0;
uint8 volatile SubSw = 0;

int8 TiAH = 8;	//闹钟小时
int8 TiAM = 1;	//闹钟分钟
bit   TAOC = 1; //闹钟开关标志位


uint8 TeAH = 30; //DS18B20报警上限温度的整数部分
//uint8 TeAHFloat = 5;   // DS18B20报警上限温度的小数部分
bit TeAHF	= 1;   // 符号位

uint8 TeAL = 20;  //DS18B20报警下限温度的整数部分
//uint8 TeALFloat = 4;
bit TeALF	= 0;

uint8 YearCh = 10;
uint8 MontCh = 9 ;

void LCD1602_Display_1302();
void LCD1602_Display_18B20();
void SystemIni( );
void DisplayTempTime( );
void KeyEqu2( ); // 判断按键是否为2
void KeyEqu9( ); // 按键9 为子选项按键 对应SubSW变量
void DisplayTimeAlarm();
void DisplayTempAlarm();
void KeyEqu1_Plus( uint8 i);
void KeyEqu5_Sub(uint8 i);
void SetDs1302Display(uint8 i);
//void Bell(void);
void delay1ms();   // 这个延时函数并不知道大约多久,只是用于在光标和Bell中稍微延时一下罢了
uint8 KeyData_Plus( uint8 Data );   //对应的变量加
uint8 KeyData_Sub ( uint8 Data ); //对应的变量减
void TimeAlarmBell(); //闹钟蜂鸣器报警
void TempAlarmBellLed(); // 温度报警函数
void JudgeMonthDays(void); 	 // 设置月份的最多天数
uint8 GetWeekFromDay(unsigned char year,unsigned char month,unsigned char day);//计算任一天是星期几

/***************************************************************************************************/
void UartService(void)  interrupt 4
{
	 char RecieveBuff = 0;
	EA = 0;
	ES = 0;
   //************************用上位机发送数据时要用16进制发送,否则不准确***************************//
		if(1 == RI)
		{
			RecieveBuff = SBUF;				// 缓存接收到的数据
			RI = 0;						// 接收标志清零
		}
		if(RecieveBuff == 0)
		{
			LED = 0;
			BELL = 1;
			delay1ms();
			delay1ms();
			delay1ms();
			Uart_Printf("%nGet %d%n",RecieveBuff);
		}
	else	if(RecieveBuff == 1)
		{
			LED = 0;  

			BELL = 0;
		
			delay1ms();
			Uart_Printf("%nGet %d%n",RecieveBuff);
		}
	else	if(RecieveBuff == 2)
		{
			LED  = 0;  

			BELL = 0;
			delay1ms();
			delay1ms();
			delay1ms();
			delay1ms();
			delay1ms();
			delay1ms();
			Uart_Printf("%nGet %d%n",RecieveBuff);
		}
//	else
	if(RecieveBuff == 'W')	  //发送这个时上位机用字符模式(用串口调试器软件) 返回的也是W  注意收发都是字符模式(不要选16进制)
		{
			LED = 0;  

			BELL = 0;
			delay1ms();
			delay1ms();
			delay1ms();
				delay1ms();
			delay1ms();
			delay1ms();
				delay1ms();
			delay1ms();
			delay1ms();
				delay1ms();
			delay1ms();
			delay1ms();
			Uart_Printf("%nGet %c%n",RecieveBuff);	
		}
		BELL = 1;
/*
	if(state == 1)
		{
			BELL = 0;
			LED = 0;
			delay1ms();
		}
	if(state == 2)
	{
		BELL = 1;
		LED  = 1;
		delay1ms();
	}
*/	
	EA = 1;
	ES = 1;
	//TR1 = 1;

}


void main()
{
   SystemIni( );
 
  while(1)
  {
  	/**********************************
	while(0 == RI);
	if(1 == RI)
		{
			RI = 0;
			LED = 0;
		}
     *******************************/

	 Uart_Printf("Hello%n");
//	UART_TXDSTRING("Test the Uart is OK !! From Func", 1);
//	    printf("From Printf");


	   TempAlarmBellLed(); // 温度报警函数
//=============================---1---Time Temperature Display==========================================//	
  	    SubSw = 0;
	  while(PageF == 2)
	  {
		 TimeAlarmBell();//Time Alarm 闹钟
		 TempAlarmBellLed();

		for( i = 0 ; i<8; i++)	// i为SubSw9按键值
		{
			//Clear1602 	 // 清屏  用于消去闹钟标志在没有指定的位置多显示

			TimeAlarmBell();
			TempAlarmBellLed(); // 温度报警函数
		
			if(1 == SubSw)		DS1302Ch[0] = ReadYs();
			if(2 == SubSw)		DS1302Ch[1] = ReadMn();
			if(3 == SubSw)		DS1302Ch[2] = ReadDay();
			if(4 == SubSw)		DS1302Ch[3] = ReadHr();
			if(5 == SubSw)		DS1302Ch[4] = ReadMin();
			if(6 == SubSw)		DS1302Ch[5] = ReadSec();
			if(7 == SubSw)		DS1302Ch[6] = ReadWe();
		 
			if(2 != PageF)	break;

			while(i == SubSw)
		    {

				if(SubSw == 0)	break; 

				SetDs1302Display(SubSw);

				if(0 == DS1302Ch[6])  //防止星期出现0  星期为unsigned char 型  在SetDs1302Display()函数中加减时能出现0
					DS1302Ch[6] = 1;
				
				if(0 == DS1302Ch[1])  //防止月份出现0  月份为unsigned char 型  在SetDs1302Display()函数中加减时能出现0
					DS1302Ch[1] = 1;


//******************在下面这段边界判断中 用到了DS1302Ch[]数组 因此在这之前需要先刷新一遍这个数组****************************//
//*****************具体函数在SystemIni()系统初始化函数中(	DS1302Ch[0] = ReadYs();...语句)*********************************//
				JudgeMonthDays(); 
				if(DS1302Ch[i-1] > DS1302Limit[i-1])	// 防止调节时间时过界  如调小时时 大于24
				{
					DS1302Ch[i-1] = DS1302Limit[i-1]; 
				}
			
				if(0 == DS1302Ch[i-1] && KeyScanValue()==5)	 //
				{
					DS1302Ch[i-1] = DS1302Limit[i-1];
				}
				if(1 == DS1302Ch[1]&& KeyScanValue()==5)
					DS1302Ch[1] = 12;
						
				if(DS1302Limit[i-1] == DS1302Ch[i-1] && KeyScanValue()==1)
				{
					if( 1 != i) DS1302Ch[i-1] = 0;  // 星期从1开始 而非0
					else 
						DS1302Ch[i-1] = 1;
				}

				if(1 == i)	
				{
					WriteYs (DS1302Ch[0]);  WriteWe( GetWeekFromDay(ReadYs(), ReadMn(), ReadDay()) );   //自动设置星期 
				}
				if(2 == i)
				{
					WriteMn (DS1302Ch[1]); WriteWe( GetWeekFromDay(ReadYs(), ReadMn(), ReadDay()) );   //自动设置星期 
				}
				if(3 == i)
				{
					WriteDay(DS1302Ch[2]);	WriteWe( GetWeekFromDay(ReadYs(), ReadMn(), ReadDay()) );   //自动设置星期 
				}
				if(4 == i)
				{
					WriteHr (DS1302Ch[3]);WriteWe( GetWeekFromDay(ReadYs(), ReadMn(), ReadDay()) );   //自动设置星期 
				}
				if(5 == i)
				{
					WriteMin(DS1302Ch[4]);WriteWe( GetWeekFromDay(ReadYs(), ReadMn(), ReadDay()) );   //自动设置星期 
				}

				if(6 == i)
				{
					WriteSec(DS1302Ch[5]);WriteWe( GetWeekFromDay(ReadYs(), ReadMn(), ReadDay()) );   //自动设置星期 
				}
				if(7 == i)
				{ 
				 WriteWe( GetWeekFromDay(ReadYs(), ReadMn(), ReadDay()) );   //自动设置星期  注意这条语句是在 7==i 的条件下才运行
				 															 //所有只有按9键(使SubSw==7)使i==7,才满足while(i==SubSw)才运行
																			 //因此需要按9键 让光标跳到星期位置 才会自动设置
				//	WriteWe (DS1302Ch[6]);
				}
		
				if(0 == SubSw) 			
				{
				  TimeAlarmBell();
				  TempAlarmBellLed(); // 温度报警函数
				  WriteC( NOFlashCur1602 , 1 );
			  
				  DisplayTempTime( );
				  KeyEqu9( );
				}

		/**************这两句仅仅是用于让光标显示清晰***下面的也一样****************/		  
				  delay1ms();
				  WriteC( NOFlashCur1602 , 1 );
		/***************************************************************************/
			    DisplayTempTime( );
			    KeyEqu9( );	
			    KeyEqu2( );
				if(2 != PageF)	break;	
			}

			while(0 == SubSw)
			{
				TimeAlarmBell();
	
				DisplayTempTime( );
			    KeyEqu9( );	
			    KeyEqu2( );
		 		if(2 != PageF)	break;
			}

		 }

			KeyEqu9( );
			if(PageF != 2)	break;
		}
//===============================---2--Time Alarm Set============================================// 
	  //LCDRst();
		Clear1602 	 // 清屏  用于消去闹钟标志在没有指定的位置多显示
	  SubSw = 0;
	   while(PageF == 1)
	  {
		  TempAlarmBellLed(); // 温度报警函数
	  	while(0 == SubSw)
		{
			KeyEqu9( );
			KeyEqu2( );
	  		DisplayTimeAlarm();
			if(PageF != 1)	break;		
		}
	/************************************************************/
		while(1 == SubSw)
		{
			
			LcdSetXY(0,1);
			WriteC( FlashCur1602 , 1 );

		    if( KeyScanValue()==5||KeyScanValue()==1)
				TAOC = ~TAOC;
				/***********************************/
				delay1ms();
				WriteC( NOFlashCur1602 , 1 );
				
			 DisplayTimeAlarm();

			KeyEqu9( );
			KeyEqu2( );
			if(PageF != 1)	break;	
		}
		WriteC( NOFlashCur1602 , 1 );

		while(2 == SubSw)
		{
			LcdSetXY(4,1);
			WriteC( FlashCur1602 , 1 );
			
			TiAH = KeyData_Plus( TiAH );
 			TiAH = KeyData_Sub ( TiAH );
			if(TiAH > 23) 
				TiAH = 0;
			if(TiAH < 0)
				TiAH = 23;
		
			delay1ms();
			WriteC( NOFlashCur1602 , 1 );

		    DisplayTimeAlarm();

			KeyEqu9( );
			KeyEqu2( );
			if(PageF != 1)
			break;	
		}
		while(3 == SubSw)
		{
			LcdSetXY(7,1);
			WriteC( FlashCur1602 , 1 );
			
			TiAM = KeyData_Plus( TiAM );
 			TiAM = KeyData_Sub ( TiAM );
		  	 
			 if(TiAM > 59) 
				TiAM = 0;
			 if(TiAM < 0)
			    TiAM = 59;
		    delay1ms();
				WriteC( NOFlashCur1602 , 1 );

			DisplayTimeAlarm();

			KeyEqu9( );
			KeyEqu2( );
			if(PageF != 1)
			break;	
		}

		if(4 == SubSw)//这两条语句的作用是解决在这里按换屏键(2键,对应变量为PageF)无效
			break;	
	//上面的两行与下面的KeyEqu2()函数是一样的  	
		//KeyEqu2( );
		KeyEqu9( );
	
		if(PageF != 1)
			break;
	  }

//===============================---3--Temperature Alarm Set============================================// 
	    LCDRst();
	    SubSw = 0;
		//TempAlarmBellLed(); // 温度报警函数
	  while(PageF == 0)
	  {
		TempAlarmBellLed(); // 温度报警函数
		while(0 == SubSw)
		{
		 	TempAlarmBellLed(); // 温度报警函数
			KeyEqu9( );		
	  		DisplayTempAlarm();	
			KeyEqu2( );
			if(PageF != 0)
				break;
			break;		
		}

	  while(1 == SubSw)
		{	
			TempAlarmBellLed(); // 温度报警函数		
			LcdSetXY(3,1);
			WriteC( FlashCur1602 , 1 );
			
		    if( KeyScanValue()==5||KeyScanValue()==1)
				TeALF = ~TeALF;
		  
		    delay1ms();
			WriteC( NOFlashCur1602 , 1 );
		
	  		DisplayTempAlarm();	

			KeyEqu9( );	
			
			KeyEqu2( );
			if(PageF != 0)
				break;
			
		}

			while(2 == SubSw)
		{	
			TempAlarmBellLed(); // 温度报警函数			
			LcdSetXY(5,1);
			WriteC( FlashCur1602 , 1 );
			
		    TeAL = KeyData_Plus( TeAL );
 			TeAL = KeyData_Sub ( TeAL );
		  
		    delay1ms();
			WriteC( NOFlashCur1602 , 1 );
		
	  		DisplayTempAlarm();	

			KeyEqu9( );	
			
			KeyEqu2( );
			if(PageF != 0)
				break;
			
		}
		 
			while(3 == SubSw)
		{
			TempAlarmBellLed(); // 温度报警函数
			LcdSetXY(11,1);
			WriteC( FlashCur1602 , 1 );
			
		    if( KeyScanValue()==5||KeyScanValue()==1)
				TeAHF = ~TeAHF;
		  
		    delay1ms();
			WriteC( NOFlashCur1602 , 1 );
		
	  		DisplayTempAlarm();	

			KeyEqu9( );	
			KeyEqu2( );
			if(PageF != 0)
				break;
			
		}

			while(4 == SubSw)
		{	
			TempAlarmBellLed(); // 温度报警函数			
			LcdSetXY(13,1);
			WriteC( FlashCur1602 , 1 );
			
		    TeAH = KeyData_Plus( TeAH );
 			TeAH = KeyData_Sub ( TeAH );
		  
		    delay1ms();
			WriteC( NOFlashCur1602 , 1 );
		
	  		DisplayTempAlarm();	

			KeyEqu9( );	
			
			KeyEqu2( );
			if(PageF != 0)
				break;
			
		}
		if(SubSw == 5)
			break;	
	//	KeyEqu2( );		
		if(PageF != 0)
			break;
	  }
	 
	
  }
}




void SystemIni(void)
{
	LCDRst();
	DS1308_init();   // 硬件设计上存在失误:  没有加上拉电阻  所以如果不给写入那么会出现85这个值
	DS1302Ch[0] = ReadYs ();
	DS1302Ch[1] = ReadMn ();
	DS1302Ch[2] = ReadDay();
	DS1302Ch[3] = ReadHr ();
	DS1302Ch[4] = ReadMin();
	DS1302Ch[5] = ReadSec();
	DS1302Ch[6] = ReadWe ();

	LCDRst();

//	DS1302_Write_Ini_Data(10,12,31,23,59,55,2); //因为已经将写入和读出的值均转换成BCD码  所以直接给10进制的数就可以了

 	// 下面让18B20转换2次 第一个get_temperature是读出默认的+85 
	// 第二个是DS18B20完成第一次转换,更新温度值,
	// 避免温度报警的第一次判断错误(即TempAlarmBellLed函数第一次判断)
	get_temperature();
	get_temperature();
	
	Uart_Init(4800);
}

void LCD1602_Display_1302()
{
 
	DisC(0,0,ys/10+'0');
	DisC(1,0,ys%10+'0');
	
	DisC(3,0,mn/10+'0');
	DisC(4,0,mn%10+'0');
	
	DisC(6,0,day/10+'0');  
	DisC(7,0,day%10+'0');
	
	DisC(0,1,hr/10+'0');
	DisC(1,1,hr%10+'0');

	   	DisC(2,1,':');

	DisC(3,1,min/10+'0');
	DisC(4,1,min%10+'0');
		
		DisC(5,1,':');
	
	
	DisC(6,1,sec/10+'0');
	DisC(7,1,sec%10+'0');

	DisS(10,1,"Week:");		
	DisC(15,1,we+'0');
	
}



void LCD1602_Display_18B20()
{
	if(1 == f)  TemF = '-';
	if(0 == f)  TemF = '+';	

	DisC(10,0,TemF);
	DisC(11,0,tempint/10+'0');
	DisC(12,0,tempint%10+'0');
	DisC(13,0,'.');

	tempdf = (tempdf * 5 / 8); // 将温度的小数部分的0.00~0.15转换到0.0~0.9  
	DisC(14,0,tempdf+'0');
}

void DisplayTempTime(void)	// PageF == 0  时显示时间和温度
{

//	Clear1602 
//	delay1ms();
  /************************下面是自定义字符 存在问题**************************************/
    v_LcdWriteSelfFont_f( );

		LcdSetXY( 2,0 );  // 自定义字符要在1602显示其他东西的最前面
	v_LcdDisplaySelfFont_f( 0 );  	   //年
		LcdSetXY( 5,0 );
	v_LcdDisplaySelfFont_f( 1 );	  // 月
		LcdSetXY( 8,0 );
	v_LcdDisplaySelfFont_f( 2 );	  // 日
	
	if(TAOC)
		LcdSetXY( 8,1 );
	v_LcdDisplaySelfFont_f( 3 );	  //
		LcdSetXY( 15,0 );
	v_LcdDisplaySelfFont_f( 4 );	  //温度标志℃	

	 	read_DS1308();
	LCD1602_Display_1302();

	get_temperature();
	LCD1602_Display_18B20();

}


 void KeyEqu2( void )
 {
 	 if( KeyScanValue()==2)
  {
	  PageF++;	  
  }
  	 if(PageF == 3)
	 {
	 	PageF = 0;
	 }
 }


 void KeyEqu9( void )
 {
 	 if( KeyScanValue()==9)
  {
  	 SubSw++;
  }
  if(SubSw == 8)
	 {
	 	SubSw = 0;
	 }
	
 }

void KeyEqu1_Plus( uint8 SubSw )   //对应的时间加
 {
 	 if( KeyScanValue()==1)
  {
  	 (DS1302Ch[SubSw-1])++;
  }
//  if( DS1302Ch[i] == DS1302Limit[i])
//	 {
//	 	DS1302Ch[i] = 0;
//	 }
	
 }

 
void KeyEqu5_Sub( uint8 SubSw )	 //对应的时间减
 {
 	 if( KeyScanValue()==5)
  {
  	 (DS1302Ch[SubSw-1])--;
  }
 // if(YearCh == 0)
//	 {
//	 	YearCh = 0x20;
//	 }
	
 }

uint8 KeyData_Plus( uint8 Data )   //对应的变量加
 {
 	 if( KeyScanValue()==1)
  {
  	 ++Data;
  }

	 return Data;
 }

 
uint8 KeyData_Sub( uint8 Data )	 //对应的变量减
 {
 	 if( KeyScanValue()==5)
  {
  	 --Data;
  }
 	 return Data;
 }

 void DisplayTimeAlarm()
 {
 	
	  DisS(0,0,"1--TimeAlarm:");

//	  DisC(0,1,(char)(TAOC) +'0');
	  
	  if(TAOC)
	  {
	  	LcdSetXY(0,1);
	  	v_LcdDisplaySelfFont_f( 3 );
	  }
	  else
	  {
	   	//LcdSetXY(0,1);
		DisC(0,1,'C');
	  }

	  DisC(3,1,TiAH/10+'0');
	  DisC(4,1,TiAH%10+'0');

	  DisC(5,1,':');

	  DisC(6,1,TiAM/10+'0');
	  DisC(7,1,TiAM%10+'0');

 }

 
 void DisplayTempAlarm()
 {
 	
	  DisS(0,0,"2--TempLimit:");

	  DisC(0,1,'L');
	  DisC(1,1,':');
	  DisC(3,1, ((char)(TeALF)==0)?'-':'+');

	  DisC(4,1,TeAL/10+'0');
	  DisC(5,1,TeAL%10+'0');
//	  DisC(4,1,'.');
//	  DisC(5,1,TeALFloat);
//	  DisC(6,1,'C');
	  	LcdSetXY( 6,1 );
	v_LcdDisplaySelfFont_f( 4 );	  //温度标志℃

	  

	  DisC(8,1,'H');
	  DisC(9,1,':');
	  DisC(11,1, ((char)(TeAHF)==0)?'-':'+');

	  DisC(12,1,TeAH/10+'0');
	  DisC(13,1,TeAH%10+'0');
//	  DisC(12,1,'.');
//	  DisC(13,1,TeAHFloat);
//	  DisC(14,1,'C');
	  	LcdSetXY( 14,1 );
	v_LcdDisplaySelfFont_f( 4 );	  //温度标志℃

 }


void SetDs1302Display(uint8 i)
{


	KeyEqu1_Plus( i );
	KeyEqu5_Sub(  i );
	

	if(1 == SubSw)
		LcdSetXY(1, 0);
	if( SubSw==3 || SubSw==2 )
	{
		LcdSetXY(SubSw+SubSw+SubSw-2,0);  // 3*i-2
	}

	if(4 == SubSw)
		LcdSetXY(1, 1);
	if( SubSw==5 || SubSw==6 )
	{
		LcdSetXY( SubSw+SubSw+SubSw-11,1);  // 3*i-2
	}
	if(7==SubSw)
	{
		LcdSetXY(15,1);
	}
	 WriteC( FlashCur1602 , 1 );
}

void delay1ms()
{
	unsigned char i,j;
	for(i=0; i<100; i++)
		for(j=0; j<100; j++);
}

/*
void Bell(void)	 
{
	BELL = 0;
	delay1ms();
	BELL = 1;
}
*/


void TimeAlarmBell()
{
 	while( TAOC )
	 {
	 	if(min == TiAM &&  hr== TiAH)	 // min,hr
		{
//			BELL = 0;
			break;
		}
		else 
		{
			BELL  = 1;
			break;
		}
	 }
}



void TempAlarmBellLed() // 温度报警函数
{
 
	  //下面的四个判断 为上下对称两组 第一行是正的温度判断(标志位==1)  第二行是负温度的判断(标志位==0)//  
	 	if(      (( (~f == TeAHF) && (TeAHF == 1) ) && (tempint >= TeAH) ) 
		    || (( ( ~f == TeALF) && (TeALF == 1) ) && (tempint <= TeAL) ) 
	    	||	(( ( ~f ==TeAHF ) && (TeAHF == 0) ) && (tempint <= TeAH) )  
	    	|| (  ( ~f == TeALF && ( TeALF == 0) ) && (tempint >= TeAL) ) )
		{
		//	BELL = 0;
			LED  = 0;
	
		}
		else 
		{
		//	BELL  = 1;
			LED   = 1;
		}
	 
}

void JudgeMonthDays()  // 设置月份的最多天数
{
	if(3 == i)   //如果是Day  那么得区分是30 还是31 如果是二月那么还应该判断是平年(28 days)还是闰年(29 days)
							     //if((year%4==0&&year%100!=0)||(year%400==0)) {那么是闰年}	  
					{
						if(2 == ReadMn()) // 如果是二月
					 	{
							if( !(ReadYs()%4) )	  //读取年份 如果是闰年  这里没判断%100!=0 但是在20XX年这个条件就够了
								DS1302Limit[2] = 29;  
							else 
							   	DS1302Limit[2] = 28;  
						}
						else // 如果不是二月
							{
								if(4 == ReadMn() || 6 == ReadMn() || 9 == ReadMn() || 11 == ReadMn())  // 如果月份是 4 6 9 11 那么是30days
									DS1302Limit[2] = 30;
								else 
									DS1302Limit[2] = 31;	//剩下的月份是31days
							}
					 }
}

// 计算2000～2099年任一天星期几
// year    : 00-99
// month: 01-12
// day     : 01-31
uint8 GetWeekFromDay(unsigned char year,unsigned char month,unsigned char day)
{
    if( month == 1 || month == 2 )  
    {
        month += 12;
        if( year> 0 )
            year--;
        else
            year = 4;
    }

    // 返回星期几(星期一用1表示，而星期天用7表示)
    return 1+(( day + 2*month + 3*(month+1)/5 + year + year/4 ) %7);
}  