#include <reg52.h>
#include "MacroAndConst.h"
#include "key44y.h"

#define KEYPORT  P0

uint8 KeyVal = 0;

void DelayForKey(uint8 xms)
{
 	 uint8 i,j;
	 for(i = xms; i > 0;i-- )
	     for(j = 110; j > 0; j-- );		 
}


uint8 KeyScanValue(void)
{
	 char KeyValTemp=0;
/*==================第一排S1~S4=====================*/
	 KEYPORT = 0xfe;
	 KeyVal  = KEYPORT;
	 KeyVal  = KeyVal&0xf0;
	 if(KeyVal != 0xf0)
	 {
	    DelayForKey(50);
		KeyVal = KEYPORT;
		switch(KeyVal)
		{	
			case 0xee:
		   	     KeyValTemp = 1;
		       	 break;

			case 0xde:
				 KeyValTemp = 2;
			     break; 	
				 
			 case 0xbe:
		   	     KeyValTemp = 3;
		       	 break;

			case 0x7e:
				 KeyValTemp = 4;
			     break; 		

			default:
				 KeyValTemp = 0;
		}
	 }

/*==================第二排S5~S8=====================*/
	 	 KEYPORT = 0xfd;
	 KeyVal  = KEYPORT;
	 KeyVal  = KeyVal&0xf0;
	 if(KeyVal != 0xf0)
	 {
	    DelayForKey(100);
		KeyVal = KEYPORT;
		switch(KeyVal)
		{	
			case 0xed:
		   	     KeyValTemp = 5;
		       	 break;

			case 0xdd:
				 KeyValTemp = 6;
			     break; 	
				 
			case 0xbd:
		   	     KeyValTemp = 7;
		       	 break;

			case 0x7d:
				 KeyValTemp = 8;
			     break; 		

			default:
				 KeyValTemp = 0;
		}
	 }

/*==================第三排S9~S12=====================*/
	 	 KEYPORT = 0xfb;
	 KeyVal  = KEYPORT;
	 KeyVal  = KeyVal&0xf0;
	 if(KeyVal != 0xf0)
	 {
	    DelayForKey(100);
		KeyVal = KEYPORT;
		switch(KeyVal)
		{	
			case 0xeb:
		   	     KeyValTemp = 10;
		       	 break;

			case 0xdb:
				 KeyValTemp = 9;
			     break; 	
				 
			 case 0xbb:
		   	     KeyValTemp = 11;
		       	 break;

			case 0x7b:
				 KeyValTemp = 12;
			     break; 		

			default:
				 KeyValTemp = 0;
		}
	 }

/*==================第四排S13~S16=====================*/
	 	 KEYPORT = 0xf7;
	 KeyVal  = KEYPORT;
	 KeyVal  = KeyVal&0xf0;
	 if(KeyVal != 0xf0)
	 {
	    DelayForKey(100);
		KeyVal = KEYPORT;
		switch(KeyVal)
		{	
			case 0xe7:
		   	     KeyValTemp = 13;
		       	 break;

			case 0xd7:
				 KeyValTemp = 14;
			     break; 	
				 
			 case 0xb7:
		   	     KeyValTemp = 15;
		       	 break;

			case 0x77:
				 KeyValTemp = 16;
			     break; 		

			default:
				 KeyValTemp = 0;
		}
	 }

	return KeyValTemp;	
}