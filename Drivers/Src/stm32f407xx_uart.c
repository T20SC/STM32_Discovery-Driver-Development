#include "../Inc/stm32f407xx_uart.h"   
#include "../Inc/stm32f407xx_rcc.h"

/*
 * Returns the state (0 or 1) of one flag in the Status Register (SR).
 * FlagName is a bit position such as UART_SR_TXE or UART_SR_TC.
 */
uint8_t UART_GetFlag_Status(UART_RegDef_t *pUARTx , uint8_t FlagName) {
    // CTS is the highest implemented flag in SR, so anything above it is not a valid flag
    if (FlagName > UART_SR_CTS) {
        return 0 ;                         // invalid flag name
    }
    // read the requested bit out of SR
    return GET_BIT(pUARTx->SR , FlagName) ;
}
 
/*
 * Enables or disables the peripheral clock of a UART/USART in RCC.
 * USART1 and USART6 sit on APB2, the others sit on APB1.
 * Registers are not accessible until the clock is enabled.
 */
void UART_CLK_Control(UART_RegDef_t *pUARTx , uint8_t ENorDI) { 
    if(ENorDI == ENABLE) { 
        // pick the right enable bit by comparing the peripheral base address
        switch((uint32_t)pUARTx) { 
            case (uint32_t)USART1 : 
            SET_BIT(RCC->APB2ENR , RCC_USART1EN) ;      // USART1 -> APB2
            break ;  
 
            case (uint32_t)USART2 : 
            SET_BIT(RCC->APB1ENR , RCC_USART2EN) ;      // USART2 -> APB1
            break ;
 
            case (uint32_t)USART3 : 
            SET_BIT(RCC->APB1ENR , RCC_USART3EN) ;      // USART3 -> APB1
            break ;
 
            case (uint32_t)UART4 : 
            SET_BIT(RCC->APB1ENR , RCC_UART4EN) ;       // UART4 -> APB1
            break ;
 
            case (uint32_t)UART5 : 
            SET_BIT(RCC->APB1ENR , RCC_UART5EN) ;       // UART5 -> APB1
            break ;
 
            case (uint32_t)USART6 : 
            SET_BIT(RCC->APB2ENR , RCC_USART6EN) ;      // USART6 -> APB2
            break ; 
            
            default :                                   // unknown peripheral, do nothing
            break ;
        }
    } 
    else if(ENorDI == DISABLE) { 
        // same mapping as above, but clearing the enable bits
        switch((uint32_t)pUARTx) { 
            case (uint32_t)USART1 : 
            CLR_BIT(RCC->APB2ENR , RCC_USART1EN) ; 
            break ;  
 
            case (uint32_t)USART2 : 
            CLR_BIT(RCC->APB1ENR , RCC_USART2EN) ; 
            break ;
 
            case (uint32_t)USART3 : 
            CLR_BIT(RCC->APB1ENR , RCC_USART3EN) ; 
            break ;
 
            case (uint32_t)UART4 : 
            CLR_BIT(RCC->APB1ENR , RCC_UART4EN) ; 
            break ;
 
            case (uint32_t)UART5 : 
            CLR_BIT(RCC->APB1ENR , RCC_UART5EN) ; 
            break ;
 
            case (uint32_t)USART6 : 
            CLR_BIT(RCC->APB2ENR , RCC_USART6EN) ; 
            break ; 
            
            default :
            break ;
        }
    } 
}


/*
 * Computes and writes the BRR register for the requested baud rate.
 *
 * Baud = fPCLK / (8 * (2 - OVER8) * USARTDIV)
 * so USARTDIV = fPCLK / (8 * (2 - OVER8) * Baud)
 *
 * To avoid floating point, USARTDIV is scaled by 100 (UART_DIV = USARTDIV * 100):
 *   OVER8 = 1: UART_DIV = 100*PCLK / (8*Baud)  = 25*PCLK / (2*Baud)
 *   OVER8 = 0: UART_DIV = 100*PCLK / (16*Baud) = 25*PCLK / (4*Baud)
 *
 * BRR layout: DIV_Mantissa in the upper field, DIV_Fraction in the lower field
 * (with OVER8 = 1 the fraction is only 3 bits wide and the top fraction bit must stay 0)
 */
void UART_BR_Control(UART_RegDef_t *pUARTx , uint32_t BaudRate) { 
    uint32_t PCLK ; 
    uint32_t Mantissa , Fraction ; 
    uint32_t UART_DIV ;  
    uint32_t Temp = 0 ; 
 
    // USART1 and USART6 are clocked from APB2, the rest from APB1
    if(pUARTx == USART1 || pUARTx == USART6) 
    { 
        PCLK = RCC_GetAPB2_CLK() ;
    }
 
    else 
    { 
        PCLK = RCC_GetAPB1_CLK() ;
    }
 
    // scaled USARTDIV (x100), formula depends on the oversampling mode
    if(GET_BIT(pUARTx->CR1 , UART_CR1_OVER8) == 1) {
        UART_DIV = ( (25*PCLK) / (2*BaudRate) ) ;
    } 
 
    else if(GET_BIT(pUARTx->CR1 , UART_CR1_OVER8) == 0) {
        UART_DIV = ( (25*PCLK) / (4*BaudRate) ) ;
    }
 
    // integer part of USARTDIV goes into the mantissa field
    Mantissa = UART_DIV / 100 ;
    Temp |= Mantissa << UART_BRR_MANT_L ; 
 
    // remaining two decimal digits (0..99) are the fractional part x100
    Fraction = ( UART_DIV - (Mantissa*100) ) ; 
 
    if(GET_BIT(pUARTx->CR1 , UART_CR1_OVER8) == 1) 
    { 
        // Oversampling by 8
        // fraction field is 3 bits: round(Fraction/100 * 8), keep 3 bits
        Fraction = (((Fraction * 8) + 50) / 100) & 0x07;
    } 
    
    else if(GET_BIT(pUARTx->CR1 , UART_CR1_OVER8) == 0) { 
        //Oversampling by 16
        // fraction field is 4 bits: round(Fraction/100 * 16), keep 4 bits
        Fraction = (((Fraction * 16) + 50) / 100) & 0x0F;
    }
 
    // combine mantissa and fraction and write the register
    Temp |= Fraction;
    pUARTx->BRR = Temp ;
}



/*
 * Configures a UART from the settings stored in the handle:
 * word length, parity, stop bits, mode (TX/RX), hardware flow control,
 * oversampling and baud rate. UE (UART enable) is set last.
 */
void UART_Init(UART_Handle_t *pUARTHandle) { 
 
    // enable the peripheral clock first, otherwise writes to the registers have no effect
    UART_CLK_Control(pUARTHandle->pUARTx , ENABLE) ;
 
        // ---- Word length: CR1.M (0 = 8 data bits, 1 = 9 data bits) ----
        switch(pUARTHandle->UARTConfig.UART_WordLength) { 
            case UART_WordLength_8: 
            CLR_BIT(pUARTHandle->pUARTx->CR1 , UART_CR1_M) ; 
            break; 
 
            case UART_WordLength_9: 
            SET_BIT(pUARTHandle->pUARTx->CR1 , UART_CR1_M) ;
            break ; 
 
            default: 
            break ;
        } 
 
        // ---- Parity: CR1.PCE enables parity, CR1.PS selects even (0) or odd (1) ----
        switch(pUARTHandle->UARTConfig.UART_ParityControl) { 
            case UART_ParityControl_None: 
            CLR_BIT(pUARTHandle->pUARTx->CR1 , UART_CR1_PCE) ;      // parity disabled
            break ; 
 
            case UART_ParityControl_Even: 
            SET_BIT(pUARTHandle->pUARTx->CR1 , UART_CR1_PCE) ;      // parity enabled
            CLR_BIT(pUARTHandle->pUARTx->CR1 , UART_CR1_PS) ;       // PS = 0 -> even
            break ; 
 
            case UART_ParityControl_Odd: 
            SET_BIT(pUARTHandle->pUARTx->CR1 , UART_CR1_PCE) ;      // parity enabled
            SET_BIT(pUARTHandle->pUARTx->CR1 , UART_CR1_PS) ;       // PS = 1 -> odd
            break; 
 
            default: 
            break;
        }
 
        // ---- Stop bits: CR2.STOP is a 2-bit field ----
        // 00 = 1 stop bit, 01 = 0.5, 10 = 2, 11 = 1.5
        // UART_CR2_STOP is the lower bit of the field, UART_CR2_STOP + 1 is the upper bit
        switch(pUARTHandle->UARTConfig.UART_NStopBits) { 
            case UART_NStopBits_1:                                       // 00
            CLR_BIT(pUARTHandle->pUARTx->CR2 , UART_CR2_STOP) ;  
            CLR_BIT(pUARTHandle->pUARTx->CR2 , (UART_CR2_STOP + 1)) ;
            break ; 
 
            case UART_NStopBits_0_5:                                     // 01
            SET_BIT(pUARTHandle->pUARTx->CR2 , UART_CR2_STOP) ;  
            CLR_BIT(pUARTHandle->pUARTx->CR2 , (UART_CR2_STOP + 1)) ;
            break; 
 
            case UART_NStopBits_2:                                       // 10
            CLR_BIT(pUARTHandle->pUARTx->CR2 , UART_CR2_STOP) ;  
            SET_BIT(pUARTHandle->pUARTx->CR2 , (UART_CR2_STOP + 1)) ;
            break ; 
 
            case UART_NStopBits_1_5:                                     // 11
            SET_BIT(pUARTHandle->pUARTx->CR2 , UART_CR2_STOP) ;  
            SET_BIT(pUARTHandle->pUARTx->CR2 , (UART_CR2_STOP + 1)) ;
            break; 
 
            default: 
            break; 
        }
 
 
        // ---- Direction: CR1.TE enables the transmitter, CR1.RE enables the receiver ----
        switch(pUARTHandle->UARTConfig.UART_Mode) { 
            case UART_Mode_TX:                                           // transmit only
            CLR_BIT(pUARTHandle->pUARTx->CR1 , UART_CR1_RE) ;
            SET_BIT(pUARTHandle->pUARTx->CR1 , UART_CR1_TE) ;
            break ; 
 
            case UART_Mode_RX:                                           // receive only
            CLR_BIT(pUARTHandle->pUARTx->CR1 , UART_CR1_TE) ;
            SET_BIT(pUARTHandle->pUARTx->CR1 , UART_CR1_RE) ;
            break ; 
 
            case UART_Mode_RXTX:                                         // both directions
            SET_BIT(pUARTHandle->pUARTx->CR1 , UART_CR1_RE) ;
            SET_BIT(pUARTHandle->pUARTx->CR1 , UART_CR1_TE) ;
            break ; 
 
            default:
            break;
        }
 
        // ---- Hardware flow control: CR3.CTSE (CTS enable) and CR3.RTSE (RTS enable) ----
        switch(pUARTHandle->UARTConfig.UART_HWFlowControl) { 
            case UART_HW_FC_NONE:                                        // no flow control
            CLR_BIT(pUARTHandle->pUARTx->CR3 , UART_CR3_CTSE) ; 
            CLR_BIT(pUARTHandle->pUARTx->CR3 , UART_CR3_RTSE) ;
            break; 
 
            case UART_HW_FC_CTS:                                         // CTS only
            SET_BIT(pUARTHandle->pUARTx->CR3 , UART_CR3_CTSE) ; 
            CLR_BIT(pUARTHandle->pUARTx->CR3 , UART_CR3_RTSE) ;
            break; 
 
            case UART_HW_FC_RTS:                                         // RTS only
            CLR_BIT(pUARTHandle->pUARTx->CR3 , UART_CR3_CTSE) ; 
            SET_BIT(pUARTHandle->pUARTx->CR3 , UART_CR3_RTSE) ;
            break; 
 
            case UART_HW_FC_CTS_RTS:                                     // CTS and RTS
            SET_BIT(pUARTHandle->pUARTx->CR3 , UART_CR3_CTSE) ; 
            SET_BIT(pUARTHandle->pUARTx->CR3 , UART_CR3_RTSE) ;
            break; 
            
            default:
            break; 
        }
        //Configure OVER8 and BRR Later 
        
        // ---- Oversampling: CR1.OVER8 (0 = by 16, 1 = by 8) ----
        // must be set before computing BRR because the baud formula depends on it
        switch(pUARTHandle->UARTConfig.UART_OverSampling) { 
            case UART_OVER_16: 
                CLR_BIT(pUARTHandle->pUARTx->CR1 , UART_CR1_OVER8) ;
            break; 
 
    
            case UART_OVER_8: 
                SET_BIT(pUARTHandle->pUARTx->CR1 , UART_CR1_OVER8) ;
            break; 
 
            default: 
            break;
 
        }
 
        // compute and write the baud rate register (mantissa + fraction)
        UART_BR_Control(pUARTHandle->pUARTx , pUARTHandle->UARTConfig.UART_BaudRate) ;
 
        // finally enable the UART (CR1.UE) once everything is configured
        SET_BIT(pUARTHandle->pUARTx->CR1 , UART_CR1_UE) ;
}
 

/*
 * Resets one UART/USART back to its power-on register values using RCC reset registers.
 * Pulse the reset bit: set it, then clear it.
 * The reset bit positions in APBxRSTR match the enable bit positions in APBxENR,
 * which is why the RCC_xxxEN macros are reused here.
 */
void UART_DeInit(UART_RegDef_t *pUARTx) { 
    switch((uint32_t)pUARTx) { 
            case (uint32_t)USART1 :  
            SET_BIT(RCC->APB2RSTR , RCC_USART1EN) ;     // assert reset
            CLR_BIT(RCC->APB2RSTR , RCC_USART1EN) ;     // release reset
            break ;  
 
            case (uint32_t)USART2 :  
            SET_BIT(RCC->APB1RSTR , RCC_USART2EN) ; 
            CLR_BIT(RCC->APB1RSTR , RCC_USART2EN) ; 
            break ;
 
            case (uint32_t)USART3 :  
            SET_BIT(RCC->APB1RSTR , RCC_USART3EN) ; 
            CLR_BIT(RCC->APB1RSTR , RCC_USART3EN) ; 
            break ;
 
            case (uint32_t)UART4 :  
            SET_BIT(RCC->APB1RSTR , RCC_UART4EN) ;
            CLR_BIT(RCC->APB1RSTR , RCC_UART4EN) ; 
            break ;
 
            case (uint32_t)UART5 :  
            SET_BIT(RCC->APB1RSTR , RCC_UART5EN) ;
            CLR_BIT(RCC->APB1RSTR , RCC_UART5EN) ; 
            break ;
 
            case (uint32_t)USART6 :  
            SET_BIT(RCC->APB2RSTR , RCC_USART6EN) ; 
            CLR_BIT(RCC->APB2RSTR , RCC_USART6EN) ; 
            break ; 
            
            default :
            break ;
        }
}  
 
/*
 * Blocking transmit of Len frames.
 * 8-bit word: one byte per frame.
 * 9-bit word, no parity: 9 data bits, so each frame consumes 2 bytes of the buffer.
 * 9-bit word with parity: 8 data bits + parity bit, so each frame consumes 1 byte.
 */
void UART_SendData(UART_Handle_t *pUARTHandle ,uint8_t *pTxBuffer, uint32_t Len) { 
    uint16_t *pData ;  
 
    for(int i = 0 ; i< Len ; i++) { 
 
        // wait until the data register is empty so the next frame can be written
        while(UART_GetFlag_Status(pUARTHandle->pUARTx , UART_SR_TXE) == 0) { 
            //loop until the TXE Flag is set (Transmit Empty)
        }
        if(pUARTHandle->UARTConfig.UART_WordLength == UART_WordLength_9) { 
            // treat the buffer as 16-bit data and keep only the lower 9 bits
            pData = (uint16_t *)pTxBuffer ; 
            pUARTHandle->pUARTx->DR = (*pData & (uint16_t)0x01FF) ;
            if(pUARTHandle->UARTConfig.UART_ParityControl == UART_ParityControl_None) { 
                pTxBuffer += 2 ;        // all 9 bits are data, so move to the next 16-bit value
            } 
 
            else { 
                pTxBuffer++ ;           // 8 data bits + parity, so only one byte was used
            }
        } 
 
        else if (pUARTHandle->UARTConfig.UART_WordLength == UART_WordLength_8) { 
 
            // write one byte; writing DR also clears the TXE flag
            pUARTHandle->pUARTx->DR = (*pTxBuffer  & (uint8_t)0xFF); 
            pTxBuffer++ ;
           
        } 
 
    } 
    // after the last frame, wait until the shift register has finished sending it
    while(UART_GetFlag_Status(pUARTHandle->pUARTx , UART_SR_TC) == 0) { 
            //loop until the TC Flag is set (Transmit complete)
    }
}
