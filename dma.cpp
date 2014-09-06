
#include "dma.h"



	
DMA* DMA::dma=NULL;		//To be reviewed. need to add declaretion in the cpp otherwise report "undefined symbols" at link

DMA::DMA ( int priority ){
	
    chan = chooseFreeChannel(priority) ;
    channel_num = _channel_num;    	
	//	DMA_ITConfig (chan, DISABLE);
    dma=this; // point the static class pointer to the current object
		DMA_deinit(chan,&dma_init_struct);// reset the register value to default and clear the interrupts
    DMA_StructInit(&dma_init_struct); // initialize the dma data structure to default value
}


// if no channel choosed, choose whichever free channel, otherwise, wait until the channel is free
int DMA::chooseFreeChannel (int channel){
    int reval=channel;
  // LPC_SC->PCONP |= (1 << 29); 
    if (channel > (channel_num-1) || channel < 0){
	      reval = 0;    
				while (1)		//if not chosen channel, round robin checked which channel is free 			
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
	 // DMA_ITConfig (dma->chan,DISABLE);

	 //only call the attached function when certain interrupt happened on the right channel
	  if (DMA_ITStatus(dma->chan, ERR))
		  dma->_ErrCallback.call();
  	 if (DMA_ITStatus(dma->chan, FINISH) )
		  dma->_TCCallback.call();
		DMA_IRQ_handler0 (); // the DMA handler implemented by per platform
	// DMA_ITConfig (dma->chan,DISABLE);
}
/*
void  DMA::IRQ_handler( void){
        uint32_t TCStatus=0;
        uint32_t ErrStatus=0;
        //:to be done, check whether the interrupt is from the target channel
        TCStatus = LPC_GPDMA->DMACIntTCStat;
        ErrStatus= LPC_GPDMA->DMACIntErrStat;
        LPC_GPDMA->DMACIntTCClear |= TCStatus;
        LPC_GPDMA->DMACIntErrClr |=ErrStatus;
        if (TCStatus!=0)
            dma->_TCCallback.call();
        if (ErrStatus!=0)
            dma->_ErrCallback.call();
}
*/





/*
		template<typename T>    // To be reviewd, should put the template into the cpp rather than the header?
    void source (T* src, int width = sizeof(T)*8, bool inc =0){
				inc = isMemory ((uint32_t) src);
		    DMA_source(&dma_init_struct, (uint32_t)src, width, inc);
		}
		template<typename T> 
    void destination (T* dst, int width = sizeof(T)*8, bool inc = 0){
			inc = isMemory ((uint32_t) dst);
			DMA_destination(&dma_init_struct, (uint32_t)dst, width, inc);
		}
*/
void DMA::TriggerSource(TriggerType trig){
		DMA_TriggerSource(&dma_init_struct, trig);
}

void DMA::TriggerDestination(TriggerType trig){
		DMA_TriggerDestination(&dma_init_struct, trig);
}

void DMA::start ( int length){
    DMA_TransferSize(&dma_init_struct, (uint32_t)length); 
		DMA_init(chan, &dma_init_struct);
		DMA_Cmd(chan, ENABLE); // enable channel
		DMA_ITConfig (chan, ENABLE); // enable interrupt
} 

void DMA::wait(){
    while (DMA_ChannelActive(chan));
}


