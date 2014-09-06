#ifndef DMA_API_H
#define DMA_API_H



#include "device.h"  
#include "platform.h"
#include <stdbool.h>



#ifdef __cplusplus
 extern "C" {
#endif

//Declare number of DMA channels available. Defined in the cpp file
extern int _channel_num ;


typedef enum {DISABLE = 0, ENABLE = !DISABLE} FunctionalState;


//DMA transfer type
typedef enum
{
    M2M = 0x00,
    M2P = 0x01,  
    P2M = 0x02,
    P2P = 0x03
} TransferType;

//DMA transfer data width 
typedef enum
{
  _byte,
	_halfword,
	_word,
	_long
}	TransferWidth;


// DMA Trigger type.
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
}TriggerType;


//define DMA interrupt type: ERR, generated when err happens; FINISH, generated when transfer finish
typedef enum {ERR, FINISH} DMA_IT;


// DMA Init structures definition. It used to describe DMA configurable features. 
// These are supposed to be used across platforms. Therefore try to define full features
// of DMA. The implementor could choose to only implement features suiable for the platform. 
typedef struct
{
    /*!< Specifies the features set by channel control register */
    uint32_t DMA_DstAddr; /*!< Specifies the destination base address for DMAy Channelx. */
    uint32_t DMA_SrcAddr;     /*!< Specifies the source base address for DMAy Channelx. */
    uint32_t DMA_TransferSize;/*!< Specifies the source  transfer size    */
    uint32_t DMA_SrcBurst; /*!< Specifies the source  burst size    */
    uint32_t DMA_DestBurst; /*!< Specifies the destination burst size   */
    uint32_t DMA_SrcWidth; /*!< Specifies the source transfer width   */
    uint32_t DMA_DestWidth; /*!< Specifies the destination transfer width   */
		uint32_t DMA_LLI; /*!< Specifies the next link address   */
    bool DMA_SrcInc;  /*!< Specifies whether the source is incremented or not */
    bool DMA_DestInc; /*!< Specifies whether the destination is incremented or not */
    bool DMA_TermInt; /*!< Specifies whether the terminal count interrupt enabled or not */
    int DMA_TriggerSource; /*!< Specifies the source trigger */
	  int DMA_TriggerDestination; /*!< Specifies the destination trigger */
    TransferType  DMA_TransferType;       
} DMA_InitTypeDef;



void DMA_destination (DMA_InitTypeDef* DMA_InitStruct, uint32_t dst, int width, bool inc);
void DMA_source (DMA_InitTypeDef* DMA_InitStruct, uint32_t src, int width, bool inc);
void DMA_TriggerSource(DMA_InitTypeDef* DMA_InitStruct, TriggerType trig);
void DMA_TriggerDestination(DMA_InitTypeDef* DMA_InitStruct, TriggerType trig);
void DMA_TransferSize (DMA_InitTypeDef* DMA_InitStruct, uint32_t len);  


/** 
  * @brief  Check whether channel is active or not
  * @param  channel. The channel number.  
  * @retval 0 or 1
  */
bool DMA_ChannelActive (int channel); 


/**
  * @brief  Initializes the DMAy Channelx according to the specified parameters 
  *         in the DMA_InitStruct.
  * @param  DMAyChannelx.Channel base address. x can be 1 to 7 for DMA to select the DMA Channel.
  * @param  DMA_InitStruct: pointer to a DMA_InitTypeDef structure that contains
  *         the configuration information for the specified DMA Channel.
  * @retval None
  */
void DMA_init(int channel, DMA_InitTypeDef* DMA_InitStruct);



/**
  * @brief  Reset the DMA channel to default reset value
  * @param  DMAyChannelx.Channel base address. x can be 1 to 7 for DMA to select the DMA Channel.
  * @param  DMA_InitStruct: pointer to a DMA_InitTypeDef structure that contains
  *         the configuration information for the specified DMA Channel.
  * @retval None
  */
void DMA_deinit(int channel, DMA_InitTypeDef* DMA_InitStruct);



/**
  * @brief  Fills each DMA_InitStruct member with its default value.
  * @param  DMA_InitStruct: pointer to a DMA_InitTypeDef structure which will
  *         be initialized.
  * @retval None
  */
void DMA_StructInit(DMA_InitTypeDef* DMA_InitStruct);

/**
  * @brief  Check whether the address is with memory ranage. 
  * @param  src: the physical address
  * @retval 0 or 1
  */
bool isMemory (uint32_t addr);


/**
  * @brief  Enable or disable the specified channel
  * @param  channel: the chosen channel number
	* @param  NewState: ENABLE or DISABLE
  * @retval None
  */
void DMA_Cmd(int channel, FunctionalState NewState);

/**
  * @brief  IRQ handler
  * @retval None
  * @Note   Define two interrupt handlers here because some devices like STM32f30x might have mutiple GPDMA or mutiple interrupt
  *					handlers. For LPC1768, only implement one IRQ_handler 
	*/
void DMA_IRQ_handler0(void);
void DMA_IRQ_handler1(void);

/**
  * @brief  	Mask or unmask the interrupt on chosen channel
	* @param 		channel: then chosen channel number
	* @NewState	ENABLE or DISABLE
	* @retval 	None
  */
void DMA_ITConfig (int channel, FunctionalState NewState);

/**
  * @brief  Check whether ERR or FINISH interrupt on a channel has happend or not
	* @param: channel: then chosen channel number
	* @param: interrupt: ERR interrupt or FINISH interrupt
	* @retval None
	*/
bool DMA_ITStatus (int channel, DMA_IT interrupt);

#ifdef __cplusplus
}
#endif

#endif
