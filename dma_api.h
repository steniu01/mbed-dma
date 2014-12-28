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

	
typedef enum {DISABLE = 0, ENABLE = !DISABLE} FunctionalState;


//DMA transfer type
typedef enum {
    M2M = 0x00,
    M2P = 0x01,
    P2M = 0x02,
    P2P = 0x03
} TransferType;



// DMA Trigger type.
// To be done: trigger type should be able to caculated automatically according to source/destination address
typedef enum {  
     ALWAYS ,
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
    _MATCH0_0,
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

typedef struct LLI
{
	 uint32_t LLI_src;
	 uint32_t LLI_dst;
	 uint32_t LLI_next;
	 uint32_t LLI_control; // Only care about transfer size, as other charactors will be same as initial transfer
}LLI;

void DMA_destination(DMA_InitTypeDef* DMA_InitStruct, const uint32_t dst, unsigned int width, bool inc);
void DMA_source(DMA_InitTypeDef* DMA_InitStruct, const uint32_t src, unsigned int width, bool inc);
void DMA_trigger_source(DMA_InitTypeDef* DMA_InitStruct, TriggerType trig);
void DMA_trigger_destination(DMA_InitTypeDef* DMA_InitStruct, TriggerType trig);

/**
  * @brief  Initializes the DMAy Channelx according to the specified parameters
  *         in the DMA_InitStruct.
  * @param  channel. The chosen channel number.
  * @param  DMA_InitStruct: pointer to a DMA_InitTypeDef structure that contains
  *         the configuration information for the specified DMA Channel.
  * @retval None
  */
void DMA_init(int channel, DMA_InitTypeDef* DMA_InitStruct);

void DMA_start(int channel, DMA_InitTypeDef* DMA_InitStruct, unsigned int lengh);
//void DMA_next(DMA_InitTypeDef* DMA_InitStruct, LLI* list);
void DMA_next(DMA_InitTypeDef* DMA_InitStruct, const uint32_t src, const uint32_t dst, uint32_t size); // The input size has to smaller than 4096bytes. 
/**
  * @brief  Check whether channel is active or not
	* @param  channel. The channel number.
  * @retval 0 or 1
  */
bool DMA_channel_active(int channel);





/**
  * @brief  Reset the DMA channel to default reset value
  * @param  channel. The chosen channel number
  * @retval None
  */
void DMA_reset(int channel);


/**
  * @brief  Allocate a struct pointer point to DMA_InitTypeDef type memory space. Fills each DMA_InitStruct member with its default value.
  * @param  None
	* @retval a pointer point to a  DMA_InitTypeDef structure.
  */
DMA_InitTypeDef*  DMA_struct_create(void);



/**
  * @brief  Delete the memory allocated for the DMA_InitTypeDef structure
  * @param  A pointer point to a DMA_InitTypeDef structure 
	* @retval None
  */
void DMA_struct_delete(DMA_InitTypeDef* ptr);


typedef void (*func_ptr)();



void DMA_IRQ_handler(void);
void DMA_IRQ_attach(int channel, int status, func_ptr ptr);
void DMA_IRQ_detach(int channel);

#ifdef __cplusplus
}
#endif

#endif
