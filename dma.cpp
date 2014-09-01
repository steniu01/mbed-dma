
#include "dma.h"

 DMA* DMA::dma=NULL;		//need to add declaretion in the cpp otherwise report "undefined symbols" at link

/* if no channel choosed, choose whichever free channel, otherwise, wait until the channel is free*/
int DMA::chooseFreeChannel (int channel){
    int reval=channel;
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

  DMA_IRQ_handler0 (dma->_TCCallback, dma->_ErrCallback); // all memebers in static function should be static
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



DMA::DMA ( int priority ){
    chan = chooseFreeChannel(priority) ;
    channel_num = _channel_num;    	
    dma=this; // point the static class pointer to the current object
    DMA_StructInit(&dma_init_struct); 
}

template void DMA::source <int> (int, int, bool);
template void DMA::source <char> (char, int, bool);
/*
template<typename T> 
void DMA::source (T src, int width, bool inc ){
		inc = isMemory ((uint32_t) src);
		DMA_source(&dma_init_struct, (uint32_t)src, width, inc);
}
/*
 /*   
template<typename T> 
void DMA::destination (T dst, int width, bool inc){
		inc = isMemory ((uint32_t) dst);
    DMA_destination(&dma_init_struct, (uint32_t)dst, width, inc);
}
*/
void DMA::trigger(TriggerType trig){
		DMA_trigger(&dma_init_struct, trig);
}

void DMA::start ( int length){
    DMA_TransferSize(&dma_init_struct, (uint32_t)length); 
		DMA_init(chan, &dma_init_struct);
		DMA_Cmd(chan, ENABLE); // enable channel
		DMA_ITConfig (chan, ENABLE);
} 

void DMA::wait(){
    while (DMA_ChannelActive(chan));
}



