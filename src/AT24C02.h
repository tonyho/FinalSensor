#include "IIc.h"

#define AT24C02_DeviceAdd_W 0xa0	//器件地址(写)
#define AT24C02_DeviceAdd_R 0xa1	//器件地址(读)

#define I2C_R_BIT_SUCCESS     0x01
#define I2C_R_BIT_ERR         0x02
#define I2C_W_BIT_SUCCESS     0x04
#define I2C_W_BIT_ERR         0x08


unsigned char AT24C02_flag;

//**************************************************************************
// 函数名称：AT24C02_WriteByte()
// 功    能：向AT24C02指定地址上写入指定个字节数据
// 入口参数：add   AT24C02内的地址
//           Num   写入的数据个数（小于16）
//           Byte  将要写入AT24C02的节数据地址
// 出口参数：无
// 注    意：add和Num必须满足: add>=16*n且(add+Num)<16*(n+1))
//           即写入的数据都必须保证在同一页内(一页16字节)，本函数
//				 没有翻页功能。	
//**************************************************************************
void AT24C02_WriteByte( unsigned char add,
			 					unsigned char Num,
                        unsigned char *Byte)
{
	unsigned char i;

	I2C_Start();										// 启动I2C总线
	I2C_Write8Bit(AT24C02_DeviceAdd_W);			// 发送器件地址(写)
	AT24C02_flag = I2C_W_BIT_SUCCESS;			// 标志位
	if( I2C_Check_Ack() == 0 )						// 检测从机应答
		AT24C02_flag = I2C_W_BIT_ERR;				// 无应答，写错误
	I2C_Write8Bit(add);   							// 发送将要处理的数据的地址
	if( I2C_Check_Ack() == 0 )                // 检测从机应答
		AT24C02_flag = I2C_W_BIT_ERR;				// 无应答，写错误
	
	for( i=0; i<Num; i++ )
	{
		I2C_Write8Bit(Byte[i]);                // 写入数据 
		if( I2C_Check_Ack() == 0 )             // 检测从机应答
			AT24C02_flag = I2C_W_BIT_ERR;	
	}
	
	I2C_Stop();				        					// 停止I2C总线
}

//**************************************************************************
// 函数名称：AT24C02_ReadnByte()
// 功    能：从AT24C02指定地址上读出指定字节数据
// 入口参数：add   AT24C02内的地址
//           Num   需要读取的数据个数
//           *stor 读取数据后的存储地址
// 出口参数：无
//**************************************************************************
/*void AT24C02_ReadnByte(unsigned int add,
                       unsigned int Num,
                       unsigned char *stor)
{    
	unsigned int  i;
	
	I2C_Start();										// 启动I2C总线
	
	I2C_Write8Bit(AT24C02_DeviceAdd_W);			// 发送器件地址(写)
	AT24C02_flag = I2C_R_BIT_SUCCESS;
	if( I2C_Check_Ack() == 0 )                // 检测从机应答
		AT24C02_flag = I2C_R_BIT_ERR;	
	I2C_Write8Bit(add);   							// 发送将要处理的数据的地址(MSB)
	if( I2C_Check_Ack() == 0 )                // 检测从机应答
		AT24C02_flag = I2C_R_BIT_ERR;	
	
	I2C_Start();				         			// 再次启动IIC总线
	I2C_Write8Bit(AT24C02_DeviceAdd_R);	   	// 发送器件地址(读)
	if( I2C_Check_Ack() == 0 )                // 检测从机应答
		AT24C02_flag = I2C_R_BIT_ERR;	  
	stor[0] = I2C_Read8Bit();		        		// 读取第一个数据  
	for( i=1; i<Num; i++ )
	{
		I2C_Send_Bit_0();                      // 主机应答(ACK)，继续接收数据
		stor[i] = I2C_Read8Bit();		         // 读取数据
	}
	I2C_Send_Bit_1();				 					// 产生NO ACK信号结束读取数据			
	I2C_Stop();				        					// 停止I2C总线
}		*/