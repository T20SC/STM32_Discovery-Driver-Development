/*
 * stm32f407xx.h
 *
 *  Created on: Sep 22, 2026
 *      Author: tmt
 */

#ifndef STM32F407XX_H_
#define STM32F407XX_H_

#include <stdint.h>

/*==========================================================*/
/*=                                                        =*/
/*=        ARM Cortex M4 Processor Specific Details        =*/
/*=                                                        =*/
/*==========================================================*/

#define NVIC_ISER0  ((volatile uint32_t *)0xE000E100) /*Interrupt Set-Enable Register 0*/
#define NVIC_ISER1  ((volatile uint32_t *)0xE000E104) /*Interrupt Set-Enable Register 1*/
#define NVIC_ISER2  ((volatile uint32_t *)0xE000E108) /*Interrupt Set-Enable Register 2*/

#define NVIC_ICER0  ((volatile uint32_t *)0xE000E180) /*Interrupt Clear-Enable Register 0*/
#define NVIC_ICER1  ((volatile uint32_t *)0xE000E184) /*Interrupt Clear-Enable Register 1*/
#define NVIC_ICER2  ((volatile uint32_t *)0xE000E188) /*Interrupt Clear-Enable Register 2*/

#define NVIC_IPR_BASE_ADDR ((volatile uint32_t *)0xE000E400) /*Interrupt Priority Register Base Address*/

#define NO_OF_PR_BITS 4
/*==========================================================*/ 

/* ===Genric Macros=== */
#define ENABLE 1
#define DISABLE 0
#define HIGH 1 
#define LOW 0 
#define SET 1 
#define RESET 0

/*=== Bus Base Addresses ===*/ 
#define APB1_BASE_ADDR (0x40000000U) /* Base address of APB1 bus */
#define APB2_BASE_ADDR (0x40010000U) /* Base address of APB2 bus */
#define AHB1_BASE_ADDR (0x40020000U) /* Base address of AHB1 bus */ 

/* ==== GPIO Related Macros and Structure Layout ==== */

#define GPIOA_BASE   (0x40020000U)
#define GPIOB_BASE   (0x40020400U)
#define GPIOC_BASE   (0x40020800U)
#define GPIOD_BASE   (0x40020C00U)
#define GPIOE_BASE   (0x40021000U)
#define GPIOF_BASE   (0x40021400U)
#define GPIOG_BASE   (0x40021800U)
#define GPIOH_BASE   (0x40021C00U)
#define GPIOI_BASE   (0x40022000U) 

/* GPIO register layout */

typedef struct {

volatile uint32_t MODER ;               //OFFSET : 0x00
volatile uint32_t OTYPER ;              //OFFSET : 0x04
volatile uint32_t OSPEEDR ;             //OFFSET : 0x08
volatile uint32_t PUPDR ;               //OFFSET : 0x0C
volatile uint32_t IDR ;                 //OFFSET : 0x10
volatile uint32_t ODR ;                 //OFFSET : 0x14
volatile uint32_t BSRR ;                //OFFSET : 0x18
volatile uint32_t LCKR ;                //OFFSET : 0x1C
volatile uint32_t AFRL ;                //OFFSET : 0x20
volatile uint32_t AFRH ;                //OFFSET : 0x24

}GPIO_RegDef_t; /*The type describing the layout of a GPIO peripheral.*/ 

/* GPIO_RegDef_t * : pointer to a GPIO_RegDef_t */

#define GPIOA   ((GPIO_RegDef_t *)GPIOA_BASE)
#define GPIOB   ((GPIO_RegDef_t *)GPIOB_BASE)
#define GPIOC   ((GPIO_RegDef_t *)GPIOC_BASE)
#define GPIOD   ((GPIO_RegDef_t *)GPIOD_BASE)
#define GPIOE   ((GPIO_RegDef_t *)GPIOE_BASE)
#define GPIOF   ((GPIO_RegDef_t *)GPIOF_BASE)
#define GPIOG   ((GPIO_RegDef_t *)GPIOG_BASE)
#define GPIOH   ((GPIO_RegDef_t *)GPIOH_BASE)
#define GPIOI   ((GPIO_RegDef_t *)GPIOI_BASE)

/*==========================================================*/ 

#define RCC_BASE (0x40023800U)  


/* RCC_AHB1ENR bit positions */
#define RCC_GPIOAEN   0U
#define RCC_GPIOBEN   1U
#define RCC_GPIOCEN   2U
#define RCC_GPIODEN   3U
#define RCC_GPIOEEN   4U
#define RCC_GPIOFEN   5U
#define RCC_GPIOGEN   6U
#define RCC_GPIOHEN   7U
#define RCC_GPIOIEN   8U


/* RCC_SPI bit positions : APB1 , APB2 */ 
#define RCC_SPI1EN      12U         //Hangs on APB2 Bus
#define RCC_SPI2EN      14U         //Hangs on APB1 Bus
#define RCC_SPI3EN      15U         //Hangs on APB1 Bus

/*--- Clock Control / Deinit Bits (RCC) ---*/
#define RCC_USART1EN      4U      // RCC_APB2ENR
#define RCC_USART2EN      17U     // RCC_APB1ENR
#define RCC_USART3EN      18U     // RCC_APB1ENR
#define RCC_UART4EN       19U     // RCC_APB1ENR
#define RCC_UART5EN       20U     // RCC_APB1ENR
#define RCC_USART6EN      5U      // RCC_APB2ENR


/*--- Clock Prescalars and Clock Selection*/  
#define RCC_CR_HSION            0U
#define RCC_CR_HSIRDY           1U
#define RCC_CR_HSITRIM_L        3U
#define RCC_CR_HSITRIM_H        7U
#define RCC_CR_HSICAL_L         8U 
#define RCC_CR_HSICAL_H         15U
#define RCC_CR_HSEON            16U 
#define RCC_CR_HSERDY           17U 
#define RCC_CR_HSEBYP           18U 
#define RCC_CR_CSSON            19U
#define RCC_CR_PLLON            24U
#define RCC_CR_PLLRDY           25U
#define RCC_CR_PLLI2SON         26U
#define RCC_CR_PLLI2SRDY        27U



#define RCC_CFGR_SW_L            0U 
#define RCC_CFGR_SW_H            1U 
#define RCC_CFGR_SWS_L           2U
#define RCC_CFGR_SWS_H           3U  
#define RCC_CFGR_HPRE_L          4U 
#define RCC_CFGR_HPRE_H          7U
#define RCC_CFGR_PPRE1_L         10U
#define RCC_CFGR_PPRE1_H         12U  
#define RCC_CFGR_PPRE2_L         13U  
#define RCC_CFGR_PPRE2_H         15U  
#define RCC_CFGR_RTCPRE_L        16U 
#define RCC_CFGR_RTCPRE_H        20U 
#define RCC_CFGR_MCO1_L          21U 
#define RCC_CFGR_MCO1_H          22U  
#define RCC_CFGR_I2SSCR          23U
#define RCC_CFGR_MCO1PRE_L       24U  
#define RCC_CFGR_MCO1PRE_H       26U
#define RCC_CFGR_MCO2PRE_L       27U   
#define RCC_CFGR_MCO2PRE_H       29U
#define RCC_CFGR_MCO2_L          30U 
#define RCC_CFGR_MCO2_H          31U 







//RCC Layout
typedef struct {

volatile uint32_t CR ;                 //OFFSET : 0x00
volatile uint32_t PLLCFGR ;            //OFFSET : 0x04
volatile uint32_t CFGR ;               //OFFSET : 0x08
volatile uint32_t CIR ;                //OFFSET : 0x0C
volatile uint32_t AHB1RSTR ;           //OFFSET : 0x10
volatile uint32_t AHB2RSTR ;           //OFFSET : 0x14
volatile uint32_t AHB3RSTR ;           //OFFSET : 0x18
volatile uint32_t RESERVED0 ;          //OFFSET : 0x1C
volatile uint32_t APB1RSTR ;           //OFFSET : 0x20
volatile uint32_t APB2RSTR ;           //OFFSET : 0x24
volatile uint32_t RESERVED1[2] ;       //OFFSET : 0x28 - 0x2C
volatile uint32_t AHB1ENR ;            //OFFSET : 0x30
volatile uint32_t AHB2ENR ;            //OFFSET : 0x34
volatile uint32_t AHB3ENR ;            //OFFSET : 0x38
volatile uint32_t RESERVED2 ;          //OFFSET : 0x3C
volatile uint32_t APB1ENR ;            //OFFSET : 0x40
volatile uint32_t APB2ENR ;            //OFFSET : 0x44
volatile uint32_t RESERVED3[2] ;       //OFFSET : 0x48 - 0x4C
volatile uint32_t AHB1LPENR ;          //OFFSET : 0x50
volatile uint32_t AHB2LPENR ;          //OFFSET : 0x54
volatile uint32_t AHB3LPENR ;          //OFFSET : 0x58
volatile uint32_t RESERVED4 ;           //OFFSET : 0x5C
volatile uint32_t APB1LPENR ;           //OFFSET : 0x60
volatile uint32_t APB2LPENR ;           //OFFSET : 0x64
volatile uint32_t RESERVED5[2] ;        //OFFSET : 0x68 - 0x6C
volatile uint32_t BDCR ;                //OFFSET : 0x70
volatile uint32_t CSR ;                 //OFFSET : 0x74
volatile uint32_t RESERVED6[2] ;        //OFFSET : 0x78 - 0x7C
volatile uint32_t SSCGR ;               //OFFSET : 0x80
volatile uint32_t PLLI2SCFGR ;          //OFFSET : 0x84

}RCC_RegDef_t; /*The type describing the layout of a RCC peripheral.*/

#define RCC ((RCC_RegDef_t *)RCC_BASE) /*The base address of the RCC peripheral.*/ 

/*==========================================================*/ 

/* ==== EXTI Related Macros and Structure Layout ==== */  

#define EXTI_BASE_ADDR ((APB2_BASE_ADDR) + 0x3C00U) /* Base address of EXTI peripheral */ 

typedef struct {

volatile uint32_t IMR ;             //OFFSET : 0x00
volatile uint32_t EMR ;             //OFFSET : 0x04
volatile uint32_t RTSR ;            //OFFSET : 0x08
volatile uint32_t FTSR ;            //OFFSET : 0x0C
volatile uint32_t SWIER ;           //OFFSET : 0x10
volatile uint32_t PR ;              //OFFSET : 0x14

}EXTI_RegDef_t; /*The type describing the layout of an EXTI peripheral.*/

#define EXTI ((EXTI_RegDef_t *)EXTI_BASE_ADDR) /*The base address of the EXTI peripheral.*/ 

/*====================*/
#define IRQ_NO_EXTI0        6
#define IRQ_NO_EXTI1        7
#define IRQ_NO_EXTI2        8
#define IRQ_NO_EXTI3        9
#define IRQ_NO_EXTI4        10
#define IRQ_NO_EXTI9_5      23
#define IRQ_NO_EXTI15_10    40   
/*====================*/

/*==========================================================*/  

/* ==== SYSCFG Related Macros and Structure Layout ==== */ 
#define SYSCFG_BASE_ADDR ((APB2_BASE_ADDR) + 0x3800U) /* Base address of SYSCFG peripheral */

typedef struct {

volatile uint32_t MEMRMP ;          //OFFSET : 0x00
volatile uint32_t PMC ;             //OFFSET : 0x04
volatile uint32_t EXTICR[4] ;      //OFFSET : 0x08 - 0x14
volatile uint32_t RESERVED[2] ;     //OFFSET : 0x18-0x1C
volatile uint32_t CMPCR ;            //OFFSET : 0x20

}SYSCFG_RegDef_t; /*The type describing the layout of a SYSCFG peripheral.*/

#define SYSCFG ((SYSCFG_RegDef_t *)SYSCFG_BASE_ADDR) /* Pointer to the SYSCFG_RegDef_t Struct */

/*==========================================================*/ 

/* ==== SPI Related Macros and Structure Layout ==== */

#define SPI1_BASE_ADDR ((APB2_BASE_ADDR) + 0x3000U)
#define SPI2_BASE_ADDR ((APB1_BASE_ADDR) + 0x3800U) 
#define SPI3_BASE_ADDR ((APB1_BASE_ADDR) + 0x3C00U)   

/* SPI_CR1 bit positions */
#define SPI_CR1_CPHA        0U
#define SPI_CR1_CPOL        1U
#define SPI_CR1_MSTR        2U
#define SPI_CR1_BR          3U     /* BR[2:0] field, bits 3-5 */
#define SPI_CR1_SPE         6U
#define SPI_CR1_LSBFIRST    7U
#define SPI_CR1_SSI         8U
#define SPI_CR1_SSM         9U
#define SPI_CR1_RXONLY      10U
#define SPI_CR1_DFF         11U
#define SPI_CR1_CRCNEXT     12U
#define SPI_CR1_CRCEN       13U
#define SPI_CR1_BIDIOE      14U
#define SPI_CR1_BIDIMODE    15U

/* SPI_CR2 bit positions */
#define SPI_CR2_RXDMAEN     0U
#define SPI_CR2_TXDMAEN     1U
#define SPI_CR2_SSOE        2U
#define SPI_CR2_FRF         4U     /* bit 3 is reserved */
#define SPI_CR2_ERRIE       5U
#define SPI_CR2_RXNEIE      6U
#define SPI_CR2_TXEIE       7U

/* SPI_SR bit positions */
#define SPI_SR_RXNE         0U
#define SPI_SR_TXE          1U
#define SPI_SR_CHSIDE       2U     /* I2S mode only */
#define SPI_SR_UDR          3U     /* I2S mode only */
#define SPI_SR_CRCERR       4U
#define SPI_SR_MODF         5U
#define SPI_SR_OVR          6U
#define SPI_SR_BSY          7U
#define SPI_SR_FRE          8U

// SPI MASKS // 
#define SPI_CR1_BR_MSK    (7U << SPI_CR1_BR)

//SPI Register Layout
typedef struct { 

    volatile uint32_t CR1 ;     //OFFSET : 0x00
    volatile uint32_t CR2 ;     //OFFSET : 0x04
    volatile uint32_t SR ;      //OFFSET : 0x08
    volatile uint32_t DR ;      //OFFSET : 0x0C
    volatile uint32_t CRCPR ;   //OFFSET : 0x10
    volatile uint32_t RXCRCR ;  //OFFSET : 0x14
    volatile uint32_t TXCRCR ;  //OFFSET : 0x18
    volatile uint32_t I2SCFGR ; //OFFSET : 0x1C
    volatile uint32_t I2SPR ;   //OFFSET : 0x20

}SPI_RegDef_t ;   

#define SPI1 ((SPI_RegDef_t *)SPI1_BASE_ADDR)   /*Pointer to the SPI1 peripheral.*/
#define SPI2 ((SPI_RegDef_t *)SPI2_BASE_ADDR)   /*Pointer to the SPI2 peripheral.*/ 
#define SPI3 ((SPI_RegDef_t *)SPI3_BASE_ADDR)   /*Pointer to the SPI3 peripheral.*/

/*==========================================================*/ 

/* ==== UART/USART Related Macros and Structure Layout ==== */  
  

/*--- UART SR Register Bits (status) ---*/
#define UART_SR_PE              0U      // parity error
#define UART_SR_FE              1U      // framing error
#define UART_SR_NF              2U      // noise flag
#define UART_SR_ORE             3U      // overrun error
#define UART_SR_IDLE            4U      // idle line detected
#define UART_SR_RXNE            5U      // RX data register not empty
#define UART_SR_TC              6U      // transmission complete
#define UART_SR_TXE             7U      // TX data register empty
#define UART_SR_LBD             8U      // LIN break detected
#define UART_SR_CTS             9U      // CTS flag (not on UART4/5) 

/*--- UART BRR Register Bits ---*/
#define UART_BRR_DIV_FRACTION       0U      // [3:0] fraction (4-bit field)
#define UART_BRR_DIV_MANTISSA       4U      // [15:4] mantissa (12-bit field)

/*--- UART DR Register Bits ---*/
#define UART_DR_DATA            0U      // [8:0] data value (9-bit field)

/*--- UART CR1 Register Bits ---*/
#define UART_CR1_SBK            0U      // send break
#define UART_CR1_RWU            1U      // receiver wakeup
#define UART_CR1_RE             2U      // receiver enable
#define UART_CR1_TE             3U      // transmitter enable
#define UART_CR1_IDLEIE         4U      // IDLE interrupt enable
#define UART_CR1_RXNEIE         5U      // RXNE interrupt enable
#define UART_CR1_TCIE           6U      // TC interrupt enable
#define UART_CR1_TXEIE          7U      // TXE interrupt enable
#define UART_CR1_PEIE           8U      // PE interrupt enable
#define UART_CR1_PS             9U      // parity selection (0 even, 1 odd)
#define UART_CR1_PCE            10U     // parity control enable
#define UART_CR1_WAKE           11U     // wakeup method
#define UART_CR1_M              12U     // word length (0 = 8 bits, 1 = 9 bits, parity included)
#define UART_CR1_UE             13U     // USART enable
#define UART_CR1_OVER8          15U     // oversampling mode (0 = x16, 1 = x8)

/*--- UART CR2 Register Bits ---*/
#define UART_CR2_ADD            0U      // [3:0] node address (4-bit field)
#define UART_CR2_LBDL           5U      // LIN break detection length
#define UART_CR2_LBDIE          6U      // LIN break detection interrupt enable
#define UART_CR2_LBCL           8U      // last bit clock pulse (sync mode, not on UART4/5)
#define UART_CR2_CPHA           9U      // clock phase (sync mode, not on UART4/5)
#define UART_CR2_CPOL           10U     // clock polarity (sync mode, not on UART4/5)
#define UART_CR2_CLKEN          11U     // clock pin enable (sync mode, not on UART4/5)
#define UART_CR2_STOP           12U     // [13:12] stop bits (2-bit field)
#define UART_CR2_LINEN          14U     // LIN mode enable

/*--- UART CR3 Register Bits ---*/
#define UART_CR3_EIE            0U      // error interrupt enable (DMA/multibuffer)
#define UART_CR3_IREN           1U      // IrDA mode enable
#define UART_CR3_IRLP           2U      // IrDA low-power
#define UART_CR3_HDSEL          3U      // half-duplex selection
#define UART_CR3_NACK           4U      // smartcard NACK (not on UART4/5)
#define UART_CR3_SCEN           5U      // smartcard mode (not on UART4/5)
#define UART_CR3_DMAR           6U      // DMA enable receiver
#define UART_CR3_DMAT           7U      // DMA enable transmitter
#define UART_CR3_RTSE           8U      // RTS enable (not on UART4/5)
#define UART_CR3_CTSE           9U      // CTS enable (not on UART4/5)
#define UART_CR3_CTSIE          10U     // CTS interrupt enable (not on UART4/5)
#define UART_CR3_ONEBIT         11U     // one sample bit method

/*--- UART GTPR Register Bits (smartcard/IrDA, not on UART4/5) ---*/
#define UART_GTPR_PSC           0U      // [7:0] prescaler (8-bit field)
#define UART_GTPR_GT            8U      // [15:8] guard time (8-bit field)

/*--- UART CR3 Register Bits ---*/
#define UART_BRR_FRAC_L     0U 
#define UART_BRR_FRAC_H     3U 

#define UART_BRR_MANT_L     4U 
#define UART_BRR_MANT_H     15U 
/*=== UART Bits ===*/ 

//UART Base Addresses
#define USART1_BASE_ADDR ((APB2_BASE_ADDR) + 0x1000U)
#define USART2_BASE_ADDR ((APB1_BASE_ADDR) + 0x4400U) 
#define USART3_BASE_ADDR ((APB1_BASE_ADDR) + 0x4800U)  
#define UART4_BASE_ADDR  ((APB1_BASE_ADDR) + 0x4C00U)  
#define UART5_BASE_ADDR  ((APB1_BASE_ADDR) + 0x5000U)  
#define USART6_BASE_ADDR ((APB2_BASE_ADDR) + 0x1400U) 

//UART/USART Register Layout 
typedef struct { 
    
    volatile uint32_t SR ;      //OFFSET : 0x00
    volatile uint32_t DR ;      //OFFSET : 0x04
    volatile uint32_t BRR ;     //OFFSET : 0x08
    volatile uint32_t CR1 ;     //OFFSET : 0x0C
    volatile uint32_t CR2 ;     //OFFSET : 0x10
    volatile uint32_t CR3 ;     //OFFSET : 0x14
    volatile uint32_t GTPR ;    //OFFSET : 0x18
}UART_RegDef_t ; 

#define USART1 ((UART_RegDef_t *)USART1_BASE_ADDR)   /*Pointer to the USART1 peripheral.*/
#define USART2 ((UART_RegDef_t *)USART2_BASE_ADDR)   /*Pointer to the USART2 peripheral.*/ 
#define USART3 ((UART_RegDef_t *)USART3_BASE_ADDR)   /*Pointer to the USART3 peripheral.*/
#define UART4  ((UART_RegDef_t *)UART4_BASE_ADDR)    /*Pointer to the UART4 peripheral.*/
#define UART5  ((UART_RegDef_t *)UART5_BASE_ADDR)    /*Pointer to the UART5 peripheral.*/
#define USART6 ((UART_RegDef_t *)USART6_BASE_ADDR)   /*Pointer to the USART6 peripheral.*/

/*==========================================================*/ 


#endif /* STM32F407XX_H_ */
