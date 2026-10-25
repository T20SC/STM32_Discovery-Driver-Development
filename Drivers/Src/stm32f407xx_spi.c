#include "../Inc/stm32f407xx_spi.h"

/*
 * ---------------------------------------------------------
 * Function: SPI_GetFlagStatus
 *
 * Description:
 *      Checks whether a specific flag bit in the SPI
 *      status register (SR) is set or cleared.
 *
 * Parameters:
 *      SPI_RegDef_t *pSPIx
 *          Pointer to the SPI peripheral whose SR is checked.
 *
 *      uint8_t FlagName
 *          Bit position of the flag inside SR
 *          (e.g. TXE = bit 1, RXNE = bit 0, BSY = bit 7).
 *
 * Returns:
 *      uint8_t
 *          1 (SET)   if the flag bit is 1
 *          0 (RESET) if the flag bit is 0
 * ---------------------------------------------------------
 */
uint8_t SPI_GetFlagStatus(SPI_RegDef_t *pSPIx , uint8_t FlagName) { 
    if(GET_BIT(pSPIx->SR , FlagName)) { 
        return SET; 
    } 
    return RESET; 
}

/*
 * ---------------------------------------------------------
 * Function: SPI_CLK_Control
 *
 * Description:
 *      Enables or disables the peripheral clock for the
 *      selected SPI peripheral through the RCC registers.
 *
 * Parameters:
 *      SPI_RegDef_t *pSPIx
 *          Pointer to the SPI peripheral whose clock will
 *          be enabled or disabled.
 *
 *      uint8_t ENorDI
 *          Control value:
 *              ENABLE  -> Enable SPI peripheral clock
 *              DISABLE -> Disable SPI peripheral clock
 *
 * Returns:
 *      None
 *
 * Notes:
 *      SPI1 is connected to APB2.
 *      SPI2 and SPI3 are connected to APB1.
 * ---------------------------------------------------------
 */

void SPI_CLK_Control(SPI_RegDef_t *pSPIx , uint8_t ENorDI) { 
    if(ENorDI == ENABLE) { 
        //Identify which SPI peripheral pSPIx points to, then enable its clock
        switch((uint32_t) pSPIx) { 
            case (uint32_t) SPI1:  
            SET_BIT(RCC->APB2ENR , RCC_SPI1EN) ;  
            break; 

            case (uint32_t) SPI2: 
            SET_BIT(RCC->APB1ENR , RCC_SPI2EN) ;  
            break;

            case (uint32_t) SPI3: 
            SET_BIT(RCC->APB1ENR , RCC_SPI3EN) ;  
            break; 

            default:  
            //Invalid Option
            break;
        }
    } 
    else if(ENorDI == DISABLE) {  
        //Identify which SPI peripheral pSPIx points to, then disable its clock
        switch((uint32_t) pSPIx) {  
            case (uint32_t) SPI1:  
            CLR_BIT(RCC->APB2ENR , RCC_SPI1EN) ;  
            break; 

            case (uint32_t) SPI2:  
            CLR_BIT(RCC->APB1ENR , RCC_SPI2EN) ;  
            break;

            case (uint32_t) SPI3: 
            CLR_BIT(RCC->APB1ENR , RCC_SPI3EN) ;  
            break; 

            default:  
            //Invalid Option 
            break;
        }
    }
} 

/*
 * ---------------------------------------------------------
 * Function: SPI_Init
 *
 * Description:
 *      Initializes the SPI peripheral according to the
 *      configuration parameters stored inside the SPI handle.
 *
 *      The function configures:
 *          - SPI device mode (Master / Slave)
 *          - SPI bus configuration
 *          - SPI clock speed
 *          - Clock polarity (CPOL)
 *          - Clock phase (CPHA)
 *
 * Parameters:
 *      SPI_Handle_t *pSPIHandle
 *          Pointer to the SPI handle containing:
 *              - SPI peripheral base address
 *              - SPI configuration structure
 *
 * Returns:
 *      None
 *
 * Notes:
 *      The function only configures the SPI control
 *      registers. The SPI peripheral clock should be
 *      enabled before calling this function (via
 *      SPI_CLK_Control), since writes to an unclocked
 *      peripheral's registers may not take effect.
 * -------------------------------------------------------- 
 */
void SPI_Init(SPI_Handle_t *pSPIHandle) {  

    if(pSPIHandle->SPIConfig.SPI_SSI == SPI_SSI_EN) { 
        SET_BIT(pSPIHandle->pSPIx->CR1 , SPI_CR1_SSI) ; 
    } 
    else if(pSPIHandle->SPIConfig.SPI_SSI == SPI_SSI_DIS) { 
        CLR_BIT(pSPIHandle->pSPIx->CR1 , SPI_CR1_SSI) ;
    }


    if(pSPIHandle->SPIConfig.SPI_SSM == SPI_SSM_EN) { 
        SET_BIT(pSPIHandle->pSPIx->CR1 , SPI_CR1_SSM) ;
    }  
    else if(pSPIHandle->SPIConfig.SPI_SSM == SPI_SSM_DIS) { 
        CLR_BIT(pSPIHandle->pSPIx->CR1 , SPI_CR1_SSM) ;
    }  

    
    //CR1 bit 2 (MSTR): 1 = master mode, 0 = slave mode
    if(pSPIHandle->SPIConfig.SPI_DeviceMode == SPI_Master) { 
        SET_BIT(pSPIHandle->pSPIx->CR1 , SPI_CR1_MSTR);  
    }  
    else if(pSPIHandle->SPIConfig.SPI_DeviceMode == SPI_Slave) { 
        CLR_BIT(pSPIHandle->pSPIx->CR1 , SPI_CR1_MSTR); 
    } 

    //CR1 bit 15 (BIDIMODE) and bit 10 (RXONLY) select the data line configuration
    switch(pSPIHandle->SPIConfig.SPI_BusConfig) { 
        case SPI_FD : 
        //2-line full duplex: BIDIMODE cleared, MOSI and MISO both active
        CLR_BIT(pSPIHandle->pSPIx->CR1 , SPI_CR1_BIDIMODE) ; 
        break ; 

        case SPI_HD :  
        //1-line half duplex: BIDIMODE set, single shared data line
        SET_BIT(pSPIHandle->pSPIx->CR1 , SPI_CR1_BIDIMODE) ; 
        break; 

        case SPI_S_R : 
        //Simplex receive-only: 2-line mode (BIDIMODE cleared) but RXONLY set
        //so the peripheral only clocks data in on MISO
        CLR_BIT(pSPIHandle->pSPIx->CR1 , SPI_CR1_BIDIMODE) ; 
        SET_BIT(pSPIHandle->pSPIx->CR1 , SPI_CR1_RXONLY) ; 
        break; 

        default:  
        //Invalid Option
        break;
    } 

    //CR1 bits 5:3 (BR): baud rate prescaler, SCLK = f_PCLK / prescaler
    //Using |= so only these bits change, leaving MSTR/BIDIMODE/RXONLY above untouched
    pSPIHandle->pSPIx->CR1 &= ~SPI_CR1_BR_MSK ;
    pSPIHandle->pSPIx->CR1 |= pSPIHandle->SPIConfig.SPI_SclkSpeed << SPI_CR1_BR; 

    //CR1 bit 1 (CPOL): clock idle level, 0 = idle low, 1 = idle high
    if(pSPIHandle->SPIConfig.SPI_CPOL == SPI_CPOL_0) { 
        CLR_BIT(pSPIHandle->pSPIx->CR1 , SPI_CR1_CPOL) ; 
    } 
    else if (pSPIHandle->SPIConfig.SPI_CPOL == SPI_CPOL_1) { 
        SET_BIT(pSPIHandle->pSPIx->CR1 , SPI_CR1_CPOL) ; 
    }
    
    //CR1 bit 0 (CPHA): sample edge, 0 = sample on 1st edge, 1 = sample on 2nd edge
    if(pSPIHandle->SPIConfig.SPI_CPHA == SPI_CPHA_0) { 
        CLR_BIT(pSPIHandle->pSPIx->CR1 , SPI_CR1_CPHA) ; 
    } 
    else if (pSPIHandle->SPIConfig.SPI_CPHA == SPI_CPHA_1) { 
        SET_BIT(pSPIHandle->pSPIx->CR1 , SPI_CR1_CPHA) ; 
    }  
}

/*
 * ---------------------------------------------------------
 * Function: SPI_DeInit
 *
 * Description:
 *      Resets the selected SPI peripheral back to its
 *      default power-up state by pulsing its reset bit
 *      in the RCC reset register.
 *
 * Parameters:
 *      SPI_RegDef_t *pSPIx
 *          Pointer to the SPI peripheral to reset.
 *
 * Returns:
 *      None
 *
 * Notes:
 *      SPI1 is reset via APB2RSTR.
 *      SPI2 and SPI3 are reset via APB1RSTR.
 *      Setting the bit holds the peripheral in reset;
 *      clearing it immediately after releases it, wiping
 *      all its registers back to their reset values.
 *      (POLLING BASED FUNCTION)
 * ---------------------------------------------------------
 */
void SPI_DeInit(SPI_RegDef_t *pSPIx) { 
    //Identify which SPI peripheral pSPIx points to, then pulse its reset bit
    switch((uint32_t) pSPIx) { 
        case(uint32_t) SPI1: 
        SET_BIT(RCC->APB2RSTR , RCC_SPI1EN) ; 
        CLR_BIT(RCC->APB2RSTR , RCC_SPI1EN) ; 
        break;  

        case(uint32_t) SPI2: 
        SET_BIT(RCC->APB1RSTR , RCC_SPI2EN) ; 
        CLR_BIT(RCC->APB1RSTR , RCC_SPI2EN) ; 
        break; 

        case(uint32_t) SPI3: 
        SET_BIT(RCC->APB1RSTR , RCC_SPI3EN) ; 
        CLR_BIT(RCC->APB1RSTR , RCC_SPI3EN) ; 
        break; 

        default: 
        //Invalid Option
        break;
    }
}

/*
 * ---------------------------------------------------------
 * Function: SPI_Send_Data
 *
 * Description:
 *      Transmits data through the SPI peripheral using the
 *      data frame format configured in the SPI control register.
 *
 *      The function supports:
 *          - 8-bit data frame format
 *          - 16-bit data frame format
 *
 *      In 8-bit mode, one byte is transmitted at a time.
 *      In 16-bit mode, two bytes are transmitted as one
 *      complete SPI data frame.
 *
 * Parameters:
 *      SPI_RegDef_t *pSPIx
 *          Pointer to the SPI peripheral.
 *
 *      uint8_t *pTXBuffer
 *          Pointer to the transmit data buffer.
 *
 *      uint32_t Len
 *          Number of bytes to transmit.
 *
 * Returns:
 *      None
 *
 * Notes:
 *      - The function waits until the TXE flag is set before
 *        loading new data into the SPI data register.
 *
 *      - In 16-bit mode, Len must be EVEN because each SPI
 *        data frame consists of 2 bytes.
 *
 *      - If Len is odd while using 16-bit mode, the function
 *        stops transmission because the remaining byte cannot
 *        form a complete 16-bit data frame.
 *
 *      - The SPI peripheral clock should be enabled before
 *        calling this function. 
 *        (POLLING BASED FUNCTION)
 * ---------------------------------------------------------
 */
void SPI_Send_Data(SPI_RegDef_t *pSPIx , uint8_t *pTXBuffer , uint32_t Len) { 
    while (Len > 0 ){  

        while(SPI_GetFlagStatus(pSPIx , SPI_TXE_FLAG) == RESET) { 
            //Wait until is TXE Set 
        }  

        //8-Bit Data Fram Format 
        if(GET_BIT(pSPIx->CR1 , SPI_CR1_DFF) == SPI_DFF_8) {  
            //load  the data register
            pSPIx->DR = *pTXBuffer;
            pTXBuffer++;
            Len--;
        }  

        //16-Bit Data Fram Format 
        else if(GET_BIT(pSPIx->CR1 , SPI_CR1_DFF) == SPI_DFF_16) { 
            //load  the data register 
             if ((Len%2) == 1)
            {
                // Cannot send a complete 16-bit frame
                break;
            }
            pSPIx->DR = *((uint16_t *)pTXBuffer) ; 
            pTXBuffer += 2 ; 
            Len -= 2 ; 
        }

    }
} 

/*
 * ---------------------------------------------------------
 * Function: SPI_SPE_Control
 *
 * Description:
 *      Enables or disables the SPI peripheral by setting
 *      or clearing the SPE bit in CR1.
 *
 * Parameters:
 *      SPI_RegDef_t *pSPIx
 *          Pointer to the SPI peripheral.
 *
 *      uint8_t ENorDI
 *          Control value:
 *              ENABLE  -> Enable the SPI peripheral
 *              DISABLE -> Disable the SPI peripheral
 *
 * Returns:
 *      None
 *
 * Notes:
 *      - Configure CR1/CR2 (via SPI_Init) before enabling SPE.
 *      - Some CR1 bits (BR, CPOL, CPHA, DFF, LSBFIRST, MSTR)
 *        must not be changed while SPE is set.
 *      - Before disabling, wait for TXE = 1 and then BSY = 0
 *        so the last frame is not cut off.
 * ---------------------------------------------------------
 */
void SPI_SPE_Control(SPI_RegDef_t *pSPIx , uint8_t ENorDI) {  
    //bit number 6 of the CR1 register is SPE (SPI Enable)
    if (ENorDI == ENABLE) { 
        SET_BIT(pSPIx->CR1 , SPI_CR1_SPE) ; 
    } 
    else if (ENorDI == DISABLE) { 
        CLR_BIT(pSPIx->CR1 , SPI_CR1_SPE) ; 
    }
} 

/*
 * ---------------------------------------------------------
 * Function: SPI_Recieve_Data
 *
 * Description:
 *      Receives data from the SPI peripheral using the
 *      data frame format configured in CR1 (DFF).
 *
 *      The function supports:
 *          - 8-bit data frame format
 *          - 16-bit data frame format
 *
 *      In 8-bit mode, one byte is read at a time.
 *      In 16-bit mode, two bytes are read as one complete
 *      SPI data frame.
 *
 * Parameters:
 *      SPI_RegDef_t *pSPIx
 *          Pointer to the SPI peripheral.
 *
 *      uint8_t *pRXBuffer
 *          Pointer to the receive data buffer.
 *
 *      uint32_t Len
 *          Number of bytes to receive.
 *
 * Returns:
 *      None
 *
 * Notes:
 *      - The function waits until the RXNE flag is set before
 *        reading the SPI data register.
 *
 *      - In 16-bit mode, Len must be EVEN because each SPI
 *        data frame consists of 2 bytes. With an odd Len the
 *        function stops without reading.
 *
 *      - The SPI peripheral clock must be enabled and SPE set
 *        before calling this function.
 *
 *      - In full-duplex master mode the clock only runs when
 *        data is written to DR, so a dummy write is needed
 *        for each frame, otherwise RXNE never sets.
 *        (POLLING BASED FUNCTION)
 * ---------------------------------------------------------
 */
void SPI_Recieve_Data(SPI_RegDef_t *pSPIx , uint8_t *pRXBuffer , uint32_t Len) { 
    while (Len > 0 ){  
 
        while(SPI_GetFlagStatus(pSPIx , SPI_RXNE_FLAG) == RESET) { 
            //Wait until is RXNE Set 
        }  

        //8-Bit Data Fram Format 
        if(GET_BIT(pSPIx->CR1 , SPI_CR1_DFF) == SPI_DFF_8) {  
            //Read the data register
            *pRXBuffer = pSPIx->DR ;
            pRXBuffer++;
            Len--;
        }  

        //16-Bit Data Fram Format 
        else if(GET_BIT(pSPIx->CR1 , SPI_CR1_DFF) == SPI_DFF_16) { 
            //load  the data register 
             if ((Len%2) == 1)
            {
                // Cannot receive a complete 16-bit frame
                break;
            }
            *((uint16_t *)pRXBuffer) = pSPIx->DR ; 
            pRXBuffer += 2 ; 
            Len -= 2 ; 
        }
    }
}