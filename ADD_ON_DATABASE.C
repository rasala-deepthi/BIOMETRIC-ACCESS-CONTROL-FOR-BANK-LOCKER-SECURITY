#include <LPC21xx.h>
#include <string.h>
#include "delay.h" 
#include "lcd.h"  
#include "uart0.h"
#include "kpm.h"
#include "R305.h" 

int check_user(char *password, int fingerprint_id);
void add(void);
// Password & User Settings
#define PASSWORD "1234"
#define ADMIN_PASS "ADMIN"
#define MAX_ATTEMPTS 3
#define MAX_USERS 3

// User Database (Password + Fingerprint ID)
typedef struct {
    char password[5];
    int fingerprint_id;
} User;

User users[MAX_USERS] = {
	{"1234", -1}, // User 1
    {"5678", 2}, // User 2
    {"0000", 3}  // User 3
};

int main(void)
{
    //unsigned int pageid; 
   	int fingerprint_id;
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
		int i = 0;
		char key;
	 	Write_CMD_LCD(0x01);
	   	Write_CMD_LCD(0x80);
	   	Write_str_LCD("1:Unlock 2:Reset");		
		Write_CMD_LCD(0xc0);
		do
		{
		   key = keyscan();
		}while((key != '1') && (key != '2') && (key != '3'));
 		Write_CMD_LCD(key);
		 delay_ms(500);
		if (key == '1') 
		{ // Unlock Mode
            char entered_pass[5];
            Write_CMD_LCD(0x01);
            Write_str_LCD("Enter Pass:");

            while (i < 4) 
			{
                key = keyscan();
				delay_ms(50);
                if (key != 0) 
				{
                    entered_pass[i++] = key;
                    Write_str_LCD("*");
                }
            }
		
            entered_pass[4] = '\0';
			Write_CMD_LCD(0x01);
	   		Write_CMD_LCD(0x80);
	   		Write_str_LCD(entered_pass);	
			delay_ms(1000);
			Write_CMD_LCD(0x01);
	   		Write_CMD_LCD(0x80);
	   		Write_str_LCD("PLACE FINGER");
         	Write_CMD_LCD(0xC0);
			fingerprint_id = (search_fp()-1);
			Write_int_LCD(fingerprint_id);
			delay_ms(1000); 
			if (check_user(entered_pass, fingerprint_id)) 
			{
                Write_CMD_LCD(0x01);
                Write_str_LCD("Access Granted");
				delay_ms(1500);
            }
			else
			{
				Write_CMD_LCD(0x01);
                Write_str_LCD("Access Denied");
				delay_ms(1500);
			}		
		}
		else if(key == '3')
		{
			add();		
		}
	}
}
void add(void)
{
		int pageid; 
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
// Check User Credentials
int check_user(char *password, int fingerprint_id) 
{
     int i;
    for (i = 0; i < MAX_USERS; i++) 
	{
        if (strcmp(password, users[i].password) == 0 && fingerprint_id == users[i].fingerprint_id) 
		{
            return 1; // Valid user
        }
    }
    return 0; // Invalid user
}
