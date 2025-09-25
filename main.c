#include <LPC21xx.h>
#include "delay.h" 
#include "lcd.h"  
#include "uart0.h"
#include "keypad.h"
#include "R305.h" 


int main(void)
{
    unsigned int pageid; 
   
    LCD_Init();
    InitUART0();
	initrows();
	initcols();
    Write_CMD_LCD(0x01);
    Write_CMD_LCD(0x80);
    Write_str_LCD("FINGER PRINT ");

    delay_ms(2000);

    while (1)
    {
	   	Write_CMD_LCD(0x01);
	   	Write_CMD_LCD(0x80);
	   	Write_str_LCD("1.ENROLL 3.DEL ");		
		Write_CMD_LCD(0xc0);
		Write_str_LCD("2.SEARCH 4.D.ALL");
		do
		{
			pageid = keyscan();
		}while((pageid != 0x01) && (pageid != 0x02) && (pageid != 0x03) && (pageid != 0x04));

		if(pageid == 0x01)
		{
			Write_CMD_LCD(0x01);
	   		Write_CMD_LCD(0x80);
	   		Write_str_LCD("Enter ID: ");
			pageid = keyscan();
	   		Write_CMD_LCD(0xC0);
	   		Write_int_LCD(pageid);					
    		delay_ms(100);
			if(pageid>0 && pageid <10)
			{
				enroll(pageid);
			}
		}
		else if(pageid == 0x02)
		{
	   		Write_CMD_LCD(0x01);
	   		Write_CMD_LCD(0x80);
	   		Write_str_LCD("PLACE FINGER");
         	Write_CMD_LCD(0xC0);
          	Write_int_LCD(search_fp()-1); 
			delay_ms(1000); 
		}
		else if(pageid == 0x03)
		{
			Write_CMD_LCD(0x01);
	   		Write_CMD_LCD(0x80);
	   		Write_str_LCD("Enter ID: ");
			pageid = keyscan();
	   		Write_CMD_LCD(0xC0);
	   		Write_int_LCD(pageid);					
    		delay_ms(100);
			if(pageid>0 && pageid <10)
			{
				if(delete_fp(pageid) == 0x00)
				{
					Write_CMD_LCD(0x01);
			   		Write_CMD_LCD(0x80);
			   		Write_str_LCD("SELECTED ID DEL.");					
					delay_ms(1000);
				}
				else
				{
					Write_CMD_LCD(0x01);
			   		Write_CMD_LCD(0x80);
			   		Write_str_LCD("Failed to del..");					
					delay_ms(1000);
				}
			}
		}
		else if(pageid == 0x04)
		{
			if(deleteall_fp() == 0x00)
			{
                Write_CMD_LCD(0x01);
                Write_CMD_LCD(0x80);
                Write_str_LCD("DEL ALL SUCCUSS");					
				delay_ms(1000);	
			}
			else
			{
				Write_CMD_LCD(0x01);
			   	Write_CMD_LCD(0x80);
			   	Write_str_LCD("Failed to del..");					
				delay_ms(1000);				
			}
		}	
    }
}
