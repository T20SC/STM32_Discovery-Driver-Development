#include "../../Utils/Inc/bitmath.h"
#include "../../Drivers/Inc/stm32f407xx_gpio.h" 

#ifndef LCD_H
#define LCD_H

//Operations 
#define COMMAND     0U
#define DATA        1U

#define WRITE       0U
#define READ        1U


//Commands Macros
#define LCD_CMD_CLEAR            0x01U
#define LCD_CMD_FUNC_8BIT_2LINE  0x38U
#define LCD_CMD_DISPLAY_OFF      0x08U
#define LCD_CMD_DISPLAY_ON       0x0CU   /* display on, no cursor */
#define LCD_CMD_ENTRY_INC        0x06U   /* cursor moves right after each char */

/* Cursor positioning */
#define LCD_CMD_SET_DDRAM        0x80U
#define LCD_LINE1_ADDR           0x00U
#define LCD_LINE2_ADDR           0x40U

//Mode 
#define LCD_8_BIT   0
#define LCD_4_BIT   1

/*======================== Project Configurables ========================*/  

/* 
Project Configurables: Change these values  
to configure the LCD display settings 
for your specific hardware setup.
*/ 


#define LCD_PORT    GPIOA

//Data Bus
#define LCD_D0      0U
#define LCD_D1      1U 
#define LCD_D2      2U
#define LCD_D3      3U 
#define LCD_D4      4U
#define LCD_D5      5U  
#define LCD_D6      6U
#define LCD_D7      7U 

//Control lines
#define LCD_RS      8U
#define LCD_RW      9U
#define LCD_E       10U

/*======================== Project Configurables ========================*/

void LCD_E_Control() ; 
void LCD_Send_Command(uint8_t Command) ;
void LCD_Send_Char(uint8_t Char) ;
#endif // LCD_H