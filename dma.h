
#include "dma_api.h"
#include "mbed.h"

/** A generic DMA for transfer data without hanging the CPU.
 * It can be used for m2m, m2p, p2m, m2m transfer
 * It will choose the DMA with the priority you have set. 
 * Lower number means higher prority. 
 * If no prority is set, it will choose which ever free channel.
 * Example:
 * @code
 * // Send the memory data "Hello world" in source address to destination address via DMA
 * // attach a function to swtich LED on when the transfer finish 
 *	char src[] = "Hello world\r\n";
 *	uint8_t size = sizeof (src);
 *  char *dst  = (char *) malloc(size);
 *  memset (dst, '\0', size);
 *  LPC_SC->PCONP |= (1 << 29);    // Enable LPC1768 GPDMA clock 
 *  DMA dma1 (0) ;
 *  dma1.source (src,0,1);
 *	dma1.destination (dst,0,1);
 *	dma1.attach_TC(led_switchon) ;
 *  dma1.start(size);
 *  dma1.wait();
 *  printf("dst text: %s", dst);
 * @endcode
*/


class DMA {
public:
		/** @brief  Create the DMA Channel according to the prority or choose whichever free if no prority given. 
		*					  Initialize the number of channels in the device
		*  @param   priority: The DMA prority.
		*	 @note 	  If prority has been given but that channel is not avaiable, it will wait until the channel avaiable.
		*/
		DMA(int priority = -1); 
	
		/** @brief  Get source starting address, transfer width and setting auto increment.
		 *  @param  src. The source starting address. 
		 *	@param  width. The transfer data width
		 *  @param  inc. Set memory automatice increment.
		 *	@note 	If width is not given, it will set the width automatically according to the data type in the memory	
		 *					if increment is not given. It will set automatically according to whether the source is within memory range.
		 */
		/*template<typename T>    // To be reviewd, should put the template into the cpp rather than the header?
    void source (T* src, int width = (sizeof(T)-1), bool inc = 0){
				inc = isMemory ((uint32_t) src); // To be reviewed. How to put this line into the arguments?
		    DMA_source(&dma_init_struct, (uint32_t)src, width, inc);
		} */

		 void source (uint32_t src, int width, bool inc = 0){
				inc = isMemory ((uint32_t) src); // To be reviewed. How to put this line into the arguments?
		    DMA_source(&dma_init_struct, src, width, inc);
		}
		
		/** @brief  Get destination starting address, transfer width and setting auto increment.
		 *  @param  dst. The destination starting address. 
		 *	@param  width. The transfer data width
		 *  @param  inc. Set memory automatice increment.
		 *	@note 	If width is not given, it will set the width automatically according to the data type in the memory	
		 *					if increment is not given. It will set automatically according to whether the source is within memory range.
		 */
		/*template<typename T> 
    void destination (T* dst, int width = (sizeof(T)-1), bool inc = 0){
			inc = isMemory ((uint32_t) dst);
			DMA_destination(&dma_init_struct, (uint32_t)dst, width, inc);
		} */
		
		  void destination ( uint32_t dst, int width , bool inc = 0){
			inc = isMemory ((uint32_t) dst);
			DMA_destination(&dma_init_struct, dst, width, inc);
		}
		
		
		/** @brief  Get source trigger type
		 *  @param  trig. The triggert type. 
		 *	@note 	If the source is memory. The trigger type would be ALWAYS even user set it differently.				
		 */
		void TriggerSource(TriggerType trig = ALWAYS);
		
		/** @brief  Get destination trigger type
		 *  @param  trig. The triggert type. 
		 *	@note 	If the destination is memory. The trigger type would be ALWAYS even user set it differently.				
		 */
		void TriggerDestination(TriggerType trig = ALWAYS );
		
		/** @brief  Start the DMA to transfer data
		 *  @param  lenght. Define how many data the DMA needs to transfer			
		 */
    void start ( int length); 
		
		/** @brief  Wait for DMA
		 *  @param  trig. The triggert type. 
		 *	@note 	If the destination is memory. The trigger type would be ALWAYS even user set it differently.				
		 */		
    void wait();

		/** @brief  Attach a function to DMA IRQ handler. The attached function will be called when the transfer has completed successfully. 
		 *  @param  *fptr. The function pointer. 			
		 */		
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
		/** @brief  Attach a function to DMA IRQ handler. The attached function will be called when the transfer has completed successfully. 
		 *  @param  *fptr. The function pointer. 
		 *	@note 	There are two attach function attach_TC and attach_Err in case the user want to attach different functions when tranfer finish or fail				
		 */		
    void attach_Err(void (*fptr)(void)){
            _ErrCallback.attach(fptr);
            NVIC_SetVector(DMA_IRQn, (uint32_t)&IRQ_handler);
            NVIC_EnableIRQ(DMA_IRQn);   
        }
        template<typename T> 
        void attach_Err(T* tptr, void (T::*mptr)(void)) {  
                if((mptr != NULL) && (tptr != NULL)) {
                        _ErrCallback.attach(tptr, mptr);
                }
                NVIC_SetVector(DMA_IRQn, (uint32_t)&IRQ_handler);
                NVIC_EnableIRQ(DMA_IRQn); 
        }
    DMA_InitTypeDef dma_init_struct;
		int channel_num;
    
protected: 
      FunctionPointer _ErrCallback ; // if declare as a function pointer, it will hang when attach
      FunctionPointer _TCCallback;   	
      int chan;
      static DMA* dma ;// To be reviewed, whether should declare a instance here?
      int chooseFreeChannel(int channel);
      static void IRQ_handler( void); // need to be static to be passed to the NVIC_SetVector

};


