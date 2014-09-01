#include "LPC1768_dma.h"
#include "mbed.h"


class DMA {
public: 
		DMA_InitTypeDef dma_init_struct;
    DMA(int prority = -1); // constructor
		template<typename T> 
    void source (T src, int width = sizeof(T), bool inc =0){
				inc = isMemory ((uint32_t) src);
		    DMA_source(&dma_init_struct, (uint32_t)src, width, inc);
		}
		template<typename T> 
    void destination (T dst, int width = sizeof(T), bool inc = 0){
			inc = isMemory ((uint32_t) dst);
			DMA_destination(&dma_init_struct, (uint32_t)dst, width, inc);
		}
		void trigger(TriggerType trig);
    void start ( int length);  
    void wait();

		void attach_TC(void (*fptr)(void)){
                _TCCallback.attach(fptr);
                NVIC_SetVector(DMA_IRQn, (uint32_t)&IRQ_handler);
                NVIC_EnableIRQ(DMA_IRQn);   
        }
        template<typename T> //has to use template in the header file
        void attach_TC(T* tptr, void (T::*mptr)(void)) {  
                if((mptr != NULL) && (tptr != NULL)) {
                _TCCallback.attach(tptr, mptr);
                }
                NVIC_SetVector(DMA_IRQn, (uint32_t)&IRQ_handler);
                NVIC_EnableIRQ(DMA_IRQn); 
        }

        void attach_Err(void (*fptr)(void)){
            _ErrCallback.attach(fptr);
                NVIC_SetVector(DMA_IRQn, (uint32_t)&IRQ_handler);
            NVIC_EnableIRQ(DMA_IRQn);   
        }
        template<typename T> //has to use template in the header file
        void attach_Err(T* tptr, void (T::*mptr)(void)) {  
                if((mptr != NULL) && (tptr != NULL)) {
                        _ErrCallback.attach(tptr, mptr);
                }
                NVIC_SetVector(DMA_IRQn, (uint32_t)&IRQ_handler);
                NVIC_EnableIRQ(DMA_IRQn); 
        }
    
    
protected: 
      FunctionPointer _ErrCallback ; // if declare as a function pointer, it will hang when attach
      FunctionPointer _TCCallback;   
       				
        int chan;
				int channel_num;
        static DMA* dma;// To be reviewed, whether should declare a instance here?
        int chooseFreeChannel(int channel);
        static void IRQ_handler( void); // need to be static to be passed to the NVIC_SetVector

};
