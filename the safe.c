

#define  F_CPU 8000000UL
#include <util/delay.h>
#include "keypad_driver.h"
#include "LCD.h"
#include "the safe_priv.h"


char arr[4];

int main(void)
{
	char value=NOTPRESSED;
	char flag=0,counter;
	char tries=MAX_TRIES;
	keypad_vInit();
	LCD_vInit();
	LCD_movecursor(1,4);
	LCD_vSend_string("welcome");
	_delay_ms(2000);
	LCD_clearscreen();
	
	if (EEPROM_read(EEPROM_STATUS_LOCATION)==NOTPRESSED)
	{
		LCD_vSend_string("set pass:");
		for (counter=0;counter<=3;counter++)
		{
			do 
			{
				value=keypad_u8check_press();
			}
			while (value==NOTPRESSED);
			LCD_vSend_char(value);
			_delay_ms(500);
			LCD_movecursor(1,10+counter);
			LCD_vSend_char('*');
			EEPROM_write(EEPROM_PASSWORD_LOCATION1+counter,value);
		}	
		EEPROM_write(EEPROM_STATUS_LOCATION,0x00);
		_delay_ms(1000);
	}
					
    while(flag==0)
    {
		arr[0]=arr[1]=arr[2]=arr[3]=NOTPRESSED;
		LCD_clearscreen();
		LCD_vSend_string("check pass:");
		for (counter=0;counter<=3;counter++)
		{
			do
			{
				arr[counter]=keypad_u8check_press();
			}
			while (arr[counter]==NOTPRESSED);
			LCD_vSend_char(arr[counter]);
			_delay_ms(500);
			LCD_movecursor(1,12+counter);
			LCD_vSend_char('*');
		}		
		
	if(EEPROM_read(EEPROM_PASSWORD_LOCATION1)==arr[0] &&  EEPROM_read(EEPROM_PASSWORD_LOCATION2)==arr[1] && EEPROM_read(EEPROM_PASSWORD_LOCATION3)==arr[2] && EEPROM_read(EEPROM_PASSWORD_LOCATION4)==arr[3])
	{
		LCD_clearscreen();
		LCD_vSend_string("right password");
		LCD_movecursor(2,1);
		LCD_vSend_string("safe opened");
		flag=1;
	}	
	else
	{
		tries=tries-1;
		if (tries>0)
		{
			LCD_clearscreen();
			LCD_vSend_string("wrong password");
			_delay_ms(1000);
			LCD_clearscreen();
			LCD_vSend_string("tries left:");
			LCD_vSend_char(tries+48);
			_delay_ms(1000);
		
		}
		else
		{
			LCD_clearscreen();
			LCD_vSend_string("wrong password");
			LCD_movecursor(2,1);
			LCD_vSend_string("safe closed");
			flag=1;
		}
	}
	}			
}

