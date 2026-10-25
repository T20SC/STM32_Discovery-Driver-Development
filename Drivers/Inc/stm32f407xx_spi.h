#include "stm32f407xx.h"
#include "../../Utils/Inc/bitmath.h" 
#include "stm32f407xx_gpio.h" 

/*=== @SPI_SR_Flags ===*/
#define SPI_RXNE_FLAG   0   //SR bit 0
#define SPI_TXE_FLAG    1   //SR bit 1
#define SPI_BSY_FLAG    7   //SR bit 7
/*==========================*/

/*=== @SPI_DeviceMode ===*/ 
#define SPI_Slave  0U
#define SPI_Master 1U
/*=======================*/ 

/*=== @SPI_BusConfig ===*/ 
#define SPI_FD  1U      //Full-Duplex
#define SPI_HD  2U      //Half-Duplex
#define SPI_S_R 3U      //Simplex-Recieve
/*======================*/ 

/*=== @SPI_SclkSpeed ===*/ 
//BR[2:0] Configs the Prescalar
#define SPI_CLK_DIV2    0U  /*BR[2:0] = 000 -> SCLK = f_PCLK / 2*/
#define SPI_CLK_DIV4    1U  /*BR[2:0] = 001 -> SCLK = f_PCLK / 4*/
#define SPI_CLK_DIV8    2U  /*BR[2:0] = 010 -> SCLK = f_PCLK / 8*/
#define SPI_CLK_DIV16   3U  /*BR[2:0] = 011 -> SCLK = f_PCLK / 16*/
#define SPI_CLK_DIV32   4U  /*BR[2:0] = 100 -> SCLK = f_PCLK / 32*/
#define SPI_CLK_DIV64   5U  /*BR[2:0] = 101 -> SCLK = f_PCLK / 64*/
#define SPI_CLK_DIV128  6U  /*BR[2:0] = 110 -> SCLK = f_PCLK / 128*/
#define SPI_CLK_DIV256  7U  /*BR[2:0] = 111 -> SCLK = f_PCLK / 256*/
/*==========================*/ 

/*=== @SPI_DFF ===*/  
#define SPI_DFF_8   0U      //8-Bit Data Format For Recieve and Transmit
#define SPI_DFF_16  1U      //16-Bit Data Format For Recieve and Transmit
/*==========================*/ 

/*=== @SPI_CPOL ===*/   
#define SPI_CPOL_0      0U    //CLK 0 When Idle
#define SPI_CPOL_1      1U    //CLK 1 When Idle
/*==========================*/ 

/*=== @SPI_CPHA ===*/   
#define SPI_CPHA_0      0U    //First CLK Edge is Reads Second Loads
#define SPI_CPHA_1      1U    //First CLK Edge is Loads Second Reads
/*==========================*/ 

/*=== @SPI_SSM ===*/   
#define SPI_SSM_DIS      0U    //Disable Software Slave Management
#define SPI_SSM_EN       1U    //Enable Software Slave Management
/*==========================*/  

/*==========================*/  
#define SPI_SSI_EN  1U 
#define SPI_SSI_DIS 0U
/*==========================*/ 

typedef struct { 

    uint8_t SPI_DeviceMode ;   //Options at @SPI_DeviceMode
    uint8_t SPI_BusConfig ;    //Options at @SPI_BusConfig
    uint8_t SPI_SclkSpeed ;    //Options at @SPI_SclkSpeed 
    uint8_t SPI_DFF ;          //Options at @SPI_DFF
    uint8_t SPI_CPOL ;         //Options at @SPI_CPOL
    uint8_t SPI_CPHA ;         //Options at @SPI_CPHA
    uint8_t SPI_SSM ;          //Options at @SPI_SSM 
    uint8_t SPI_SSI ;

}SPI_Config_t;

typedef struct { 

    SPI_RegDef_t *pSPIx ;  //Holds the Base Address of SPIx(0:2) Peripheral 
    SPI_Config_t SPIConfig ; //Holds the SPIx(0:2) Configs

}SPI_Handle_t; 

/*
 * Returns the state of a flag in the SPI status register (SR).
 * FlagName: one of the SPI_FLAG macros (e.g. TXE, RXNE, BSY).
 * Returns FLAG_SET or FLAG_RESET.
 */
uint8_t SPI_GetFlagStatus(SPI_RegDef_t *pSPIx , uint8_t FlagName) ;

/*
 * Enables or disables the peripheral clock (RCC) for the given SPI.
 * ENorDI: ENABLE or DISABLE.
 */
void SPI_CLK_Control(SPI_RegDef_t *pSPIx , uint8_t ENorDI) ; 

/*
 * Initializes the SPI peripheral from the config in the handle
 * (device mode, bus config, CPOL, CPHA, DFF, SSM, clock speed)
 * by writing CR1/CR2.
 */
void SPI_Init(SPI_Handle_t *pSPIHandle) ; 

/*
 * Resets the given SPI peripheral to its default state
 * (via the RCC reset register).
 */
void SPI_DeInit(SPI_RegDef_t *pSPIx) ; 

/*
 * Blocking transmit: sends Len bytes from pTXBuffer,
 * waiting on the TXE flag before writing each frame to DR.
 */
void SPI_Send_Data(SPI_RegDef_t *pSPIx , uint8_t *pTXBuffer , uint32_t Len) ; 

/*
 * Blocking receive: reads Len bytes from DR into the buffer,
 * waiting on the RXNE flag before each read.
 * Note: the buffer parameter is named pTXBuffer but it is the RX destination.
 */
void SPI_Recieve_Data(SPI_RegDef_t *pSPIx , uint8_t *pRXBuffer , uint32_t Len) ;

/*
 * Enables or disables the given SPI interrupt line in the NVIC (ISER/ICER).
 * IRQNumber: IRQ number of the SPI peripheral.
 */
void SPI_IRQ_Config(uint8_t IRQNumber , uint8_t ENorDI) ;

/*
 * Sets the NVIC priority of the given SPI interrupt.
 * IRQPriority: priority value to write to the NVIC IPR register.
 */
void SPI_IRQ_Priority_Config(uint8_t IRQNumber , uint32_t IRQPriority) ;

/*
 * Handles an SPI interrupt: checks which event occurred
 * (TXE, RXNE, error) and processes it. Call from the SPIx_IRQHandler.
 */
void SPI_IRQ_Handling(SPI_Handle_t *pSPIHandle) ;  

/*
 * Enables or disables the SPI peripheral itself (SPE bit in CR1).
 * Must be enabled after SPI_Init and before transmitting.
 */
void SPI_SPE_Control(SPI_RegDef_t *pSPIx , uint8_t ENorDI) ; 


   
