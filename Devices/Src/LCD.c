#include "../Inc/LCD.h"

void LCD_E_Control() { 
    SET_BIT(LCD_PORT->ODR , LCD_E) ; 
    delay_us(1) ; 
    CLR_BIT(LCD_PORT->ODR , LCD_E) ;
    delay_us(50) ;
}

void LCD_Send_Command(uint8_t Command) { 
    CLR_BIT(LCD_PORT->ODR , LCD_RS) ;
    WRITE_FIELD(LCD_PORT->ODR , LCD_D7 , LCD_D0 , Command) ;
    LCD_E_Control() ; 
    delay_ms(2) ;
}

void LCD_Clear() { 
    LCD_Send_Command(LCD_CMD_CLEAR) ; 
}

void LCD_Send_Char(uint8_t Char) { 
    SET_BIT(LCD_PORT->ODR , LCD_RS) ;
    WRITE_FIELD(LCD_PORT->ODR , LCD_D7 , LCD_D0 , Char) ;
    LCD_E_Control() ;
    delay_ms(2) ;
}