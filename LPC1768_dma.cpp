#include "LPC1768_dma.h"

int _channel_num = 8;

/*DMA error interrupt register base address*/
#define DMA_IE  ((uint32_t)0x00004000)
/*DMA terminal counter interrupt register base address*/
#define DMA_ITC ((uint32_t)0x00008000) 

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



enum DMA_PeripheralType{
	 non_memory = 0,
	 SSP0,
	 SSP1,
	 ADC,
	 I2S,
	 DAC,
	 UART0,
	 UART1,
	 UART2,
	 UART3
};

static TransferType getTransferType (uint32_t src_addr, uint32_t dst_addr );


/**
  * @brief  Get the channel address according to the channel number
  * @param  channel. The chosen channel number
  * @retval Chosen channel address
  */
static LPC_GPDMACH_TypeDef* returnChannel(int channel){
    switch (channel){
        case 0: return LPC_GPDMACH0; 
        case 1: return LPC_GPDMACH1;
        case 2: return LPC_GPDMACH2; 
        case 3: return LPC_GPDMACH3; 
        case 4: return LPC_GPDMACH4; 
        case 5:	return LPC_GPDMACH5; 
        case 6: return LPC_GPDMACH6; 
        case 7: return LPC_GPDMACH7; 
        default: return LPC_GPDMACH0; 
    }
}

/**
  * @brief  Initializes the DMAy Channelx according to the specified parameters 
  *         in the DMA_InitStruct.
  * @param  DMAyChannelx.Channel base address. x can be 1 to 7 for DMA to select the DMA Channel.
  * @param  DMA_InitStruct: pointer to a DMA_InitTypeDef structure that contains
  *         the configuration information for the specified DMA Channel.
  * @retval None
  */
void DMA_init(int channel, DMA_InitTypeDef* DMA_InitStruct)
{
		LPC_GPDMACH_TypeDef* GPDMA_channel;	
	  /* get DMA channel address*/
		GPDMA_channel =	returnChannel(channel);
    /* Calcuate transfer type according to source address and destination address*/	
		DMA_InitStruct->DMA_TransferType = getTransferType(DMA_InitStruct->DMA_SrcAddr, DMA_InitStruct->DMA_DstAddr);
		/* Reset interrupt pending bits for DMA Channel */
		LPC_GPDMA->DMACIntTCClear = 1<<channel;
    LPC_GPDMA->DMACIntErrClr = 1<<channel;
 
/*--------------------------- DMAy channelx request select register ----------------*/
		/* Choose UART or Timer DMA request for DMA inputs 8 through 15*/
    if ( DMA_InitStruct->DMA_Trigger & 0x10) 
        LPC_SC->DMAREQSEL |= 1 << (DMA_InitStruct->DMA_Trigger - 24); // I don't know why. The user manual says DMAREQSET is GPDMA register, but in mbed, it is LPC_SC register
    else
        LPC_SC->DMAREQSEL &= ~(1 << (DMA_InitStruct->DMA_Trigger - 24));
	
 
/*--------------------------- DMAy channelx source and destination ----------------*/
    /* Write to DMA channel source address */
    GPDMA_channel->DMACCSrcAddr = DMA_InitStruct->DMA_SrcAddr;
    /* Write to DMA channel destination address */
    GPDMA_channel->DMACCDestAddr = DMA_InitStruct->DMA_DstAddr;
		
/*--------------------------- DMAy channelx control register ----------------*/
    /* Set TransferSize */
    /* Set source burst size */
    /* Set destination burst size */
    /* Set source width size */  
    /* Set destination width size */
    /* Set source increment */
    /* Set destination increment */
    /* Set destination increment */
    /* Enable terminal count interrupt */
    GPDMA_channel->DMACCControl |= (DMA_InitStruct->DMA_TransferSize<<DMA_CCxControl_TransferSize_Pos) |
                                  (DMA_InitStruct->DMA_SrcBurst<<DMA_CCxControl_SBSize_Pos)  |
                                  (DMA_InitStruct->DMA_DestBurst<<DMA_CCxControl_DBSize_Pos) |
                                  (DMA_InitStruct->DMA_SrcWidth<<DMA_CCxControl_SWidth_Pos)  |
                                  (DMA_InitStruct->DMA_DestWidth<<DMA_CCxControl_DWidth_Pos) |
                                  (DMA_InitStruct->DMA_SrcInc<<DMA_CCxControl_SI_Pos)    |
                                  (DMA_InitStruct->DMA_DestInc<<DMA_CCxControl_DI_Pos)   |
                                  (DMA_InitStruct->DMA_TermInt<<DMA_CCxControl_I_Pos ); 
																	
																	
/*--------------------------- DMAy channelx configuration register ----------------*/
    /* Set source periheral trigger. If the source is memory, this bit is ignored. Set according low 4 bit of DMA_Trigger  */
    /* Set destination periheral trigger. If the destination is memory, this bit is ignored. Set according low 4 bit of DMA_Trigger */
    /* Set transfer type: M2M, M2P, P2M, M2M */
    GPDMA_channel->DMACCConfig |= ((DMA_InitStruct->DMA_Trigger & 0xf) <<DMA_CCxConfig_SrcPeripheral_Pos)|    //set SrcPeripheral according low 4 bit of DMA_Trigger 
                                  ((DMA_InitStruct->DMA_Trigger & 0xf)<<DMA_CCxConfig_DestPeripheral_Pos)|		//set DstPeripheral according low 4 bit of DMA_Trigger 	
                                  (DMA_InitStruct->DMA_TransferType<<DMA_CCxConfig_TransferType_Pos);  


}//end of DMA_init

/**
  * @brief  Fills each DMA_InitStruct member with its default value.
  * @param  DMA_InitStruct: pointer to a DMA_InitTypeDef structure which will
  *         be initialized.
  * @retval None
  */
void DMA_StructInit(DMA_InitTypeDef* DMA_InitStruct)
{
/*-------------- Reset DMA init structure parameters values ------------------*/  
    DMA_InitStruct->DMA_DstAddr = 0; 
    DMA_InitStruct->DMA_SrcAddr = 0;    
    DMA_InitStruct->DMA_TransferSize = 0;
    DMA_InitStruct->DMA_SrcBurst = 0; 
    DMA_InitStruct->DMA_DestBurst = 0;
    DMA_InitStruct->DMA_SrcWidth = 0;
    DMA_InitStruct->DMA_DestWidth = 0;
		DMA_InitStruct->DMA_LLI = 0;
    DMA_InitStruct->DMA_SrcInc = 0; 
    DMA_InitStruct->DMA_DestInc = 0; 
    DMA_InitStruct->DMA_TermInt = 0;     
    DMA_InitStruct->DMA_Trigger = ALWAYS;
    DMA_InitStruct->DMA_TransferType = M2M;       
} //end of DMA_StructInit


void DMA_destination (DMA_InitTypeDef* DMA_InitStruct, uint32_t dst, int width, bool inc){
    DMA_InitStruct->DMA_DstAddr = dst;
		DMA_InitStruct->DMA_SrcBurst = 0x00; // 1 byte To be done
    DMA_InitStruct->DMA_DestWidth = width;
    DMA_InitStruct->DMA_DestInc = inc;
}

void DMA_source (DMA_InitTypeDef* DMA_InitStruct, uint32_t src, int width, bool inc){
    DMA_InitStruct->DMA_SrcAddr = src;
    DMA_InitStruct->DMA_SrcBurst = 0x00; // 1 byte
    DMA_InitStruct->DMA_SrcWidth = width;
    DMA_InitStruct->DMA_SrcInc = inc;   
}

void DMA_TransferSize ( DMA_InitTypeDef* DMA_InitStruct, uint32_t len){
    DMA_InitStruct->DMA_TransferSize = len;   
}

void DMA_trigger(DMA_InitTypeDef* DMA_InitStruct, TriggerType trig){
		DMA_InitStruct->DMA_Trigger = trig;
}


void IRQ_handler0(FunctionPointer _Err, FunctionPointer _Success){
        uint32_t TCStatus=0;
        uint32_t ErrStatus=0;
        //:to be done, check whether the interrupt is from the target channel
        TCStatus = LPC_GPDMA->DMACIntTCStat;
        ErrStatus= LPC_GPDMA->DMACIntErrStat;
        LPC_GPDMA->DMACIntTCClear |= TCStatus;
        LPC_GPDMA->DMACIntErrClr |=ErrStatus;
        if (TCStatus!=0)
            _Success.call();
        if (ErrStatus!=0 )
            _Err.call();
}

bool isMemory (uint32_t src){
    if ((src >> 28) == 0 || (src >> 28)== 1) 
			return 1;
		else
			return 0;
       
}

void DMA_IRQ_handler0(FunctionPointer _Success, FunctionPointer _Err){
        uint32_t TCStatus=0;
        uint32_t ErrStatus=0;
        //:to be done, check whether the interrupt is from the target channel
        TCStatus = LPC_GPDMA->DMACIntTCStat;
        ErrStatus= LPC_GPDMA->DMACIntErrStat;
        LPC_GPDMA->DMACIntTCClear |= TCStatus;
        LPC_GPDMA->DMACIntErrClr |=ErrStatus;
        if (TCStatus!=0 && (&_Success != NULL)) // to be reviewd, how to check whether Functionpointer _Err has been attached or not
            _Success.call();
        if (ErrStatus!=0 && (&_Err)) // to be reviewd, how to check whether Functionpointer _Err has been attached or not
            _Err.call();
}


DMA_PeripheralType getPeripheralType (uint32_t src){
    if (src >= 0x40100000 || src <= 0x40000000)    
	      return non_memory;	
		else if (src >= 0x4000C000 || src <= 0x40010000)
		    return UART0;
		else if (src >= 0x40010000 || src <= 0x40014000)	
				return UART1;
		else if (src >= 0x40030000 || src <= 0x40034000)
				return SSP1;
		else if (src >= 0x40040000 || src <= 0x40048000)
				return ADC;
    else if (src >= 0x40088000 || src <= 0x4008c000)
				return SSP0;
		else if (src >= 0x4008c000 || src <= 0x40090000)
				return DAC;
		else if (src >= 0x40098000 || src <= 0x4009c000)
				return UART2;
		else if (src >= 0x4009c000 || src <= 0x400A0000)
				return UART3;
		else if (src >= 0x400a8000 || src <= 0x400Ac000)
				return I2S; 
		return non_memory;
}	

static TransferType getTransferType (uint32_t src_addr, uint32_t dst_addr ){
    if (isMemory(src_addr)){    //if source is memory
		    if(isMemory(dst_addr))  //if destination is memory
					  return M2M;    //return M2M if source is memory and destination is memory. 	  
				else    //if destination is peripheral   
						return M2P;    //return M2P if source is memory and destination is peripheral
		}
		else{		//if source is peripheral
				if(isMemory(dst_addr))		//if destination is memory
				    return P2M;		//return P2M if source is peripheral and destination is memory 
				else		//if destination is peripheral
						return P2P;		//return P2P if source is peripheral and destination is peripheral 
		}		
}

/** 
  * @brief  Check whether channel is active or not
  * @param  channel. The channel number.  
  * @retval 0 or 1
  */
bool DMA_ChannelActive (int channel){
    return (LPC_GPDMA->DMACEnbldChns && (1<<channel));
}


/**
  * @brief  Enables or disables the specified DMAy Channelx.
  * @param  channel. DMA channel number. x can be 1 to 7 for DMA1 to select the DMA Channel.  
  * @param  NewState: new state of the DMAy Channelx. 
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void DMA_Cmd(int channel, FunctionalState NewState)
{
		LPC_GPDMACH_TypeDef* GPDMA_channel;	
	  /* get DMA channel address*/
		GPDMA_channel =	returnChannel(channel);
	
		while ( (LPC_GPDMA->DMACConfig & 0x01) == 0){ 
	      LPC_GPDMA->DMACConfig = 0x01; // enable DMA and little endian
		}
	   
    if (NewState != DISABLE)
    {
        /* Enable the selected DMAy Channelx */
        GPDMA_channel->DMACCConfig |= 1ul<<DMA_CCxConfig_E_Pos;
    }
    else
    {
        /* Disable the selected DMAy Channelx */
        GPDMA_channel->DMACCConfig &= (uint16_t)(~1ul<<DMA_CCxConfig_E_Pos);
    }
} // end of DMA_cmd

/** 
  * @brief  Mask or unmask selected interrupt
  * @param  channel. DMA channel number. x can be 1 to 7 for DMA1 to select the DMA Channel.  
  * @param  NewState: new state of the DMAy Channelx Interrupt. 
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */

void DMA_ITConfig (int channel, FunctionalState NewState)
{
	  LPC_GPDMACH_TypeDef* GPDMA_channel;	
	  /* get DMA channel address*/
		GPDMA_channel =	returnChannel(channel);
	
    if (NewState != DISABLE)
    {
        /*unmask ITC*/
        GPDMA_channel->DMACCConfig |= 1ul <<DMA_CCxConfig_ITC_Pos;
        GPDMA_channel->DMACCControl |= 1ul <<DMA_CCxControl_I_Pos; 
        /*unmask IE*/
        GPDMA_channel->DMACCConfig |= 1ul<< DMA_CCxConfig_IE_Pos;
    }
    else // to be consider: remove else?
    {
        /*mask ITC*/
        GPDMA_channel->DMACCConfig &= ~(1ul <<DMA_CCxConfig_ITC_Pos);
        GPDMA_channel->DMACCControl &= ~(1ul <<DMA_CCxControl_I_Pos);
     
       /*mask IE*/
        GPDMA_channel->DMACCConfig &= ~(1ul <<DMA_CCxConfig_IE_Pos);
    }      
} //end of DMA_ITConfig 


