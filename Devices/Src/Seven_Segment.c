#include "../Inc/Seven_Segment.h" 
#include "../../Drivers/Inc/stm32f407xx.h"  
#include "../../Drivers/Inc/stm32f407xx_gpio.h" 
#include "../../Utils/Inc/bitmath.h"

static const uint8_t SEVSEG_MAP[10] = { 
    0b00111111, // 0
    0b00000110, // 1
    0b01011011, // 2
    0b01001111, // 3
    0b01100110, // 4
    0b01101101, // 5
    0b01111101, // 6
    0b00000111, // 7
    0b01111111, // 8
    0b01101111  // 9
};     

//Must intialize CLK + Configure GPIO Handle before calling this function
void SEVEN_SEG_INIT(uint8_t SEG_MODE , GPIO_Handle_t *pGPIOHandle) {  
    switch(SEG_MODE) { 
        case SEVEN_SEG_DIRECT: 
            pGPIOHandle->pGPIOx = SEVEN_SEG_PORT;
            // Initialize GPIO pins for direct drive mode  
            for(uint8_t i = SEG_A_PIN  ; i <= SEG_DP_PIN ; i++) {  
                pGPIOHandle->GPIO_PinConfig.GPIO_Pin_Number = i ; 
                GPIO_Init(pGPIOHandle); 
            }
        break; 

        // Initialize GPIO pins for BCD to 7-segment decoder mode 
        case SEVEN_SEG_DECODER:  
            pGPIOHandle->pGPIOx = SEVEN_SEG_BCD_PORT;
            for(uint8_t i = BCD_A_PIN  ; i <= BCD_D_PIN ; i++) {    
                pGPIOHandle->GPIO_PinConfig.GPIO_Pin_Number = i ; 
                GPIO_Init(pGPIOHandle); 
            }
        break;

        default : /* Do nothing */ break;
    }
}  

void SEVENSEG_ALL_ON(uint8_t SEG_MODE , uint8_t SEG_TYPE)  {   
    switch(SEG_MODE) { 
        case SEVEN_SEG_DIRECT:  
            if(SEG_TYPE == COMMON_CATHODE) { 
                SET_FIELD(SEVEN_SEG_PORT->ODR , SEG_DP_PIN , SEG_A_PIN); // Turn on all segments for common cathode
            } 
            else if(SEG_TYPE == COMMON_ANODE) { 
                CLR_FIELD(SEVEN_SEG_PORT->ODR , SEG_DP_PIN , SEG_A_PIN); // Turn on all segments for common anode
            }
        break; 

        case SEVEN_SEG_DECODER:  
            WRITE_FIELD(SEVEN_SEG_BCD_PORT->ODR , BCD_D_PIN , BCD_A_PIN , BCD_8); // Digit 8: lights segments A-G (DP is not driven by the decoder)
        break; 

        default: /* Do Nothing */ break;
    }    
}

void SEVENSEG_DISPLAY(uint8_t SEG_MODE , uint8_t SEG_TYPE , uint8_t number) { 
    if(number <= SEG_MAX) { 
        switch(SEG_MODE) { 
            case SEVEN_SEG_DIRECT:  
                if(SEG_TYPE == COMMON_CATHODE) { 
                    WRITE_FIELD(SEVEN_SEG_PORT->ODR , SEG_DP_PIN , SEG_A_PIN , SEVSEG_MAP[number]); // Write segment pattern for common cathode
                }
                else if(SEG_TYPE == COMMON_ANODE) { 
                    WRITE_FIELD(SEVEN_SEG_PORT->ODR , SEG_DP_PIN , SEG_A_PIN , (uint8_t)~SEVSEG_MAP[number]); // Write inverted segment pattern for common anode
                } 
            break;  

            case SEVEN_SEG_DECODER:  
            WRITE_FIELD(SEVEN_SEG_BCD_PORT->ODR , BCD_D_PIN , BCD_A_PIN , number); // Write BCD value 
            break; 

            default : /* Do nothing */ break;

        }
    } 
    else { 
       switch(SEG_MODE) { 
            case SEVEN_SEG_DIRECT:  
                if(SEG_TYPE == COMMON_CATHODE) { 
                    CLR_FIELD(SEVEN_SEG_PORT->ODR , SEG_DP_PIN , SEG_A_PIN); // CLR (Error) segment pattern for common cathode
                }
                else if(SEG_TYPE == COMMON_ANODE) { 
                    SET_FIELD(SEVEN_SEG_PORT->ODR , SEG_DP_PIN , SEG_A_PIN); //SET (Error) segment pattern for common anode
                } 
            break;  

            case SEVEN_SEG_DECODER:  
            WRITE_FIELD(SEVEN_SEG_BCD_PORT->ODR , BCD_D_PIN , BCD_A_PIN , BCD_BLANK); // Clear Segment (Error)
            break; 

            default : /* Do nothing */ break;

        }
    }
}

