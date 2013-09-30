#include <reg52.h>
#include "MaroAndConst.h"
#include "KeyBoard.h"

#define P0 = KeyPadData;

bit flag=0; 

void Delay10ms()
{
	uint8 a,b;
	for(b=249;b>0;b--)
		for(a=17;a>0;a--);
}

uint8 ScanKeyPad()
{
    KeyPadData = 0xf0;
    
    if((KeyPadData&0xf0)!=0xf0)
    {
	    Delay10ms();
	    if((KeyPadData&0xf0)!=0xf0)
	    {
	    	flag =1;
	    	KeyPadData=0xfe;
		    switch(KeyPadData&0xf0)
		    {
			    case 0xe0:while((KeyPadData&0xf0)!=0xf0);return(1);
			    case 0xd0:while((KeyPadData&0xf0)!=0xf0);return(4);
			    case 0xb0:while((KeyPadData&0xf0)!=0xf0);return(7);
			    case 0x70:while((KeyPadData&0xf0)!=0xf0);return('*');
			    default:  break;
		    }
		    KeyPadData=0xfd;
		    switch(KeyPadData&0xf0)
		    {
			    case 0xe0:while((KeyPadData&0xf0)!=0xf0);return(2);
			    case 0xd0:while((KeyPadData&0xf0)!=0xf0);return(5);
			    case 0xb0:while((KeyPadData&0xf0)!=0xf0);return(8);
			    case 0x70:while((KeyPadData&0xf0)!=0xf0);return(0);
			    default:  break;
		    }
		    KeyPadData=0xfb;
		    switch(KeyPadData&0xf0)
		    {
			    case 0xe0:while((KeyPadData&0xf0)!=0xf0);return(3);
			    case 0xd0:while((KeyPadData&0xf0)!=0xf0);return(6);
			    case 0xb0:while((KeyPadData&0xf0)!=0xf0);return(9);
			    case 0x70:while((KeyPadData&0xf0)!=0xf0);return('#');
			    default:  break;
		    }
	
	    }
	  }
} 