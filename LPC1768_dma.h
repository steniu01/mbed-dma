#ifndef LPC1768_DMA_H
#define LPC1768_DMA_H

/*
#ifdef __cplusplus
 extern "C" {
#endif
*/

#include "mbed.h"

                   

/*declare number of DMA channels , defined in the cpp file*/
extern int _channel_num ;


typedef enum {DISABLE = 0, ENABLE = !DISABLE} FunctionalState;


/*DMA transfer type*/
typedef enum
{
    M2M = 0x00,
    M2P = 0x01,  
    P2M = 0x02,
    P2P = 0x03
} TransferType;

/** 
  * @brief  DMA Trigger type.
	* @note   Declare the trigger type in header file but implment in the 
  */
typedef enum {
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

/** 
  * @brief  DMA 
	* @note   Declare the trigger type in header file but implment in the 
  */
enum DMA_PeripheralType;
/** 
  * @brief  DMA Init structures definition. It used to describe DMA configurable features.
		@note  These are supposed to be used across platforms. Therefore try to define full features
					 of DMA. The implementor could choose to only implement features suiable for the platform. 
  */

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
  
    int DMA_Trigger;
    TransferType  DMA_TransferType;       
} DMA_InitTypeDef;




void DMA_destination (DMA_InitTypeDef* DMA_InitStruct, uint32_t dst, int width, bool inc);
void DMA_source (DMA_InitTypeDef* DMA_InitStruct, uint32_t src, int width, bool inc);
bool DMA_ChannelActive (int channel); 
void DMA_trigger(DMA_InitTypeDef* DMA_InitStruct, TriggerType trig);
void DMA_TransferSize (DMA_InitTypeDef* DMA_InitStruct, uint32_t len);  


void DMA_init(int channel, DMA_InitTypeDef* DMA_InitStruct);
void DMA_StructInit(DMA_InitTypeDef* DMA_InitStruct);

DMA_PeripheralType getPeripheralType (uint32_t src);
bool isMemory (uint32_t src);


void DMA_Cmd(int channel, FunctionalState NewState);


void DMA_IRQ_handler0(FunctionPointer _Success, FunctionPointer _Err);
void DMA_IRQ_handler1(FunctionPointer _Success, FunctionPointer _Err);
void DMA_ITConfig (int channel, FunctionalState NewState);




/*
#ifdef __cplusplus
}
#endif
*/
#endif
