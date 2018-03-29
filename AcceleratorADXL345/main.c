/*
* AcceleratorADXL345.c
*
* Created: 29.03.2018 13:52:57
* Author : manue
*/

#include <avr/io.h>
#include "ADXL345.h"
#include <stdlib.h>
#include <util/twi.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "lcd.h"


/*
** constant definitions
*/
static const PROGMEM unsigned char copyRightChar[] =
{
	0x07, 0x08, 0x13, 0x14, 0x14, 0x13, 0x08, 0x07,
	0x00, 0x10, 0x08, 0x08, 0x08, 0x08, 0x10, 0x00
};


/*
** function prototypes
*/ 
void wait_until_key_pressed(void);


void wait_until_key_pressed(void)
{
    unsigned char temp1, temp2;
    
    do {
        temp1 = PIND;                  // read input
        _delay_ms(5);                  // delay for key debounce
        temp2 = PIND;                  // read input
        temp1 = (temp1 & temp2);       // debounce input
    } while ( temp1 & _BV(PIND2) );
    
    loop_until_bit_is_set(PIND,PIND2);            /* wait until key is released */
}


int main(void)
{
    char buffer[7];
    int  num=134;
    unsigned char i;
    
    
    DDRD &=~ (1 << PD2);        /* Pin PD2 input              */
    PORTD |= (1 << PD2);        /* Pin PD2 pull-up enabled    */

    /* initialize display, cursor off */
    lcd_init(LCD_DISP_ON);
	
	ADXL345_Init(ADXL345_I2C_COMM);

    for (;;) {                           /* loop forever */
        /* 
         * Test 1:  write text to display
         */
		float *x = NULL;
		float *y = NULL;
		float *z = NULL;
		ADXL345_GetXyz(x,y,z);
		printf(&x);

        /* clear display and home cursor */
        lcd_clrscr();
        
        /* put string to display (line 1) with linefeed */
        lcd_puts("LCD Test Line 1\n");

        /* cursor is now on second line, write second line */
        lcd_puts("Line 2");
        
        /* move cursor to position 8 on line 2 */
        lcd_gotoxy(7,1);  
        
        /* write single char to display */
        lcd_putc(':');
        
        /* wait until push button PD2 (INT0) is pressed */
        wait_until_key_pressed();
        
        
        /*
         * Test 2: use lcd_command() to turn on cursor
         */
        
        /* turn on cursor */
        lcd_command(LCD_DISP_ON_CURSOR);

        /* put string */
        lcd_puts( "CurOn");
        
        /* wait until push button PD2 (INT0) is pressed */
        wait_until_key_pressed();
              
    }	
}

