#ifndef LPC1768_DMA_H
#define LPC1768_DMA_H

#include "mbed.h"
/** 
  * @brief  DMA Init structure definition
  */
/*DMA channel control register*/
#define DMA_CCxControl_TransferSize_Pos 0
#define DMA_CCxControl_SBSize_Pos 12
#define DMA_CCxControl_DBSize_Pos 15
#define DMA_CCxControl_SWidth_Pos 18
#define DMA_CCxControl_DWidth_Pos 21
#define DMA_CCxControl_SI_Pos 26
#define DMA_CCxControl_DI_Pos 27
#define DMA_CCxControl_I_Pos 31

/*DMA Channel config register*/
#define DMA_CCxConfig_E_Pos 0
#define DMA_CCxConfig_SrcPeripheral_Pos 1
#define DMA_CCxConfig_DestPeripheral_Pos 6
#define DMA_CCxConfig_TransferType_Pos 11
#define DMA_CCxConfig_IE_Pos 14
#define DMA_CCxConfig_ITC_Pos 15
#define DMA_CCxConfig_L_Pos 16
#define DMA_CCxConfig_A_Pos 17
#define DMA_CCxConfig_H_Pos 18

/*DMA Interrupt*/
#define DMA_IE                        ((uint32_t)0x00004000)
#define DMA_ITC                       ((uint32_t)0x00008000) 

#define channel_num 8

typedef enum
{
    M2M = 0x00,
    M2P = 0x01,
    P2M = 0x02,
    P2P = 0x03
} TransferType;


typedef struct
{
    uint32_t DestAddr;
    uint32_t SrcAddr;
    uint32_t next;
    uint32_t control;
} DMA_LLI;

typedef struct
{
    uint32_t DMA_DestAddr; /*!< Specifies the destination base address for DMAy Channelx. */
    uint32_t DMA_SrcAddr;     /*!< Specifies the source base address for DMAy Channelx. */
//    DMA_LLI LLI; /UNDO *!< Specifies the next linked item   */
    uint32_t DMA_TransferSize;/*!< Specifies the source  transfer size    */
    uint32_t DMA_SrcBurst; /*!< Specifies the source  burst size    */
    uint32_t DMA_DestBurst; /*!< Specifies the destination burst size   */
    uint32_t DMA_SrcWidth; /*!< Specifies the source transfer width   */
    uint32_t DMA_DestWidth; /*!< Specifies the destination transfer width   */
    bool DMA_SrcInc;  /*!< Specifies whether the source is incremented or not */
    bool DMA_DestInc; /*!< Specifies whether the destination is incremented or not */
    bool DMA_TermInt; /*!< Specifies whether the terminal count interrupt enabled or not */
    
    /*!< Specifies the features set by channel config register */
    uint32_t  DMA_SrcPeripheral;
    uint32_t  DMA_DestPeripheral;
    TransferType  DMA_TransferType;       
} DMA_InitTypeDef;



typedef enum {DISABLE = 0, ENABLE = !DISABLE} FunctionalState;
typedef enum {COUNTER, ERR} DMA_IT;


void DMA_init(LPC_GPDMACH_TypeDef* DMAy_Channelx, DMA_InitTypeDef* DMA_InitStruct);
void DMA_StructInit(DMA_InitTypeDef* DMA_InitStruct);
void DMA_Cmd(LPC_GPDMACH_TypeDef*  DMAy_Channelx, FunctionalState NewState);
void DMA_ITConfig (LPC_GPDMACH_TypeDef* DMAy_Channelx, uint32_t DMA_IT, FunctionalState NewState);
void DMA_ClearITPendingBit(LPC_GPDMACH_TypeDef* DMAy_Channelx, uint32_t DMA_IT);
bool DMA_ChannelActive (int channel);

#endif
