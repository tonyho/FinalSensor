#include<reg52.h>
#include "MacroAndConst.h"
#include "18B20.h"

sbit TMDAT=P3^6;    //DS1820温度接口
bit f;             //负温度标志
bit flag;          //DS1820初始化成功标志
unsigned char tempint,tempdf;	          //温度整数部分和小数部分

void set_ds1820()              //初始化DS1820
{
	while(1)
	{
		uchar delay;
		flag=0;
		TMDAT=1;                  //总线高电平
		delay=1;
		while(--delay);           //稍微延时
		TMDAT=0;                  //总线拉低
		delay=250;
		while(--delay);           //延时500us
		TMDAT=1;                  //拉高总线
		delay=30;
		while(--delay);           //延时60us
		while(!TMDAT)             //当DS1820拉低总线
		{
			delay=120;  
			while(--delay);        //延时240us
			if(TMDAT)
			{
				flag=1;             //DS1820初始化成功标志
				break;
			}
		}
		if(flag)                  //初始化成功后再延时480us,时序要求
		{
			delay=240;
			while(--delay);
			break;
		}
	}
}

void write_ds1820(uchar command)  //写8位
{
	uchar delay,i;
	for(i=8;i>0;i--)               //循环8次写入
	{
		TMDAT=0;                    //拉低总线
		delay=6;
		while(--delay);             //延时12us
		TMDAT=command&0x01;         //将数据放在总线上，进行采样
		delay=25;
		while(--delay);             //延时50us，采样完毕
		command=command>>1;         //数据右移一位，准备下次数据
		TMDAT=1;                    //释放总线
	}
}

void read_ds1820()                //读
{
	uchar delay,i,j,k,temp,temph,templ;
	j=2;                           //读2位字节数据
	do
	{
		for(i=8;i>0;i--)            //一个字节分8位读取
		{
			temp>>=1;                //读取1位右移1位
			TMDAT=0;                 //数据线置低电平
			delay=1;
			while(--delay);          //延时2us
			TMDAT=1;                 //拉高总线
			delay=4;
			while(--delay);          //延时8us
			if(TMDAT)temp|=0x80;     //读取1位数据
			delay=25;
			while(--delay);          //读取1位数据后延时50us
		}
		if(j==2)templ=temp;         //读取的第一字节存templ
		else temph=temp;            //读取的第二字节存temph
	}while(--j);
	f=0;                           //初始温度标志为正
	if((temph&0xf8)!=0x00)         //若温度为负的处理，对二进制补码的处理
	{
		f=1;                        //为负温度f置1
		temph=~temph;
		templ=~templ;
		k=templ+1;
		templ=k;
		if(k>255)
		{
			temph++;
		}
	}
	tempdf=templ&0x0f;            //将读取的数据转换成温度值，整数部分存tempint,小数部分存tempdf
	templ>>=4;
	temph<<=4;
	tempint=temph|templ;
}

void get_temperature()           //温度转换、获得温度子程序
{
	set_ds1820();                 //初始化DS18B20
	write_ds1820(0xcc);           //发跳过ROM匹配命令
	write_ds1820(0x44);           //发温度转换命令
	set_ds1820();                 //初始化DS18B20
	write_ds1820(0xcc);           //发跳过ROM匹配命令
	write_ds1820(0xbe);           //发出读温度命令
	read_ds1820();                //将读出的温度数据保存到tempint和tempdf处
}