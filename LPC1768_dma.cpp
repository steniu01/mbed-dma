#include "LPC1768_dma.h"
/**
  * @brief  Initializes the DMAy Channelx according to the specified parameters 
  *         in the DMA_InitStruct.
  * @param  x can be 1 to 7 for DMA1 to select the DMA Channel.
  * @param  DMA_InitStruct: pointer to a DMA_InitTypeDef structure that contains
  *         the configuration information for the specified DMA Channel.
  * @retval None
  */
void DMA_init(LPC_GPDMACH_TypeDef* DMAy_Channelx, DMA_InitTypeDef* DMA_InitStruct)
{
    uint32_t tmpreg = 0;

    
  /* clear the pending interrupts on the channel to be used by writing to the DMACIntTCClear and DMACIntErrClear register. */  
    if (DMAy_Channelx == LPC_GPDMACH0){
      /* Reset interrupt pending bits for DMA1 Channel1 */
        LPC_GPDMA->DMACIntTCClear = 1<<0;
        LPC_GPDMA->DMACIntErrClr = 1<<0;
    }
    else if (DMAy_Channelx == LPC_GPDMACH1){
     /* Reset interrupt pending bits for DMA1 Channel2 */
        LPC_GPDMA->DMACIntTCClear = 1<<1;
        LPC_GPDMA->DMACIntErrClr = 1<<1;
    }
    else if (DMAy_Channelx == LPC_GPDMACH2){
    /* Reset interrupt pending bits for DMA1 Channel3 */
        LPC_GPDMA->DMACIntTCClear = 1<<2;
        LPC_GPDMA->DMACIntErrClr = 1<<2;
    }
    else if (DMAy_Channelx == LPC_GPDMACH3){
    /* Reset interrupt pending bits for DMA1 Channel3 */
        LPC_GPDMA->DMACIntTCClear = 1<<3;
        LPC_GPDMA->DMACIntErrClr = 1<<3;
    }
    else if (DMAy_Channelx == LPC_GPDMACH4){
    /* Reset interrupt pending bits for DMA1 Channel4 */
        LPC_GPDMA->DMACIntTCClear = 1<<4;
        LPC_GPDMA->DMACIntErrClr = 1<<4;
    }
    else if (DMAy_Channelx == LPC_GPDMACH5){
    /* Reset interrupt pending bits for DMA1 Channel5 */
        LPC_GPDMA->DMACIntTCClear = 1<<5;
        LPC_GPDMA->DMACIntErrClr = 1<<5;
    }
    else if (DMAy_Channelx == LPC_GPDMACH6){
    /* Reset interrupt pending bits for DMA1 Channel6 */
        LPC_GPDMA->DMACIntTCClear = 1<<6;
        LPC_GPDMA->DMACIntErrClr = 1<<6;
    }
    else if (DMAy_Channelx == LPC_GPDMACH7) {
      /* Reset interrupt pending bits for DMA1 Channel7 */
        LPC_GPDMA->DMACIntTCClear = 1<<7;
        LPC_GPDMA->DMACIntErrClr = 1<<7;
    }
    
    
    
   // tmpreg = DMAy_Channelx->DMACControl;
    //tmpreg &= CControl_CLEAR_MASK; //undo
    
    DMAy_Channelx->DMACCSrcAddr = DMA_InitStruct->DMA_SrcAddr;
    DMAy_Channelx->DMACCDestAddr = DMA_InitStruct->DMA_DestAddr;
    //DMAy_Channelx->DMACCLLI = DMA_InitStruct->LLI->next; //undo
    DMAy_Channelx->DMACCControl |= (DMA_InitStruct->DMA_TransferSize<<DMA_CCxControl_TransferSize_Pos) |
                                  (DMA_InitStruct->DMA_SrcBurst<<DMA_CCxControl_SBSize_Pos)  |
                                  (DMA_InitStruct->DMA_DestBurst<<DMA_CCxControl_DBSize_Pos) |
                                  (DMA_InitStruct->DMA_SrcWidth<<DMA_CCxControl_SWidth_Pos)  |
                                  (DMA_InitStruct->DMA_DestWidth<<DMA_CCxControl_DWidth_Pos) |
                                  (DMA_InitStruct->DMA_SrcInc<<DMA_CCxControl_SI_Pos)    |
                                  (DMA_InitStruct->DMA_DestInc<<DMA_CCxControl_DI_Pos)   |
                                  (DMA_InitStruct->DMA_TermInt<<DMA_CCxControl_I_Pos ); 
    
    DMAy_Channelx->DMACCConfig |= (DMA_InitStruct->DMA_SrcPeripheral<<DMA_CCxConfig_SrcPeripheral_Pos)|
                                  (DMA_InitStruct->DMA_DestPeripheral<<DMA_CCxConfig_DestPeripheral_Pos)|
                                  (DMA_InitStruct->DMA_TransferType<< DMA_CCxConfig_TransferType_Pos);   
    
    //DMAy_Channelx->DMACCConfig=;

}

/**
  * @brief  Fills each DMA_InitStruct member with its default value.
  * @param  DMA_InitStruct: pointer to a DMA_InitTypeDef structure which will
  *         be initialized.
  * @retval None
  */
void DMA_StructInit(DMA_InitTypeDef* DMA_InitStruct)
{
/*-------------- Reset DMA init structure parameters values ------------------*/
  
    DMA_InitStruct->DMA_DestAddr=0; 
    DMA_InitStruct->DMA_SrcAddr=0;    
    //DMA_InitStruct->LLI;//undo
    DMA_InitStruct->DMA_TransferSize=0;
    DMA_InitStruct->DMA_SrcBurst=0; 
    DMA_InitStruct->DMA_DestBurst=0;
    DMA_InitStruct->DMA_SrcWidth=0;
    DMA_InitStruct->DMA_DestWidth=0;
    DMA_InitStruct->DMA_SrcInc=0; 
    DMA_InitStruct->DMA_DestInc=0; 
    DMA_InitStruct->DMA_TermInt=0; 
    
    DMA_InitStruct->DMA_SrcPeripheral=0;
    DMA_InitStruct->DMA_DestPeripheral=0;
    DMA_InitStruct->DMA_TransferType=0;       
}




/**
  * @brief  Enables or disables the specified DMAy Channelx.
  * @param  x can be 1 to 7 for DMA1 to select the DMA Channel.  
  * @param  NewState: new state of the DMAy Channelx. 
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void DMA_Cmd(LPC_GPDMACH_TypeDef*  DMAy_Channelx, FunctionalState NewState)
{
    /* Check the parameters */
    //assert_param(IS_DMA_ALL_PERIPH(DMAy_Channelx));
    //assert_param(IS_DMA_CONTROL(cmd));

  if (NewState != DISABLE)
  {
    /* Enable the selected DMAy Channelx */
    DMAy_Channelx->DMACCConfig |= 1ul<<DMA_CCxConfig_E_Pos;
  }
  else
  {
    /* Disable the selected DMAy Channelx */
    DMAy_Channelx->DMACCConfig &= (uint16_t)(~1ul<<DMA_CCxConfig_E_Pos);
  }
}



/* mask and unmask selected Interrupt: err interrupt or counter interrupt*/
void DMA_ITConfig (LPC_GPDMACH_TypeDef* DMAy_Channelx, uint32_t DMA_IT, FunctionalState NewState)
{
    if (NewState != DISABLE)
    {
        /*unmask the selected DMA interrupts*/
        if(DMA_IT == DMA_ITC )
        {
            DMAy_Channelx->DMACCConfig |= 1ul <<DMA_CCxConfig_ITC_Pos;
            DMAy_Channelx->DMACCControl |= 1ul <<DMA_CCxControl_I_Pos;
        } 
        else if (DMA_IT == DMA_IE )
        {
            DMAy_Channelx->DMACCConfig |= 1ul<< DMA_CCxConfig_IE_Pos;
        }
        
    }
    else 
    {
         /*mask the selected DMA interrupts*/
          if(DMA_IT == DMA_ITC )
        {
            DMAy_Channelx->DMACCConfig &= ~(1ul <<DMA_CCxConfig_ITC_Pos);
            DMAy_Channelx->DMACCControl &= ~(1ul <<DMA_CCxControl_I_Pos);
        } 
        else if (DMA_IT == DMA_IE)
        {
            DMAy_Channelx->DMACCConfig &= ~(1ul <<DMA_CCxConfig_IE_Pos);
        }  
    }    
}

void DMA_ClearITPendingBit(LPC_GPDMACH_TypeDef* DMAy_Channelx, uint32_t DMA_IT)
{
   
  /* clear the pending interrupts on the channel to be used by writing to the DMACIntTCClear and DMACIntErrClear register. */  
    if (DMAy_Channelx == LPC_GPDMACH0){
      /* Reset interrupt pending bits for DMA1 Channel1 */
        LPC_GPDMA->DMACIntTCClear = 1<<0;
        LPC_GPDMA->DMACIntErrClr = 1<<0;
    }
    else if (DMAy_Channelx == LPC_GPDMACH1){
     /* Reset interrupt pending bits for DMA1 Channel2 */
        LPC_GPDMA->DMACIntTCClear = 1<<1;
        LPC_GPDMA->DMACIntErrClr = 1<<1;
    }
    else if (DMAy_Channelx == LPC_GPDMACH2){
    /* Reset interrupt pending bits for DMA1 Channel3 */
        LPC_GPDMA->DMACIntTCClear = 1<<2;
        LPC_GPDMA->DMACIntErrClr = 1<<2;
    }
    else if (DMAy_Channelx== LPC_GPDMACH3){
    /* Reset interrupt pending bits for DMA1 Channel3 */
        LPC_GPDMA->DMACIntTCClear = 1<<3;
        LPC_GPDMA->DMACIntErrClr = 1<<3;
    }
    else if (DMAy_Channelx == LPC_GPDMACH4){
    /* Reset interrupt pending bits for DMA1 Channel4 */
        LPC_GPDMA->DMACIntTCClear = 1<<4;
        LPC_GPDMA->DMACIntErrClr = 1<<4;
    }
    else if (DMAy_Channelx == LPC_GPDMACH5){
    /* Reset interrupt pending bits for DMA1 Channel5 */
        LPC_GPDMA->DMACIntTCClear = 1<<5;
        LPC_GPDMA->DMACIntErrClr = 1<<5;
    }
    else if (DMAy_Channelx == LPC_GPDMACH6){
    /* Reset interrupt pending bits for DMA1 Channel6 */
        LPC_GPDMA->DMACIntTCClear = 1<<6;
        LPC_GPDMA->DMACIntErrClr = 1<<6;
    }
    else if (DMAy_Channelx == LPC_GPDMACH7) {
      /* Reset interrupt pending bits for DMA1 Channel7 */
        LPC_GPDMA->DMACIntTCClear = 1<<7;
        LPC_GPDMA->DMACIntErrClr = 1<<7;
    }
    
}

/*check which channels are free*/
uint32_t DMA_EnabledChannels(void){
    
    return (LPC_GPDMA->DMACEnbldChns & 0xff);
    
}

bool DMA_ChannelActive (LPC_GPDMACH_TypeDef* DMAy_Channelx){
    if( DMAy_Channelx->DMACCConfig && 1<<DMA_CCxConfig_A_Pos) 
      return 1;
    else
      return 0;   
}
