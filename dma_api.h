#ifndef MBED_DMA_API_H
#define MBED_DMA_API_H



#include "device.h"
#include "platform.h"
#include <stdbool.h>



#ifdef __cplusplus
extern "C" {
#endif

//Declare number of DMA channels available. Defined in the dma_api.c
extern int _channel_num ;

// Decide whether use burst mode
extern bool BURST_ENABLED;
// Decide whether pack bytes/half word to word
extern bool PACKED; 

	
typedef enum {DISABLE = 0, ENABLE = !DISABLE} FunctionalState;


//DMA transfer type
typedef enum {
    M2M = 0x00,
    M2P = 0x01,
    P2M = 0x02,
    P2P = 0x03
} TransferType;

//DMA transfer data width
typedef enum {
    _byte,
    _halfword,
    _word,
    _long
}   TransferWidth;


// DMA Trigger type.
// To be done: trigger tye should be able to caculated automatically according to source/destination address
typedef enum {  // to be reviewed. Is it possible to declare enum in header but defined in cpp? Different platforms might have different trigger
    ALWAYS = -1,
    _SSP0_TX,
    _SSP0_RX,
    _SSP1_TX,
    _SSP1_RX,
    _ADC,
    _I2S0,
    _I2S1,
    _DAC,
    _UART0_TX,
    _UART0_RX,
    _UART1_TX,
    _UART1_RX,
    _UART2_TX,
    _UART2_RX,
    _UART3_TX,
    _UART3_RX,
    _MATCH0_0 = 24,
    _MATCH0_1,
    _MATCH1_0,
    _MATCH1_1,
    _MATCH2_0,
    _MATCH2_1,
    _MATCH3_0,
    _MATCH3_1
} TriggerType;


//define DMA interrupt type: ERR, generated when err happens; FINISH, generated when transfer finish
typedef enum {ERR, FINISH} DMA_IT;

// declear DMA IRQ. The real IRQ number will be defined in per platform file dma_api.c
extern IRQn_Type _DMA_IRQ;


// Forward declaration 
// DMA Init structures definition. It is used to describe DMA configurable features.

typedef struct DMA_InitTypeDef DMA_InitTypeDef;


void DMA_Destination (DMA_InitTypeDef* DMA_InitStruct, uint32_t dst, int width, bool inc);
void DMA_Source (DMA_InitTypeDef* DMA_InitStruct, uint32_t src, int width, bool inc);
void DMA_TriggerSource(DMA_InitTypeDef* DMA_InitStruct, TriggerType trig);
void DMA_TriggerDestination(DMA_InitTypeDef* DMA_InitStruct, TriggerType trig);
void DMA_Start (int channel, DMA_InitTypeDef* DMA_InitStruct, int lengh);


/**
  * @brief  Check whether channel is active or not
  * @param  channel. The channel number.
  * @retval 0 or 1
  */
bool DMA_ChannelActive (int channel);


/**
  * @brief  Initializes the DMAy Channelx according to the specified parameters
  *         in the DMA_InitStruct.
  * @param  channel. The chosen channel number.
  * @param  DMA_InitStruct: pointer to a DMA_InitTypeDef structure that contains
  *         the configuration information for the specified DMA Channel.
  * @retval None
  */
void DMA_Init(int channel, DMA_InitTypeDef* DMA_InitStruct);



/**
  * @brief  Reset the DMA channel to default reset value
  * @param  channel. The chosen channel number
  * @param  DMA_InitStruct: pointer to a DMA_InitTypeDef structure that contains
  *         the configuration information for the specified DMA Channel.
  * @retval None
  */
void DMA_Reset(int channel, DMA_InitTypeDef* DMA_InitStruct);



/**
  * @brief  Allocate a struct pointer point to DMA_InitTypeDef type memory space. Fills each DMA_InitStruct member with its default value.
  * @param  None
	* @retval a pointer point to a memory holding DMA_InitTypeDef structure.
  */
DMA_InitTypeDef*  DMA_StructInit();

/**
  * @brief  Check whether the address is with memory ranage.
  * @param  src: the physical address
  * @retval 0 or 1
  */
__inline bool isMemory (uint32_t addr);


typedef void (*func_ptr)();
void DMA_IRQ_handler(void);
void DMA_IRQ_attach (int channel, int status, func_ptr ptr);

#ifdef __cplusplus
}
#endif

#endif
