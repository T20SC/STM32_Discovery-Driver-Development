#ifndef SEVEN_SEGMENT_H
#define SEVEN_SEGMENT_H   

#include "../../Drivers/Inc/stm32f407xx.h"

//7-Segment Mode
#define SEVEN_SEG_DIRECT    0U   //Direct Drive Mode (7 IO pins for 7 segments)
#define SEVEN_SEG_DECODER   1U   //BCD to 7-segment decoder (4 IO pins for BCD input)

//7-Segment Type
#define COMMON_CATHODE      0U   //Common Negative Terminal for Leds
#define COMMON_ANODE        1U   //Common Positive Terminal for Leds 

#define SEG_ERROR_PATTERN  0b01000000

/*======================== Project Configurables ========================*/  

/* 
Project Configurables: Change these values  
to configure the 7-segment display settings 
for your specific hardware setup.
*/

#define SEVEN_SEG_MODE  SEVEN_SEG_DIRECT 
#define SEVEN_SEG_TYPE   COMMON_CATHODE 

#define SEVEN_SEG_PORT        GPIOA 
#define SEVEN_SEG_BCD_PORT    GPIOA 

#define SEG_A_PIN    GPIO_PIN_0
#define SEG_B_PIN    GPIO_PIN_1     
#define SEG_C_PIN    GPIO_PIN_2
#define SEG_D_PIN    GPIO_PIN_3
#define SEG_E_PIN    GPIO_PIN_4
#define SEG_F_PIN    GPIO_PIN_5
#define SEG_G_PIN    GPIO_PIN_6
#define SEG_DP_PIN   GPIO_PIN_7    

#define BCD_A_PIN   GPIO_PIN_0
#define BCD_B_PIN   GPIO_PIN_1
#define BCD_C_PIN   GPIO_PIN_2
#define BCD_D_PIN   GPIO_PIN_3 

/* BCD Numbers */ 
#define BCD_0       0U 
#define BCD_1       1U 
#define BCD_2       2U 
#define BCD_3       3U
#define BCD_4       4U 
#define BCD_5       5U 
#define BCD_6       6U 
#define BCD_7       7U 
#define BCD_8       8U 
#define BCD_9       9U 

/* General 7 Segment Macros */
#define SEG_MAX      9U 
#define BCD_BLANK   15U
/*======================== Project Configurables ========================*/  

void SEVENSEG_INIT(uint8_t SEG_MODE , uint8_t SEG_TYPE , GPIO_Handle_t *pGPIOHandle) ;
void SEVENSEG_DISPLAY(uint8_t SEG_MODE , uint8_t SEG_TYPE , uint8_t number) ;  
void SEVENSEG_ALL_ON(uint8_t SEG_MODE, uint8_t SEG_TYPE) ;  


#endif // SEVEN_SEGMENT_H
