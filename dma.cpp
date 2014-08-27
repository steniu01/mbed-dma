
#include "dma.h"

 DMA* DMA::dma=NULL; // need to add declaretion in the cpp otherwise report "undefined symbols" at link

/* if no channel choosed, choose whichever free channel, otherwise, wait until the channel is free*/
int DMA::chooseFreeChannel (int _channel){
			int reval=_channel;
			if (_channel > (channel_num-1) || channel < 0){
			     reval = 0; 
					 while (1)
					 {
					  if (!DMA_ChannelActive(reval))
						  	return reval;
						reval++;
						if (reval>(channel_num-1))
						    reval=0;
					 }
			 } 
			 else
			     while (DMA_ChannelActive(reval));//if has chosen the channel, wait until the channel is free
			 return reval;
}


void  DMA::IRQ_handler( void){
		
	  
		uint32_t TCStatus=0;
		uint32_t ErrStatus=0;
		//:to be done, check whether the interrupt is from the target channel
		TCStatus = LPC_GPDMA->DMACIntTCStat;
		ErrStatus= LPC_GPDMA->DMACIntErrStat;
	 	LPC_GPDMA->DMACIntTCClear |= TCStatus;
		LPC_GPDMA->DMACIntErrClr |=ErrStatus;
		if (TCStatus!=0)
		    dma->_TCCallback->call();
		if (ErrStatus!=0)
		   dma->_ErrCallback->call();
		
}

DMA::DMA ( int priority){
    channel = chooseFreeChannel(priority) ;
		switch (channel){
				case 0: GPDMA_Channel= LPC_GPDMACH0; break;
			  case 1: GPDMA_Channel= LPC_GPDMACH1; break;
		    case 2: GPDMA_Channel= LPC_GPDMACH2; break;
			  case 3: GPDMA_Channel= LPC_GPDMACH3; break;
				case 4: GPDMA_Channel= LPC_GPDMACH4; break;
				case 5: GPDMA_Channel= LPC_GPDMACH5; break;
				case 6: GPDMA_Channel= LPC_GPDMACH6; break;
				case 7: GPDMA_Channel= LPC_GPDMACH7; break;
				default: GPDMA_Channel= LPC_GPDMACH0; break;
	  }
		dma=this; // point the static class pointer to the current object
	  LPC_SC->PCONP |= (1 << 29);    /* Enable GPDMA clock */
		dma_init_struct.DMA_TermInt = 1;
    DMA_StructInit(&dma_init_struct); 
   
    
}


DMA::DMA (){
    channel = chooseFreeChannel(-1) ;
		
		switch (channel){
				case 0: GPDMA_Channel= LPC_GPDMACH0; break;
			  case 1: GPDMA_Channel= LPC_GPDMACH1; break;
		    case 2: GPDMA_Channel= LPC_GPDMACH2; break;
			  case 3: GPDMA_Channel= LPC_GPDMACH3; break;
				case 4: GPDMA_Channel= LPC_GPDMACH4; break;
				case 5: GPDMA_Channel= LPC_GPDMACH5; break;
				case 6: GPDMA_Channel= LPC_GPDMACH6; break;
				case 7: GPDMA_Channel= LPC_GPDMACH7; break;
				default: GPDMA_Channel= LPC_GPDMACH0; break;
	  }
		
	  LPC_SC->PCONP |= (1 << 29);    /* Enable GPDMA clock */
	
    DMA_StructInit(&dma_init_struct); 

    
}

void DMA::destination (uint32_t dst, int width, bool inc){
		dma_init_struct.DMA_DestAddr = dst;
		dma_init_struct.DMA_DestBurst = 0x01;
		dma_init_struct.DMA_DestWidth = width;
		dma_init_struct.DMA_DestInc = inc;
}

void DMA::source (uint32_t src, int width, bool inc){
    dma_init_struct.DMA_SrcAddr = src;
		dma_init_struct.DMA_SrcBurst = 0x01; // 1 byte
    dma_init_struct.DMA_SrcWidth = width;
		dma_init_struct.DMA_SrcInc = inc;	
}
	
void DMA::getTransferType (TransferType type){
		dma_init_struct.DMA_TransferType = type;
}

/*
template <typename T>
void DMA::init (T src, T dst, TransferType type)
{
    
    
    //dma_init_struct.DMA_TransferSize = size;
    
  
     sizeof(*src); // caculate the width according to the input
   
		{
		if (type==M2M || type==M2P)
				
		else
    dma_init_struct.DMA_SrcInc = 0;
		}
		{
		if (type==M2M || type==P2M)
			
		else
			 dma_init_struct.DMA_DestInc = 0;
		}

   
    dma_init_struct.DMA_TransferType = type;
} */

void DMA::start (int length) {
    dma_init_struct.DMA_TransferSize = length;
   
	  LPC_SC->PCONP |= (1 << 29);    /* Enable GPDMA clock */ 
		DMA_init(GPDMA_Channel, &dma_init_struct);
	  LPC_GPDMA->DMACConfig = 0x01; // enable DMA and little endian
	  while ( (LPC_GPDMA->DMACConfig & 0x01) == 0);   

    DMA_Cmd(GPDMA_Channel, ENABLE); // enable channel
		DMA_ITConfig (GPDMA_Channel, DMA_ITC, ENABLE);
  
    
}


bool DMA::wait(){
    printf ("the channel is %d", channel);
		while (DMA_ChannelActive(channel));
	
}	

