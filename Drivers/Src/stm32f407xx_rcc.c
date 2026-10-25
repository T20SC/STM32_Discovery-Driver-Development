#include "../Inc/stm32f407xx_rcc.h" 
#include "../../Utils/Inc/bitmath.h"

/* index = field code, value = divider */
static const uint16_t AHB_PRESC_TABLE[16] = {1,1,1,1,1,1,1,1,2,4,8,16,64,128,256,512};
static const uint8_t  APB_PRESC_TABLE[8]  = {1,1,1,1,2,4,8,16};

void CLK_Select(uint8_t CLK , uint8_t status) { 
    if(status == ENABLE) {
        switch (CLK)
        {
            case HSI : 
            SET_BIT(RCC->CR , RCC_CR_HSION) ; 
            while(GET_BIT(RCC->CR , RCC_CR_HSIRDY) == 0) { 
                //Infinite Loop Until CLK Stablize
            } 
            WRITE_FIELD(RCC->CFGR , RCC_CFGR_SW_H , RCC_CFGR_SW_L , HSI) ; 
            while (GET_FIELD(RCC->CFGR , RCC_CFGR_SWS_H , RCC_CFGR_SWS_L) != HSI) { 
                //Loop Until HSI is Choosen as Clock
            }
            break; 

            case HSE : 
            SET_BIT(RCC->CR , RCC_CR_HSEON) ; 
            while(GET_BIT(RCC->CR , RCC_CR_HSERDY) == 0) { 
                //Infinite Loop Until CLK Stablize
            } 
            WRITE_FIELD(RCC->CFGR , RCC_CFGR_SW_H , RCC_CFGR_SW_L , HSE) ; 
            while (GET_FIELD(RCC->CFGR , RCC_CFGR_SWS_H , RCC_CFGR_SWS_L) != HSE) { 
                //Loop Until HSE is Choosen as Clock
            }
            break; 

            case PLL :  
            SET_BIT(RCC->CR , RCC_CR_PLLON) ; 
            while(GET_BIT(RCC->CR , RCC_CR_PLLRDY) == 0) { 
                //Infinite Loop Until CLK Stablize
            } 
            WRITE_FIELD(RCC->CFGR , RCC_CFGR_SW_H , RCC_CFGR_SW_L , PLL) ; 
            while (GET_FIELD(RCC->CFGR , RCC_CFGR_SWS_H , RCC_CFGR_SWS_L) != PLL) { 
                //Loop Until PLL is Choosen as Clock
            }
            break; 

            default: /*Do Nothing*/ break;
    } 
}
    else if(status == DISABLE) {  

       // if (GET_FIELD(RCC->CFGR , RCC_CFGR_SWS_H , RCC_CFGR_SWS_L) == CLK) {
       //     return 1;
    //}

        switch (CLK)
        {
            case HSI : 
            CLR_BIT(RCC->CR , RCC_CR_HSION) ; 
            break; 

            case HSE : 
            CLR_BIT(RCC->CR , RCC_CR_HSEON) ; 
            break; 

            case PLL :  
            CLR_BIT(RCC->CR , RCC_CR_PLLON) ; 
            break; 

            default: /*Do Nothing*/ break;
    } 
    } 
    //return 0; 
}

void RCC_SetAPB2Prescaler(uint8_t Prescalar) {
    WRITE_FIELD(RCC->CFGR , RCC_CFGR_PPRE2_H , RCC_CFGR_PPRE2_L , Prescalar);
} 

void RCC_SetAPB1Prescaler(uint8_t Prescalar) {
    WRITE_FIELD(RCC->CFGR , RCC_CFGR_PPRE1_H , RCC_CFGR_PPRE1_L , Prescalar);
}  

void RCC_SetAHBPrescaler(uint8_t Prescalar) {
    WRITE_FIELD(RCC->CFGR , RCC_CFGR_HPRE_H , RCC_CFGR_HPRE_L , Prescalar);
} 

uint32_t RCC_GetAHB_CLK(void) {  
    uint32_t AHB_CLK_Code = GET_FIELD(RCC->CFGR , RCC_CFGR_HPRE_H , RCC_CFGR_HPRE_L) ;
    return HSI_CLK_VALUE / AHB_PRESC_TABLE[AHB_CLK_Code] ;
} 

uint32_t RCC_GetAPB1_CLK(void) {  
    uint32_t APB1_CLK_Code = GET_FIELD(RCC->CFGR , RCC_CFGR_PPRE1_H , RCC_CFGR_PPRE1_L) ;
    return RCC_GetAHB_CLK() / APB_PRESC_TABLE[APB1_CLK_Code] ; 
}  

uint32_t RCC_GetAPB2_CLK(void) {  
    uint32_t APB2_CLK_Code = GET_FIELD(RCC->CFGR , RCC_CFGR_PPRE2_H , RCC_CFGR_PPRE2_L) ;
    return RCC_GetAHB_CLK() / APB_PRESC_TABLE[APB2_CLK_Code] ; 
}  

