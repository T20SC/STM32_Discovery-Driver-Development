#ifndef STM32F407XX_UART_H_
#define STM32F407XX_UART_H_

#include "stm32f407xx.h"                // MCU header: UART_RegDef_t, USARTx/UARTx base addresses, RCC
#include "../../Utils/Inc/bitmath.h"    // SET_BIT / CLR_BIT macros
#include "stm32f407xx_gpio.h"           // GPIO driver (needed for the UART pins' alternate function setup)

/*=================*/

/*=== @UART_Mode ===*/
// Selects which directions are enabled (maps to CR1: TE / RE)
#define UART_Mode_TX            0U      // transmitter only (TE = 1, RE = 0)
#define UART_Mode_RX            1U      // receiver only    (TE = 0, RE = 1)
#define UART_Mode_RXTX          2U      // both directions  (TE = 1, RE = 1)
/*========================*/

/*=== @UART_WordLength ===*/
// Frame length in bits, PARITY BIT INCLUDED (maps to CR1: M)
// 8E1 / 8O1 (8 data bits + parity) therefore needs UART_WordLength_9
#define UART_WordLength_8       0U      // 8 bits per frame (M = 0)
#define UART_WordLength_9       1U      // 9 bits per frame (M = 1)
/*========================*/

/*=== @UART_NStopBits ===*/
// Number of stop bits (maps to CR2: STOP[13:12])
// Values are STOP field encodings. UART4/5 support only 1 and 2 stop bits
#define UART_NStopBits_1        0U      // 1 stop bit   (STOP = 00)
#define UART_NStopBits_0_5      1U      // 0.5 stop bit (STOP = 01)
#define UART_NStopBits_2        2U      // 2 stop bits  (STOP = 10)
#define UART_NStopBits_1_5      3U      // 1.5 stop bit (STOP = 11)
/*========================*/

/*=== @UART_ParityControl ===*/
// Parity selection (maps to CR1: PCE and PS)
// The parity bit takes one position in the frame, see @UART_WordLength
#define UART_ParityControl_None     0U  // parity off  (PCE = 0)
#define UART_ParityControl_Even     1U  // even parity (PCE = 1, PS = 0)
#define UART_ParityControl_Odd      2U  // odd parity  (PCE = 1, PS = 1)
/*========================*/

/*=== @UART_BaudRate ===*/
// Baud rate in bits/s, used to compute BRR from the bus clock
// (APB2 for USART1/6, APB1 for USART2/3 and UART4/5)
#define UART_BaudRate_1200      1200U
#define UART_BaudRate_2400      2400U
#define UART_BaudRate_9600      9600U
#define UART_BaudRate_19200     19200U
#define UART_BaudRate_38400     38400U
#define UART_BaudRate_57600     57600U
#define UART_BaudRate_115200    115200U
#define UART_BaudRate_230400    230400U
#define UART_BaudRate_460800    460800U
#define UART_BaudRate_921600    921600U
#define UART_BaudRate_2M        2000000U
#define UART_BaudRate_3M        3000000U
/*========================*/

/*=== @UART_HWFlowControl ===*/
// Hardware flow control (maps to CR3: CTSE / RTSE)
// CTS/RTS pins exist only on USART1/2/3 and USART6, not on UART4/5
#define UART_HW_FC_NONE         0U      // no flow control (CTSE = 0, RTSE = 0)
#define UART_HW_FC_CTS          1U      // CTS only        (CTSE = 1)
#define UART_HW_FC_RTS          2U      // RTS only        (RTSE = 1)
#define UART_HW_FC_CTS_RTS      3U      // CTS and RTS     (CTSE = 1, RTSE = 1)
/*========================*/

/*=== @UART_OverSampling ===*/
// OverSampling 8 or 16 
#define UART_OVER_16         0U      // OverSampling by 16
#define UART_OVER_8          1U      // OverSampling by 8
/*========================*/


// User-chosen configuration for one UART/USART peripheral
typedef struct {

    uint8_t UART_Mode ;             //Options at @UART_Mode
    uint32_t UART_BaudRate ;        //Options at @UART_BaudRate
    uint8_t UART_NStopBits ;        //Options at @UART_NStopBits
    uint8_t UART_WordLength ;       //Options at @UART_WordLength
    uint8_t UART_ParityControl;     //Options at @UART_ParityControl
    uint8_t UART_HWFlowControl;     //Options at @UART_HWFlowControl 
    uint8_t  UART_OverSampling;     //Options at @UART_OverSampling

}UART_Config_t;

// Handle passed to the driver: which peripheral + its configuration
typedef struct {

    UART_RegDef_t *pUARTx ;  //Holds the Base Address of UARTx(1:6) Peripheral
    UART_Config_t UARTConfig ; //Holds the UARTx(1:6) Configs

}UART_Handle_t;

/*--- Init / DeInit ---*/
void UART_CLK_Control(UART_RegDef_t *pUARTx , uint8_t ENorDI) ;   // enable/disable the peripheral clock in RCC
void UART_Init(UART_Handle_t *pUARTHandle) ;                      // clock + frame format + baud + mode, then UE
void UART_DeInit(UART_RegDef_t *pUARTx) ;                         // pulse the RCC reset bit (registers back to default)

/*--- Data transmission and reception ---*/
// Blocking (polling) versions
void UART_SendData(UART_Handle_t *pUARTHandle,uint8_t *pTxBuffer, uint32_t Len);
void UART_ReceiveData(UART_RegDef_t *pUSARTx, uint8_t *pRxBuffer, uint32_t Len);
// Interrupt-driven (non-blocking) versions, return the state of the transfer
uint8_t UART_SendDataIT(UART_Handle_t *pUSARTHandle,uint8_t *pTxBuffer, uint32_t Len);
uint8_t UART_ReceiveDataIT(UART_Handle_t *pUSARTHandle, uint8_t *pRxBuffer, uint32_t Len); 

void UART_BR_Control(UART_RegDef_t *pUARTx , uint32_t BaudRate) ;

/*--- Peripheral control and flags ---*/
void UART_PeripheralControl(UART_RegDef_t *pUARTx , uint8_t ENorDI) ;   // set/clear UE (CR1)

void UART_Clear_Flag(UART_RegDef_t *pUARTx , uint8_t FlagName) ;        // clear a status flag in SR
uint8_t UART_GetFlag_Status(UART_RegDef_t *pUARTx , uint8_t FlagName) ; // read a status flag from SR

/*--- IRQ configuration and handling ---*/
void UART_IRQ_Config(uint8_t IRQNumber , uint8_t ENorDI) ;                  // enable/disable the IRQ in the NVIC
void UART_IRQ_Priority_Config(uint8_t IRQNumber , uint32_t IRQPriority) ;   // set the IRQ priority in the NVIC
void USART_IRQHandling(UART_Handle_t *pHandle);                             // called from the ISR to handle the event

#endif /* STM32F407XX_UART_H_ */