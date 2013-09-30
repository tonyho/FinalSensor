#ifndef __I2C_H__
#define __I2C_H__

//---------------------------------------------------------------
#define I2C_SCL_0 	I2C_SCL = 0
#define I2C_SCL_1 	I2C_SCL = 1
#define I2C_SDA_0 	I2C_SDA = 0
#define I2C_SDA_1 	I2C_SDA = 1
//--------------------管脚定义-----------------------------------
sbit I2C_SCL = P2^6;             // I2C总线的时钟信号
sbit I2C_SDA = P2^7;             // I2C总线的数据信号
//---------------------------------------------------------------                                   

#define TRUE        1
#define FALSE       0

#define DELAY_nT    2           // 延时6个机器周期(2+2*DELAY_nT),
                                // (延时时间大于5us,因此机器个数
                                // 根据晶振频率而定)

//***********************************************************************
// 函数名称：I2C_Start()
// 功    能：启动I2C总线
// 入口参数：无
// 出口参数：无
//***********************************************************************
void I2C_Start(void)
{
	unsigned char t;
	I2C_SDA_1;
	I2C_SCL_1; 
	t = DELAY_nT;	while(--t);	 // 大于5us
	I2C_SDA_0; 
	t = DELAY_nT;	while(--t);	 // 大于4us
	I2C_SCL_0;
//	t = DELAY_nT;	while(--t);	 // 大于5us
}

//***********************************************************************
// 函数名称：I2C_Stop()
// 功    能：终止I2C总线
// 入口参数：无
// 出口参数：无
//***********************************************************************
void I2C_Stop(void)
{
	unsigned char t;
	I2C_SDA_0;
	I2C_SCL_1;
	t = DELAY_nT;	while(--t);		// 大于4us
	I2C_SDA_1;
	t = DELAY_nT;	while(--t);		// 大于5us
	I2C_SCL_0;
//	t = DELAY_nT;	while(--t);	 	// 大于5us
}

//***********************************************************************
// 函数名称：I2C_Send_Bit_0()
// 功    能：发送比特0
// 入口参数：无
// 出口参数：无
//***********************************************************************
void I2C_Send_Bit_0(void)
{
	unsigned char t;
	I2C_SDA_0;
	I2C_SCL_1;
	t = DELAY_nT;	while(--t);		// tL = 6us 
	I2C_SCL_0;
//	t = DELAY_nT;	while(--t);	 	// 大于5us
}

//***********************************************************************
// 函数名称：I2C_Send_Bit_1()
// 功    能：发送比特1
// 入口参数：无
// 出口参数：无
//***********************************************************************
void I2C_Send_Bit_1(void)
{
	unsigned char t;
	I2C_SDA_1;
	I2C_SCL_1;
	t = DELAY_nT;	while(--t);	
	I2C_SCL_0;
//	t = DELAY_nT;	while(--t);	 // 大于5us
}

//***********************************************************************
// 函数名称：I2C_Check_Ack()
// 功    能：发送完一个字节后检验设备的应答信号
// 入口参数：无
// 出口参数：返回值为True，成功
//           返回值为False，失败
//***********************************************************************
unsigned char I2C_Check_Ack(void)
{
	unsigned char t;
	I2C_SDA_1;
	I2C_SCL_1;
	t = DELAY_nT;	while(--t);
	F0 = I2C_SDA;
	t = DELAY_nT;	while(--t);
	I2C_SCL_0;
	t = DELAY_nT;	while(--t);
	if( F0 == 1 )  return FALSE;
	else 		  		return TRUE;
}

//***********************************************************************
// 函数名称：I2C_Write8Bit()
// 功    能：向I2C总线写入8bit数据
// 入口参数：I2C_data     将要写入I2C总线的8bit数据 
// 出口参数：无
//***********************************************************************
void I2C_Write8Bit(unsigned char I2C_data)
{
	unsigned char i;
	
	for( i=0;i<8;i++ )
	{
		if( (I2C_data<<i)&0x80 )
	    	I2C_Send_Bit_1();
	  	else
	    	I2C_Send_Bit_0();
	}
}

//***********************************************************************
// 函数名称：I2C_Read8Bit()
// 功    能：从I2C总线接收8bit数据
// 入口参数：无
// 出口参数：返回值为从I2C总线上接收到的8bit数据
//***********************************************************************
unsigned char I2C_Read8Bit(void)
{
	unsigned char t,I2C_data = 0,i;
	
	for( i = 0; i < 8; i++ )
	{	
		I2C_data = I2C_data << 1;
		I2C_SDA_1;    
		I2C_SCL_1;    
		t = DELAY_nT;	while(--t);
		F0 = I2C_SDA;
		t = DELAY_nT;	while(--t);
		I2C_SCL_0;
		if( F0 == 1 )	
			I2C_data = I2C_data | 0x01;
	}
	return I2C_data;
}

#endif