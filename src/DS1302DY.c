#include<reg52.h>
#include<intrins.h>
#include "DS1302DY.h"


 //DS1302_Write_Ini_Data(0xa,0x9,0x29,0x23,0x59,0x55,0x7);
//  DS1302_Write_Ini_Data(0x99,0x12,0x31,0x23,0x59,0x55,0x7);

unsigned char sec,min,hr,day,we,mn,ys;

sbit clk=P2^3;
sbit dat=P2^4;
sbit rst=P2^5;

sbit A0=ACC^0;
sbit A1=ACC^1;
sbit A2=ACC^2;      
sbit A3=ACC^3;
sbit A4=ACC^4;
sbit A5=ACC^5;
sbit A6=ACC^6;
sbit A7=ACC^7;

//-------------------------------------------------------
void InputByte(unsigned char dd)        //向ds1302 写一个字节数据
   {
     unsigned char i;
     ACC=dd;
     for(i=8;i>0;i--)
      {
       dat=A0;
       clk=1;
       clk=0;
       ACC=ACC>>=1;
        }
   }  
//-------------------------------------------------------
unsigned char OutputByte(void)      //ds1302 读取一个字节的数据
   {
     unsigned char i;
     dat=1;      
     for(i=8;i>0;i--)
      {
        ACC=ACC>>1;
        A7=dat;
        clk=1;
        clk=0;
        }      
      return(ACC);
     }
//------------------------------------------------------------------------
void Write(unsigned char addr,unsigned char num)	  //向ds1302 指定地址写一个字节数据
  {
    rst=0;
    clk=0;
    rst=1;
    InputByte(addr);  
    InputByte(num);    
    clk=1;
    rst=0;
    }
//---------------------------------------------------------------------
unsigned char Read(unsigned char addr)			    //从ds1302指定地址读取一个字节的数据
  {
    unsigned char dd=0;
    rst=0;
    clk=0;
    rst=1;
    InputByte(addr);
    dd=OutputByte();
    clk=1;
    rst=0;
    return(dd);
   }
//--------------------------------------------------------------
unsigned char ReadSec()
  {
   unsigned char dd; 
   dd=Read(0x81);
 //  dd=(0xff & (dd>>4))*10 +(0xf & dd);    // Del
 	dd = dd/16*10 + dd % 16; 	 //Add     这个是将BCD码转换成十进制  因为DS1302中保存的值是BCD码形式
   return(dd);
   }
//--------------------------------------------------------------
unsigned char ReadMin()
  {
   unsigned char dd; 
   dd=Read(0x83);
   	dd = dd/16*10 + dd % 16; 	 //Add
   return(dd);
    }       
//--------------------------------------------------------------
unsigned char ReadHr()
  {
    unsigned char dd; 
    dd=Read(0x85);
	dd = dd/16*10 + dd % 16;    //Add
    return(dd);
   }
//------------------------------------------------------------   
unsigned char ReadWe()
  {
    unsigned char dd; 
    dd=Read(0x8b);
		//dd = dd/16*10 + dd % 16; 	 //Add
    return(dd);
    }
//------------------------------------------------------------
unsigned char ReadDay()
  {
    unsigned char dd; 
    dd=Read(0x87);
	
	dd = dd/16*10 + dd % 16; 	 //Add
		
    return(dd);
   }
//------------------------------------------------------------
unsigned char ReadMn()
  {
    unsigned char dd; 
    dd=Read(0x89);
	dd=(0xff & (dd>>4))*10 +(0xf & dd);
    return(dd);
   }
//------------------------------------------------------------
unsigned char ReadYs()
  {
    unsigned char dd; 
    dd=Read(0x8d);
	dd=(0xff & (dd>>4))*10 +(0xf & dd);
    return(dd);
   }
//------------------------------------------------------------
void WriteSec(unsigned char num)
      {
	   num = num / 10 *16 + num % 10;   // Add	  这个是将十进制数字转换BCD码 再写入 因为DS1302中保存的值是BCD码形式
       Write(0x80,num);
       }
//-----------------------------------------------------------
void WriteMin(unsigned char num)
  {
   num = num / 10 *16 + num % 10;   // Add
    Write(0x82,num);
   }
//-----------------------------------------------------------
void WriteHr(unsigned char num)
  {
   num = num / 10 *16 + num % 10;   // Add
    Write(0x84,num);
   }
//-----------------------------------------------------------
void WriteDay(unsigned char num)
 {
  num = num / 10 *16 + num % 10;   // Add
   Write(0x86,num);
  }
//-----------------------------------------------------------
void WriteMn(unsigned char num)
  {
   num = num / 10 *16 + num % 10;   // Add
    Write(0x88,num);
   }
//-----------------------------------------------------------
void WriteWe(unsigned char num)
 {
 // num = num / 10 *16 + num % 10;   // Add
   Write(0x8a,num);
   }
//-----------------------------------------------------------
void WriteYs(unsigned char num)
  {
   num = num / 10 *16 + num % 10;   // Add
    Write(0x8c,num);
   }
//-----------------------------------------------------------
void DisableWR(void)        //write data,read 0
 {
 
   Write(0x8e,0x00);
   }
//----------------------------------------------------------
void EnableWR(void)              //can''d write,read data   
 {
   Write(0x85,0x80);
  }

 void 	DS1308_init()
 {
 	DisableWR();
	EnableWR();         
    WriteSec(50);			     //写入时间和日期
    WriteMin(59);
    WriteHr(23);
    WriteDay(31);
    WriteWe(12);
    WriteMn(10);  
    WriteYs(10);
 }
 void DS1302_Write_Ini_Data(yea,mon,day,hou,min,sec,wee)
 {
 	DisableWR();
	EnableWR();  

	WriteSec(sec);
	WriteMn(mon);
	WriteWe(wee);
	WriteYs(yea);
	WriteDay(day);
	WriteHr(hou);
	WriteMin(min);
 }

 void  read_DS1308()
 {
    sec=ReadSec();			 	 //读取时间和日期
    min=ReadMin();
    hr= ReadHr();
    day=ReadDay();
    we= ReadWe();
    mn= ReadMn();
    ys= ReadYs();
 
 }