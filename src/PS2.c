#include<reg52.h>
#include "MacroAndConst.h"
#include "PS2.h"

#define SHIFT 0x12

/*
╭════════════════════════════════════╮
║                                                                        ║
║                      硬  件  信  号  定  义                            ║
║                                                                        ║
╰════════════════════════════════════╯
*/

sbit io_dat_ps2key = P3^5;                  //PS2键盘的数据端口
sbit io_clk_ps2key = P3^4;                  //PS2键盘的时钟端口

/*
╭════════════════════════════════════╮
║                                                                        ║
║                      键  码  转  换  数  组                            ║
║                                                                        ║
╰════════════════════════════════════╯
*/
//没有按下Shift键时的键码转换
static unsigned char code UnShifted[59][2] = {
									          0x1C, 'a',0x32, 'b',0x21, 'c',0x23, 'd',0x24, 'e',
                                              0x2B, 'f',0x34, 'g',0x33, 'h',0x43, 'i',0x3B, 'j',
                                              0x42, 'k',0x4B, 'l',0x3A, 'm',0x31, 'n',0x44, 'o',
                                              0x4D, 'p',0x15, 'q',0x2D, 'r',0x1B, 's',0x2C, 't',
                                              0x3C, 'u',0x2A, 'v',0x1D, 'w',0x22, 'x',0x35, 'y',
                                              0x1A, 'z',0x45, '0',0x16, '1',0x1E, '2',0x26, '3',
                                              0x25, '4',0x2E, '5',0x36, '6',0x3D, '7',0x3E, '8',
											  0x46, '9',0x0E, '`',0x4E, '-',0x55, '=',0x5D, '\\',
                                              0x29, ' ',0x54, '[',0x5B, ']',0x4C, ';',0x52, '\'',
                                              0x41, ',',0x49, '.',0x4A, '/',0x71, '.',0x70, '0',
                                              0x69, '1',0x72, '2',0x7A, '3',0x6B, '4',0x73, '5',
											  0x74, '6',0x6C, '7',0x75, '8',0x7D, '9',
											  };
//按下Shift键时的键码转换
/*static unsigned char code Shifted[59][2] =  {												
											  0x1C, 'A',0x32, 'B',0x21, 'C',0x23, 'D',0x24, 'E',
                                              0x2B, 'F',0x34, 'G',0x33, 'H',0x43, 'I',0x3B, 'J',
                                              0x42, 'K',0x4B, 'L',0x3A, 'M',0x31, 'N',0x44, 'O',
                                              0x4D, 'P',0x15, 'Q',0x2D, 'R',0x1B, 'S',0x2C, 'T',
                                              0x3C, 'U',0x2A, 'V',0x1D, 'W',0x22, 'X',0x35, 'Y',
                                              0x1A, 'Z',0x45, '0',0x16, '1',0x1E, '2',0x26, '3',
                                              0x25, '4',0x2E, '5',0x36, '6',0x3D, '7',0x3E, '8',
                                              0x46, '9',0x0E, '~',0x4E, '_',0x55, '+',0x5D, '|',
                                              0x29, ' ',0x54, '{',0x5B, '}',0x4C, ':',0x52, '"',
                                              0x41, '<',0x49, '>',0x4A, '?',0x71, '.',0x70, '0',
                                              0x69, '1',0x72, '2',0x7A, '3',0x6B, '4',0x73, '5',
											  0x74, '6',0x6C, '7',0x75, '8',0x7D, '9',
											  };
/*
╭════════════════════════════════════╮
║                                                                        ║
║                      函  数  原  型  声  明                            ║
║                                                                        ║
╰════════════════════════════════════╯
*/
static uchar ReceiveByte(void);
static void SkipCode(void);
static uchar ReceiveDat(void);              //接收一个正常键
static uchar ChangeASCII(uchar dat);        //将接通码转为简码的ASCII码
uchar ReceiveDatFromPS2KeyBoard(void);      //返回相应按键的ASCII码
/*
╭════════════════════════════════════╮
║  函 数 名： static uchar ReceiveByte(void)                             ║
║  功能模式： 内部                                                       ║
║  函数说明： 返回键盘传送的一个字节.                                    ║
║  调用函数： 无                                                         ║
║  全局变量： 无                                                         ║
║  端口信号:  io_clk_ps2key、io_dat_ps2key                               ║
║  输    入： 无                                                         ║
║  返    回： 11位数据中的8位数据.                                       ║
║  设 计 者： 韩林                  日    期：2007-02-09                 ║
║  修 改 者： 韩林                  日    期：2007-02-09                 ║
║  E-mail  ： hanlinxp@163.com      QQ： 33737559                        ║
║  版    本： v1.0                                                       ║
║                                                                        ║
║                      Copyright@2006 - 2007                             ║
║                                                                        ║
╰════════════════════════════════════╯
*/
static uchar ReceiveByte(void)
{
    uchar i,dat = 0;
    while(!io_clk_ps2key);          //起始位0
    while(io_clk_ps2key);
    for(i=0;i<8;i++)                //接收8位数据
    {
        dat >>= 1;
        if(io_dat_ps2key)
            dat |= 0x80;
        while(!io_clk_ps2key);
        while(io_clk_ps2key);
    }
                                    //此处取出奇校验位
    while(!io_clk_ps2key);
    while(io_clk_ps2key);
    while(!io_clk_ps2key);
    return dat;
}
/*
╭════════════════════════════════════╮
║  函 数 名： static void SkipCode(void)                                 ║
║  功能模式： 内部                                                       ║
║  函数说明： 将其余编码跳过.                                            ║
║  调用函数： 无                                                         ║
║  全局变量： 无                                                         ║
║  端口信号:  io_clk_ps2key、io_dat_ps2key                               ║
║  输    入： 无                                                         ║
║  返    回： 无                                                         ║
║  设 计 者： 韩林                  日    期：2007-02-09                 ║
║  修 改 者： 韩林                  日    期：2007-02-09                 ║
║  E-mail  ： hanlinxp@163.com      QQ： 33737559                        ║
║  版    本： v1.0                                                       ║
║                                                                        ║
║                      Copyright@2006 - 2007                             ║
║                                                                        ║
╰════════════════════════════════════╯
*/
static void SkipCode(void)
{
    (void)ReceiveByte();
    (void)ReceiveByte();
}
/*
╭════════════════════════════════════╮
║  函 数 名： static uchar ReceiveDat(void)                              ║
║  功能模式： 内部                                                       ║
║  函数说明： 返回键盘按键对应的一个接通码.                              ║
║  调用函数： ReceiveByte()、SkipCode()                                  ║
║  全局变量： 无                                                         ║
║  端口信号:  无                                                         ║
║  输    入： 无                                                         ║
║  返    回： 键盘的接通码.                                              ║
║  设 计 者： 韩林                  日    期：2007-02-08                 ║
║  修 改 者： 韩林                  日    期：2007-02-09                 ║
║  E-mail  ： hanlinxp@163.com      QQ： 33737559                        ║
║  版    本： v1.1                                                       ║
║                                                                        ║
║                      Copyright@2006 - 2007                             ║
║                                                                        ║
╰════════════════════════════════════╯
*/
static uchar ReceiveDat(void)
{
    uchar dat;
    if(io_clk_ps2key)                       //如果无键按下,返回0
        return 0;
    dat = ReceiveByte();
    SkipCode();
    return dat;                             //返回接通码
}
/*
╭════════════════════════════════════╮
║  函 数 名： static uchar ChangeASCII(uchar dat)                        ║
║  功能模式： 内部                                                       ║
║  函数说明： 将接通码转为与键盘按键对应的ASCII码.                       ║
║  调用函数： 无                                                         ║
║  全局变量： 无                                                         ║
║  端口信号:  无                                                         ║
║  输    入： 无                                                         ║
║  返    回： 键盘按键的ASCII码.                                         ║
║  设 计 者： 韩林                  日    期：2007-02-08                 ║
║  修 改 者： 韩林                  日    期：2007-02-08                 ║
║  E-mail  ： hanlinxp@163.com      QQ： 33737559                        ║
║  版    本： v1.0                                                       ║
║                                                                        ║
║                      Copyright@2006 - 2007                             ║
║                                                                        ║
╰════════════════════════════════════╯
*/
static uchar ChangeASCII(uchar dat)
{
    uchar i;
    if(dat == 0)
        return 0;
    for(i=0;i<59;i++)
    {
        if(UnShifted[i][0] == dat)          //没按Shift键
            return UnShifted[i][1];
    }
    return 0;
}
/*
╭════════════════════════════════════╮
║  函 数 名： uchar ReceiveDatFromPS2KeyBoard(void)                      ║
║  功能模式： 全局                                                       ║
║  函数说明： 返回键盘按键的ASCII码,没有按键则为0.                       ║
║  调用函数： ChangeASCII()、ReceiveDat()                                ║
║  全局变量： 无                                                         ║
║  端口信号:  无                                                         ║
║  输    入： 无                                                         ║
║  返    回： 键盘按键的ASCII码.                                         ║
║  设 计 者： 韩林                  日    期：2007-02-08                 ║
║  修 改 者： 韩林                  日    期：2007-02-08                 ║
║  E-mail  ： hanlinxp@163.com      QQ： 33737559                        ║
║  版    本： v1.0                                                       ║
║                                                                        ║
║                      Copyright@2006 - 2007                             ║
║                                                                        ║
╰════════════════════════════════════╯
*/
uchar ReceiveDatFromPS2KeyBoard(void)
{
    return ChangeASCII(ReceiveDat());
}
