#include "dma.h"

namespace mbed {


DMA::DMA (int priority):
channel_num(_channel_num) 
{
    chan = chooseFreeChannel(priority) ;
//    channel_num = _channel_num;
    DMA_reset(chan);// reset the register value to default and clear the interrupts
    dma_init_struct = DMA_struct_create(); // initialize the dma data structure to default value
}


DMA::~DMA(){
   DMA_struct_delete(dma_init_struct);
	 DMA_IRQ_detach(chan);
}

// if no channel choosed, choose whichever free channel, otherwise, wait until the channel is free
int DMA::chooseFreeChannel (int channel)
{
    int reval=channel;
    if (channel > (channel_num-1) || channel < 0) {
        reval = 0;
        while (1) { //if not chosen channel, round robin checked which channel is free
            if (!DMA_channel_active(reval))
                return reval;
            reval++;
            if (reval>(channel_num-1))
                reval=0;
        }
    } 
		else
		{
		while (DMA_channel_active(reval));//if has chosen the channel, wait until the channel is free
		}
    return reval;
}

void DMA::TriggerSource(TriggerType trig)
{
    DMA_trigger_source(dma_init_struct, trig);
}

void DMA::TriggerDestination(TriggerType trig)
{
    DMA_trigger_destination(dma_init_struct, trig);
}

/*
void DMA::next(LLI* list)
{
		DMA_next(dma_init_struct, list);
}
*/

void DMA::next(const uint32_t src, const uint32_t dst, uint32_t size)
{
    DMA_next(dma_init_struct, src, dst, size);
}

void DMA::start (unsigned int len)
{
  	DMA_init(chan,dma_init_struct);
	  DMA_start(chan, dma_init_struct, len);
}

void DMA::wait()
{
    while (DMA_channel_active(chan));
}

bool DMA::finished()
{
    return !DMA_channel_active(chan);
}
}
