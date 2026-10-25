#ifndef STM32F407XX_RCC_H_
#define STM32F407XX_RCC_H_

#include "stm32f407xx.h"

#define HSI_CLK_VALUE   16000000UL

#define HSI     0U 
#define HSE     1U
#define PLL     2U

/* AHB prescaler (HPRE) codes */
#define AHB_DIV_1       0U
#define AHB_DIV_2       8U
#define AHB_DIV_4       9U
#define AHB_DIV_8       10U
#define AHB_DIV_16      11U
#define AHB_DIV_64      12U
#define AHB_DIV_128     13U
#define AHB_DIV_256     14U
#define AHB_DIV_512     15U

/* APB prescaler (PPRE1 / PPRE2) codes */
#define APB_DIV_1       0U
#define APB_DIV_2       4U
#define APB_DIV_4       5U
#define APB_DIV_8       6U
#define APB_DIV_16      7U


void CLK_Select(uint8_t CLK , uint8_t status) ;  
void RCC_SetAPB2Prescaler(uint8_t Prescalar) ;

void RCC_SetAPB1Prescaler(uint8_t Prescalar) ;

void RCC_SetAHBPrescaler(uint8_t Prescalar) ;

uint32_t RCC_GetAHB_CLK() ;  
uint32_t RCC_GetAPB1_CLK() ;
uint32_t RCC_GetAPB2_CLK() ;


#endif /* STM32F407XX_RCC_H_ */