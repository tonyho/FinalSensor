#ifndef _MACRO_AND_CONST_H_
#define _MACRO_AND_CONST_H_
/*------------------* 
*   常 数 宏 定 义  * 
*------------------*/ 
#ifndef INTERRUPT_TIMER0_OVERFLOW 
	#define INTERRUPT_TIMER0_OVERFLOW 1
#endif

#ifndef INTERRUPT_TIMER1_OVERFLOW 
	#define INTERRUPT_TIMER1_OVERFLOW 3
#endif

#ifndef INTERRUPT_TIMER2_OVERFLOW 
	#define INTERRUPT_TIMER2_OVERFLOW 5
#endif

#ifndef FALSE 
    # define FALSE   0x00 
#endif 

#ifndef TRUE 
    # define TRUE    (!FALSE) 
#endif 

#ifndef HIGH 
    # define HIGH    0x01 
#endif 

#ifndef LOW 
    # define LOW     0x00 
#endif 

#ifndef OFF
	#define OFF		0xff
#endif

#ifndef ON
	#define ON		( !OFF )
#endif

#ifndef	RETURN_NORMAL
	#define RETURN_NORMAL (bit)0
#endif

#ifndef	RETURN_ERROR
	#define RETURN_ERROR  (bit)1
#endif

#ifndef PI 
    #define PI      3.1415926535897932f 
#endif 

#define K_D2R   (PI/180.0) 
#define K_R2D   (180.0/PI)            

/*------------------* 
*   动 作 宏 定 义  * 
*------------------*/ 
#define SIN(x)  sin((float)x * K_D2R) 
#define COS(x)  cos((float)x * K_D2R) 
#define TAN(x)  tan((float)x * K_D2R) 
#define ATAN(x) (K_R2D*atan(x)) 
#define ASIN(x) (K_R2D*asin(x)) 
#define ACOS(x) (K_R2D*acos(x)) 

#ifndef NULL 
    # define NULL    0 
#endif 

#define BIT(n) (1 << (n))

#define SET(Reg,n)    Reg |= BIT(n); 
#define CLR(Reg,n)    Reg &= ~BIT(n); 

#define MAX(a,b)  (((a) > (b)) ? (a) : (b)) 
#define MIN(a,b)  (((a) < (b)) ? (a) : (b)) 

#define UBOUND(a) (sizeof(a)/sizeof(a[0])) 

#define SAFE_CODE_PERFORMANCE(Code) {\ 
										UINT8 cFlags = (SREG & BIT(7));\ 
										CLI();\ 
										Code;\ 
										SREG |= cFlags;\ 
									} 
#define EXIT_SAFE_CODE                  SREG |= cflags; 

#define TYPE_CONVERSION(Addr,Type)     (*((Type *)(Addr))) 

#define ABS(Number)    (((Number) < 0)?(-(Number)):(Number)) 

#define SELECT_HEX(Value,n,Type)       ((Value << (n) * 4) >> ((sizeof(Type) * 8) - 4)) 

#define ABS_MOD(x,y)                   ((((x) % (y)) + (y)) % (y)) 

#define _ADD_STR(a,b)      a##b 
#define ADD_STR(a,b)       _ADD_STR(a,b) 


/******************** 
*  用户变量类型定义 * 
********************/ 
typedef unsigned int uint16; 
typedef unsigned int UINT; 
typedef unsigned int uint; 
typedef unsigned int UINT16; 
typedef unsigned int WORD; 
typedef unsigned int word; 
typedef int          int16; 
typedef int          INT16; 

typedef unsigned long uint32; 
typedef unsigned long UINT32; 
typedef unsigned long DWORD; 
typedef unsigned long dword; 
typedef long          int32; 
typedef long          INT32; 

typedef signed   char int8; 
typedef signed   char INT8; 
typedef unsigned char byte; 
typedef unsigned char BYTE; 
typedef unsigned char uchar;
typedef unsigned char UINT8; 
typedef unsigned char uint8; 
typedef unsigned char BOOL; 

#endif 
