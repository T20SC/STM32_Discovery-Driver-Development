/*
 * stm32f407xx_gpio.h
 *
 *  Created on: Sep 22, 2026
 *      Author: tmt
 */

#ifndef STM32F407XX_GPIO_H_
#define STM32F407XX_GPIO_H_

#include "stm32f407xx.h"

//GPIO Pin Numbers
//@GPIO_PIN_NUMBER
#define GPIO_PIN_0 0U
#define GPIO_PIN_1 1U
#define GPIO_PIN_2 2U
#define GPIO_PIN_3 3U
#define GPIO_PIN_4 4U
#define GPIO_PIN_5 5U
#define GPIO_PIN_6 6U
#define GPIO_PIN_7 7U
#define GPIO_PIN_8 8U
#define GPIO_PIN_9 9U
#define GPIO_PIN_10 10U
#define GPIO_PIN_11 11U
#define GPIO_PIN_12 12U
#define GPIO_PIN_13 13U
#define GPIO_PIN_14 14U
#define GPIO_PIN_15 15U

//GPIO Pin Modes
//@GPIO_PIN_MODE
#define GPIO_MODE_IN     0U   //Input Mode
#define GPIO_MODE_OUT    1U   //Output Mode
#define GPIO_MODE_AF     2U   //Alternate Function Mode
#define GPIO_MODE_ANALOG 3U   //Analog Mode
#define GPIO_MODE_IT_FT  4U   //Falling Edge Trigger
#define GPIO_MODE_IT_RT  5U   //Rising Edge Trigger
#define GPIO_MODE_IT_RFT 6U   //Rising and Falling Edge Trigger

//GPIO Pin Output Types
//@GPIO_OUT_TYPE
#define GPIO_OUT_TYPE_PP 0U   //Push-Pull Output
#define GPIO_OUT_TYPE_OD 1U   //Open-Drain Output

//GPIO Pin Output Speeds
//@GPIO_OUT_SPEED
#define GPIO_SPEED_LOW     0U    //Low Speed
#define GPIO_SPEED_MEDIUM  1U    //Medium Speed
#define GPIO_SPEED_HIGH    2U    //High Speed
#define GPIO_SPEED_VHIGH   3U    //Very High Speed

//GPIO Pin Pull-Up/Pull-Down Configurations
//@GPIO_PIN_PUPD
#define GPIO_NO_PUPD  0U   //No Pull-Up or Pull-Down
#define GPIO_PIN_PU   1U   //Pull-Up
#define GPIO_PIN_PD   2U   //Pull-Down
//11 : Reserved



//Config Struct for GPIO
typedef struct {

uint8_t GPIO_Pin_Number ;
uint8_t GPIO_Pin_Mode ;       //Options at @GPIO_PIN_MODE
uint8_t GPIO_Pin_Speed ;      //Options at @GPIO_OUT_SPEED
uint8_t GPIO_Pin_PUPDR ;      //Options at @GPIO_PIN_PUPD
uint8_t GPIO_Out_Type ;       //Options at @GPIO_OUT_TYPE
uint8_t GPIO_AltFun_Mode ;

}GPIO_PinConfig_t;

typedef struct {

GPIO_RegDef_t *pGPIOx ; /* Hold the Base Address of the GPIO Port to which the pin belongs */
GPIO_PinConfig_t GPIO_PinConfig ; /* Hold the GPIO Pin Config Settings */

} GPIO_Handle_t;

/*
  ==========================================================================================
  =                             API IMPLEMENTATION FOR GPIO                                =
  ==========================================================================================
*/

//CLK Enable/Disable
void GPIO_CLK_Control(GPIO_RegDef_t *pGPIOx , uint8_t ENorDI) ;

//Init/DeInit
void GPIO_Init(GPIO_Handle_t *pGPIOHandle) ;
void GPIO_DeInit(GPIO_RegDef_t *pGPIOx) ;

//Read/Write
uint8_t GPIO_Pin_Read(GPIO_RegDef_t *pGPIOx , uint8_t PinNumber) ;
uint16_t GPIO_Port_Read(GPIO_RegDef_t *pGPIOx) ;
void GPIO_Pin_Write(GPIO_RegDef_t *pGPIOx , uint8_t PinNumber , uint8_t Value) ;
void GPIO_Port_Write(GPIO_RegDef_t *pGPIOx , uint16_t Value) ;
void GPIO_Pin_Toggle(GPIO_RegDef_t *pGPIOx , uint8_t PinNumber) ;

//IRQ Configuration and ISR Handling
void GPIO_IRQ_Config(uint8_t IRQNumber , uint8_t ENorDI) ;
void GPIO_IRQ_Priority_Config(uint8_t IRQNumber , uint32_t IRQPriority) ;
void GPIO_IRQ_Handling(uint8_t PinNumber) ;

#endif /* STM32F407XX_GPIO_H_ */
