#include "../../Utils/Inc/bitmath.h"
#include "../Inc/stm32f407xx_gpio.h"

/*
 * ---------------------------------------------------------
 * Function: GPIO_CLK_Control
 *
 * Description:
 *     Enables or disables the clock for the chosen GPIO port.
 *
 * Parameters:
 *     param1 - Pointer to the GPIO register definition.
 *     param2 - Enable or disable flag.
 *
 * Returns:
 *     Does not return a value.
 *
 * ---------------------------------------------------------
 */

void GPIO_CLK_Control(GPIO_RegDef_t *pGPIOx , uint8_t ENorDI) {
    if (ENorDI == ENABLE) {
        switch ((uint32_t)pGPIOx) {
            case (uint32_t)GPIOA : SET_BIT(RCC->AHB1ENR, RCC_GPIOAEN); break;   
            case (uint32_t)GPIOB : SET_BIT(RCC->AHB1ENR, RCC_GPIOBEN); break;
            case (uint32_t)GPIOC : SET_BIT(RCC->AHB1ENR, RCC_GPIOCEN); break;
            case (uint32_t)GPIOD : SET_BIT(RCC->AHB1ENR, RCC_GPIODEN); break;
            case (uint32_t)GPIOE : SET_BIT(RCC->AHB1ENR, RCC_GPIOEEN); break;
            case (uint32_t)GPIOF : SET_BIT(RCC->AHB1ENR, RCC_GPIOFEN); break;
            case (uint32_t)GPIOG : SET_BIT(RCC->AHB1ENR, RCC_GPIOGEN); break;
            case (uint32_t)GPIOH : SET_BIT(RCC->AHB1ENR, RCC_GPIOHEN); break;
            case (uint32_t)GPIOI : SET_BIT(RCC->AHB1ENR, RCC_GPIOIEN); break;
            default : /* invalid port: return an error or trap here */ break;
        }
    }

    else if(ENorDI == DISABLE) {
        switch ((uint32_t)pGPIOx) {
            case (uint32_t)GPIOA : CLR_BIT(RCC->AHB1ENR , RCC_GPIOAEN) ; break ;
            case (uint32_t)GPIOB : CLR_BIT(RCC->AHB1ENR , RCC_GPIOBEN) ; break ;
            case (uint32_t)GPIOC : CLR_BIT(RCC->AHB1ENR , RCC_GPIOCEN) ; break ;
            case (uint32_t)GPIOD : CLR_BIT(RCC->AHB1ENR , RCC_GPIODEN) ; break ;
            case (uint32_t)GPIOE : CLR_BIT(RCC->AHB1ENR , RCC_GPIOEEN) ; break ;
            case (uint32_t)GPIOF : CLR_BIT(RCC->AHB1ENR , RCC_GPIOFEN) ; break ;
            case (uint32_t)GPIOG : CLR_BIT(RCC->AHB1ENR , RCC_GPIOGEN) ; break ;
            case (uint32_t)GPIOH : CLR_BIT(RCC->AHB1ENR , RCC_GPIOHEN) ; break ;
            case (uint32_t)GPIOI : CLR_BIT(RCC->AHB1ENR , RCC_GPIOIEN) ; break ;
            default : /* invalid port: return an error or trap here */ break ;
        }
    }
}

/*
 * ---------------------------------------------------------
 * Function: GPIO_Init
 *
 * Description:
 *       Configures a GPIO pin (mode, speed, pull-up/pull-down,
 *       output type, and alternate function if applicable)
 *       according to the settings provided in the handle.
 *
 * Parameters:
 *      pGPIOHandle - Pointer to a GPIO_Handle_t structure containing:
 *                     - pGPIOx: base address of the GPIO port
 *                     - GPIO_PinConfig: desired pin configuration
 *
 * Returns:
 *     Does not return a value.
 * Note:
 *     The GPIO peripheral clock must be enabled (via
 *     GPIO_PeriClockControl) before calling this function.
 * ---------------------------------------------------------
 */

void GPIO_Init(GPIO_Handle_t *pGPIOHandle) {
    //Configure the mode of GPIO pin
    uint8_t PIN_NUM = pGPIOHandle->GPIO_PinConfig.GPIO_Pin_Number ;

    if(pGPIOHandle->GPIO_PinConfig.GPIO_Pin_Mode <= GPIO_MODE_ANALOG) {
        //Non-Interrupt Mode

        switch (pGPIOHandle->GPIO_PinConfig.GPIO_Pin_Mode) {
            case GPIO_MODE_IN :
                //Configure the MODER register for input mode
                CLR_BIT(pGPIOHandle->pGPIOx->MODER, (2 * PIN_NUM)); // Clear the 2xn bit
                CLR_BIT(pGPIOHandle->pGPIOx->MODER, (2 * PIN_NUM + 1)); // Clear the 2xn+1 bit
                break ;

            case GPIO_MODE_OUT :
                //Configure the MODER register for output mode
                SET_BIT(pGPIOHandle->pGPIOx->MODER, (2 * PIN_NUM)); // Set the 2xn bit
                CLR_BIT(pGPIOHandle->pGPIOx->MODER, (2 * PIN_NUM + 1)); // Clear the 2xn+1 bit
                break ;

            case GPIO_MODE_AF :
                //Configure the MODER register for alternate function mode
                CLR_BIT(pGPIOHandle->pGPIOx->MODER, (2 * PIN_NUM)); // Clear the 2xn bit
                SET_BIT(pGPIOHandle->pGPIOx->MODER, (2 * PIN_NUM + 1)); // Set the 2xn+1 bit
                break ;

            case GPIO_MODE_ANALOG :
                //Configure the MODER register for analog mode
                SET_BIT(pGPIOHandle->pGPIOx->MODER, (2 * PIN_NUM)); // Set the 2xn bit
                SET_BIT(pGPIOHandle->pGPIOx->MODER, (2 * PIN_NUM + 1)); // Set the 2xn+1 bit
                break ;

            default : break ;

        }
    }

    else {
        //Interrupt Mode
        if(pGPIOHandle->GPIO_PinConfig.GPIO_Pin_Mode == GPIO_MODE_IT_FT) {

            //Configure the Falling Edge Trigger Selection Register (FTSR)
            SET_BIT(EXTI->FTSR, PIN_NUM); // Set the bit for falling edge trigger
            // Clear the corresponding bit in the Rising Edge Trigger Selection Register (RTSR)
            CLR_BIT(EXTI->RTSR, PIN_NUM); // Clear the bit for rising edge trigger

        }

        else if(pGPIOHandle->GPIO_PinConfig.GPIO_Pin_Mode == GPIO_MODE_IT_RT) {

            //Configure the Rising Edge Trigger Selection Register (RTSR)
            SET_BIT(EXTI->RTSR, PIN_NUM); // Set the bit for rising edge trigger
            // Clear the corresponding bit in the Falling Edge Trigger Selection Register (FTSR)
            CLR_BIT(EXTI->FTSR, PIN_NUM); // Clear the bit for falling edge trigger

        }

        else if(pGPIOHandle->GPIO_PinConfig.GPIO_Pin_Mode == GPIO_MODE_IT_RFT) {

            //Configure both the Falling and Rising Edge Trigger Selection Registers (FTSR and RTSR)
            SET_BIT(EXTI->FTSR, PIN_NUM); // Set the bit for falling edge trigger
            SET_BIT(EXTI->RTSR, PIN_NUM); // Set the bit for rising edge trigger

        }
        //Configure the GPIO port selection in the SYSCFG_EXTICR register
        SET_BIT(RCC->APB2ENR, 14); // Enable the clock for SYSCFG peripheral 


        uint8_t EXTI_INDEX = PIN_NUM / 4 ; // Determine which EXTICR register to use (0-3)
        uint8_t EXTI_POS = (PIN_NUM % 4) * 4 ; // Determine the position within the EXTICR register (0-3)

        SYSCFG->EXTICR[EXTI_INDEX] &= ~(0xF << EXTI_POS); // Clear the 4 bits for the pin
        switch ((uint32_t)pGPIOHandle->pGPIOx) {

            case (uint32_t)GPIOA :
            // No need to set anything, as 0b0000 corresponds to GPIOA
            break ; 

            case (uint32_t)GPIOB :
            SYSCFG->EXTICR[EXTI_INDEX] |= (1U << EXTI_POS);
            break;

            case (uint32_t)GPIOC :
            SYSCFG->EXTICR[EXTI_INDEX] |= (2U << EXTI_POS);
            break;

            case (uint32_t)GPIOD :
            SYSCFG->EXTICR[EXTI_INDEX] |= (3U << EXTI_POS);
            break;

            case (uint32_t)GPIOE :
            SYSCFG->EXTICR[EXTI_INDEX] |= (4U << EXTI_POS);
            break;

            case (uint32_t)GPIOF :
            SYSCFG->EXTICR[EXTI_INDEX] |= (5U << EXTI_POS);
            break;

            case (uint32_t)GPIOG :
            SYSCFG->EXTICR[EXTI_INDEX] |= (6U << EXTI_POS);
            break;

            case (uint32_t)GPIOH :
            SYSCFG->EXTICR[EXTI_INDEX] |= (7U << EXTI_POS);
            break;

            case (uint32_t)GPIOI :
            SYSCFG->EXTICR[EXTI_INDEX] |= (8U << EXTI_POS);
            break;

            default : break ;
        }


        //Enable the EXTI interrupt delivery using the IMR register
        SET_BIT(EXTI->IMR, PIN_NUM); // Enable the interrupt for the pin
    }

    //Configure the speed of GPIO pin
    switch (pGPIOHandle->GPIO_PinConfig.GPIO_Pin_Speed) {

        case GPIO_SPEED_LOW :
            CLR_BIT(pGPIOHandle->pGPIOx->OSPEEDR, (2 * PIN_NUM)); // Clear the 2xn bit
            CLR_BIT(pGPIOHandle->pGPIOx->OSPEEDR, (2 * PIN_NUM + 1)); // Clear the 2xn+1 bit
            break ;

        case GPIO_SPEED_MEDIUM :
            SET_BIT(pGPIOHandle->pGPIOx->OSPEEDR, (2 * PIN_NUM)); // Set the 2xn bit
            CLR_BIT(pGPIOHandle->pGPIOx->OSPEEDR, (2 * PIN_NUM + 1)); // Clear the 2xn+1 bit
            break ;

        case GPIO_SPEED_HIGH :
            CLR_BIT(pGPIOHandle->pGPIOx->OSPEEDR, (2 * PIN_NUM)); // Clear the 2xn bit
            SET_BIT(pGPIOHandle->pGPIOx->OSPEEDR, (2 * PIN_NUM + 1)); // Set the 2xn+1 bit
            break ;

        case GPIO_SPEED_VHIGH :
            SET_BIT(pGPIOHandle->pGPIOx->OSPEEDR, (2 * PIN_NUM)); // Set the 2xn bit
            SET_BIT(pGPIOHandle->pGPIOx->OSPEEDR, (2 * PIN_NUM + 1)); // Set the 2xn+1 bit
            break ;

        default : break ;

    }

    //Configure the pull-up/pull-down settings of GPIO pin
    switch (pGPIOHandle->GPIO_PinConfig.GPIO_Pin_PUPDR) {

        case GPIO_NO_PUPD :
            CLR_BIT(pGPIOHandle->pGPIOx->PUPDR, (2 * PIN_NUM)); // Clear the 2xn bit
            CLR_BIT(pGPIOHandle->pGPIOx->PUPDR, (2 * PIN_NUM + 1)); // Clear the 2xn+1 bit
            break ;

        case GPIO_PIN_PU :
            SET_BIT(pGPIOHandle->pGPIOx->PUPDR, (2 * PIN_NUM)); // Set the 2xn bit
            CLR_BIT(pGPIOHandle->pGPIOx->PUPDR, (2 * PIN_NUM + 1)); // Clear the 2xn+1 bit
            break ;

        case GPIO_PIN_PD :
            CLR_BIT(pGPIOHandle->pGPIOx->PUPDR, (2 * PIN_NUM)); // Clear the 2xn bit
            SET_BIT(pGPIOHandle->pGPIOx->PUPDR, (2 * PIN_NUM + 1)); // Set the 2xn+1 bit
            break ;

        default : break ;

    }

    //Configure the output type of GPIO pin
    switch (pGPIOHandle->GPIO_PinConfig.GPIO_Out_Type) {

        case GPIO_OUT_TYPE_PP :
            CLR_BIT(pGPIOHandle->pGPIOx->OTYPER, PIN_NUM); // Clear the bit for push-pull
            break ;

        case GPIO_OUT_TYPE_OD :
            SET_BIT(pGPIOHandle->pGPIOx->OTYPER, PIN_NUM); // Set the bit for open-drain
            break ;

        default : break ;

    }

    //Configure the alternate functionality of GPIO pin
    if(pGPIOHandle->GPIO_PinConfig.GPIO_Pin_Mode == GPIO_MODE_AF) {

        uint8_t AF_MODE = pGPIOHandle->GPIO_PinConfig.GPIO_AltFun_Mode ;

        if(PIN_NUM <= 7) {
            //Configure AFRL register for pins 0-7
            pGPIOHandle->pGPIOx->AFRL &= ~(0xF << (4 * PIN_NUM)); // Clear the 4 bits
            pGPIOHandle->pGPIOx->AFRL |= (AF_MODE << (4 * PIN_NUM)); // Set the alternate function
        }
        else {
            //Configure AFRH register for pins 8-15
            uint8_t afr_pos = PIN_NUM - 8 ; // Adjust pin number for AFRH
            pGPIOHandle->pGPIOx->AFRH &= ~(0xF << (4 * afr_pos)); // Clear the 4 bits
            pGPIOHandle->pGPIOx->AFRH |= (AF_MODE << (4 * afr_pos)); // Set the alternate function
        }
    }
}

/*
 * ---------------------------------------------------------
 * Function: GPIO_DeInit
 *
 * Description:
 *      Resets the given GPIO port back to its default
 *      (power-on reset) state, by pulsing the corresponding
 *       bit in RCC_AHB1RSTR.
 *
 * Parameters:
 *     param1 -  pGPIOx - Base address of the GPIO peripheral to reset
 *              (e.g. GPIOA, GPIOB, ...).
 *
 * Returns:
 *     Does not return a value.
 * ---------------------------------------------------------
 */

void GPIO_DeInit(GPIO_RegDef_t *pGPIOx) {
    switch((uint32_t)pGPIOx) {

        case (uint32_t)GPIOA :
        RCC->AHB1RSTR |= (1U << 0) ;           //Reset the GPIOA peripheral
        RCC->AHB1RSTR &= ~(1U << 0) ;         // Clear the reset bit after reset
        break ;

        case (uint32_t)GPIOB :
        RCC->AHB1RSTR |= (1U << 1) ;           //Reset the GPIOB peripheral
        RCC->AHB1RSTR &= ~(1U << 1) ;         // Clear the reset bit after reset
        break ;

        case (uint32_t)GPIOC :
        RCC->AHB1RSTR |= (1U << 2) ;           //Reset the GPIOC peripheral
        RCC->AHB1RSTR &= ~(1U << 2) ;         // Clear the reset bit after reset
        break ;

        case (uint32_t)GPIOD :
        RCC->AHB1RSTR |= (1U << 3) ;           //Reset the GPIOD peripheral
        RCC->AHB1RSTR &= ~(1U << 3) ;         // Clear the reset bit after reset
        break ;

        case (uint32_t)GPIOE :
        RCC->AHB1RSTR |= (1U << 4) ;           //Reset the GPIOE peripheral
        RCC->AHB1RSTR &= ~(1U << 4) ;         // Clear the reset bit after reset
        break ;

        case (uint32_t)GPIOF :
        RCC->AHB1RSTR |= (1U << 5) ;           //Reset the GPIOF peripheral
        RCC->AHB1RSTR &= ~(1U << 5) ;         // Clear the reset bit after reset
        break ;

        case (uint32_t)GPIOG :
        RCC->AHB1RSTR |= (1U << 6) ;           //Reset the GPIOG peripheral
        RCC->AHB1RSTR &= ~(1U << 6) ;         // Clear the reset bit after reset
        break ;

        case (uint32_t)GPIOH :
        RCC->AHB1RSTR |= (1U << 7) ;           //Reset the GPIOH peripheral
        RCC->AHB1RSTR &= ~(1U << 7) ;         // Clear the reset bit after reset
        break ;


        case (uint32_t)GPIOI :
        RCC->AHB1RSTR |= (1U << 8) ;           //Reset the GPIOI peripheral
        RCC->AHB1RSTR &= ~(1U << 8) ;         // Clear the reset bit after reset
        break ;

        default : break ;
    }
}

/*
 * ---------------------------------------------------------
 * Function: GPIO_Pin_Read
 *
 * Description:
 *     Reads the value of the specified GPIO pin.
 *
 * Parameters:
 *     param1 - pGPIOx - Base address of the GPIO peripheral to reset
 *              (e.g. GPIOA, GPIOB, ...).
 *     param2 - uint8_t PinNumber : Pin Number You Want to Read
 *
 * Returns:
 *     The value of the specified GPIO pin.
 * ---------------------------------------------------------
 */

uint8_t GPIO_Pin_Read(GPIO_RegDef_t *pGPIOx , uint8_t PinNumber) {
    return GET_BIT(pGPIOx->IDR , PinNumber) ;
}

/*
 * ---------------------------------------------------------
 * Function: GPIO_Port_Read
 *
 * Description:
 *     Reads the value of the entire GPIO port.
 *
 * Parameters:
 *     param1 - pGPIOx - Base address of the GPIO peripheral to reset
 *              (e.g. GPIOA, GPIOB, ...).
 *
 * Returns:
 *     The value of the entire GPIO port.
 * ---------------------------------------------------------
 */

uint16_t GPIO_Port_Read(GPIO_RegDef_t *pGPIOx) {
    return (uint16_t)pGPIOx->IDR ;
}

/*
 * ---------------------------------------------------------
 * Function: GPIO_Pin_Write
 *
 * Description:
 *     Sets the output state of the specified GPIO pin,
 *     driving it either HIGH or LOW.
 *
 * Parameters:
 *     pGPIOx     - Base address of the GPIO peripheral
 *                  (e.g. GPIOA, GPIOB, ...).
 *     PinNumber  - Pin to write to (0-15).
 *     Value      - Desired output state: HIGH or LOW.
 *
 * Returns:
 *     None
 *
 * Note:
 *     If Value is neither HIGH nor LOW, the function does
 *     nothing.
 * ---------------------------------------------------------
 */
void GPIO_Pin_Write(GPIO_RegDef_t *pGPIOx , uint8_t PinNumber , uint8_t Value) {

if (Value == HIGH) {
    SET_BIT(pGPIOx->ODR , PinNumber) ;
}
else if(Value == LOW)
{
    CLR_BIT(pGPIOx->ODR , PinNumber) ;
}

}

/*
 * ---------------------------------------------------------
 * Function: GPIO_Port_Write
 *
 * Description:
 *      Writes a 16-bit value to the entire GPIO port at once,
 *     setting all 16 pins' output states in a single write.
 *
 * Parameters:
 *     pGPIOx     - Base address of the GPIO peripheral
 *                  (e.g. GPIOA, GPIOB, ...).
 *     Value      - Desired output state: HIGH or LOW.
 *
 * Returns:
 *     None
 * ---------------------------------------------------------
 */
void GPIO_Port_Write(GPIO_RegDef_t *pGPIOx , uint16_t Value) {
    pGPIOx->ODR = Value ;
}

/*
 * ---------------------------------------------------------
 * Function: GPIO_Pin_Toggle
 *
 * Description:
 *     Toggles the output state of the specified GPIO pin,
 *     flipping it from HIGH to LOW or vice versa.
 *
 * Parameters:
 *     pGPIOx     - Base address of the GPIO peripheral
 *                  (e.g. GPIOA, GPIOB, ...).
 *     PinNumber  - Pin to toggle (0-15).
 *
 * Returns:
 *     None
 * ---------------------------------------------------------
 */
void GPIO_Pin_Toggle(GPIO_RegDef_t *pGPIOx , uint8_t PinNumber) {
    TOG_BIT(pGPIOx->ODR , PinNumber) ;
}

void GPIO_IRQ_Config(uint8_t IRQNumber , uint8_t ENorDI) {
    if(ENorDI == ENABLE) {
        //Enable the interrupt in the NVIC (Nested Vectored Interrupt Controller)
        if(IRQNumber <= 31) {
            // Set the corresponding bit in NVIC_ISER0 to enable the interrupt
            *NVIC_ISER0 |= (1U << IRQNumber) ;
        }
        else if(IRQNumber > 31 && IRQNumber < 64) {
            // Set the corresponding bit in NVIC_ISER1 to enable the interrupt
            *NVIC_ISER1 |= (1U << (IRQNumber % 32)) ;
        }
        else if(IRQNumber >= 64 && IRQNumber < 96) {
            // Set the corresponding bit in NVIC_ISER2 to enable the interrupt
            *NVIC_ISER2 |= (1U << (IRQNumber % 64)) ;
        }
    }
    else if(ENorDI == DISABLE) {
        //Disable the interrupt in the NVIC (Nested Vectored Interrupt Controller)
        if(IRQNumber <= 31) {
            // Set the corresponding bit in NVIC_ICER0 to disable the interrupt
            *NVIC_ICER0 |= (1U << IRQNumber) ;
        }
        else if(IRQNumber > 31 && IRQNumber < 64) {
            // Set the corresponding bit in NVIC_ICER1 to disable the interrupt
            *NVIC_ICER1 |= (1U << (IRQNumber % 32)) ;
        }
        else if(IRQNumber >= 64 && IRQNumber < 96) {
            // Set the corresponding bit in NVIC_ICER2 to disable the interrupt
            *NVIC_ICER2 |= (1U << (IRQNumber % 64)) ;
        }
    }
}


//Handle the overflow !!!!!!!!!!!!!!!!!! uint32_t
void GPIO_IRQ_Priority_Config(uint8_t IRQNumber , uint32_t IRQPriority) {
    uint8_t IPRX = IRQNumber / 4 ; // Determine which IPR register to use (0-59)
    uint8_t IPRX_POS = ( (IRQNumber % 4) * 8 ) ; // Determine the position within the IPR register (0-3)
    uint8_t Shift = IPRX_POS + (8 -NO_OF_PR_BITS) ; //The First 4-bits are reserved , shift by (8-NO_PR) = 4  

    uint32_t tmp = *(NVIC_IPR_BASE_ADDR + IPRX);   

    tmp &= ~(0xFFU << IPRX_POS);   // Clear the 8 bits for the interrupt priority
    tmp |= ( IRQPriority << (Shift) );    // Set the priority for the interrupt
    *(NVIC_IPR_BASE_ADDR + IPRX) = tmp;

}

void GPIO_IRQ_Handling(uint8_t PinNumber) {

    if(GET_BIT(EXTI->PR , PinNumber) == 1) {
        //Clear Bit
         EXTI->PR = (1U << PinNumber); 
    }

}
