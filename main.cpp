/**
M2M : read content from a files using CPU, test the time; read content from a file using DMA. Compare the content
M2P : demenstrate while DMA-UART working, LED swtich is also working 
P2M : demenstrate m2m, m2p, p2m, could work together ; keep sending 
P2P
 
Do we need constructor?
Select which ever free or which ever not declared?
How to put comments?


Done 1. make init struct generic
2. make trigger type generic
3. support pass method callback
4. read_adc
5. PC app to received data from uart
*/

// disassembler memcpy
#include "mbed.h"



#define test_m2m 0
#define test_m2p 0
#define test_p2m 0
#define test_p2p 0
#define test_all 1
#define test_adc 0

LocalFileSystem local("local");

DigitalOut myled1(LED1);
DigitalOut myled2(LED2) ;
DigitalOut myled3(LED3) ;
DigitalOut myled4(LED4) ;

// Use Timer to measure the execution time
Timer t;

RawSerial pc (USBTX, USBRX);

// Callbacks for DMA interrupts
void led_switchon_m2m(void);
void led_switchon_m2p (void);
void led_switchon_p2m (void);
void led_switchon_p2p (void);
void led_show (void);
void IRQ_err (void);


void ADC_init();
void old_ADC_init();
void burst_ADC_init();

volatile bool m2p_finishFlag = 0; 

int main(void)
{

	
	 char src[] =   "Hello world Copy the logical and implementation_tsmc28hpm directories in to the Skadi home directo \r\n" \
									"Thank you for the demonstration. I understand now. I have come to terms with the fact that I misunderstood how \r\n" \
									"memory is laid out. I thought that each individual address was capable of storing an entire word, not just a byte \r\n" \
									"(for instance address 0x12345678 could hold a value of 0xffffffff and then 0x12345679 could hold a completely  \r\n" \
									"different value 0x00000000). Now I realize that 0x12345678 holds only one byte of a word and the next word \r\n" \
									"The logic for your memcpy is correct and your interviewer didn't ask you to change it or add a restriction.  \r\n" \
									"Hello world Copy the logical and implementation_tsmc28hpm directories in to the Skadi home directo \r\n" \
									"Thank you for the demonstration. I understand now. I have come to terms with the fact that I misunderstood how \r\n" \
									"memory is laid out. I thought that each individual address was capable of storing an entire word, not just a byte \r\n" \
									"(for instance address 0x12345678 could hold a value of 0xffffffff and then 0x12345679 could hold a completely  \r\n" \
									"different value 0x00000000). Now I realize that 0x12345678 holds only one byte of a word and the next word \r\n" \
									"The logic for your memcpy is correct and your interviewer didn't ask you to change it or add a restriction.  \r\n" \
									"Copying 4 bytes at a time is faster, but becomes a problem if your size is not a multiple of 4. Hence your  \r\n" \
									"interviewer told you to use two loops: the first copies 4 bytes at a time, and the 2nd loop one byte at a time \r\n" \
									"So the bulk of the copy is done with the fast 4-byte copy, but you're not restricted to size being a multiple of 4  \r\n" \
									"There can be problems even if the total data to copy is a multiple of 4 bytes. The first pointer might point to a  \r\n" \
									"The interviewer was testing your knowledge of computer architecture, and wanted you to optimize your  \r\n" \
									"algorithm. Memory operates on words rather than bytes. In a 32-bit system, a word is typically 4 bytes, it  \r\n" \
									"akes the same amount of time to read/write 1 byte as it does to read/write 1 word. The second loop is to  \r\n" \
									"Hello world Copy the logical and implementation_tsmc28hpm directories in to the Skadi home directory\r\n" ;
						
									
	

	
#if test_m2m
    /* test the DMA M2M, copy data from src to dest, and then print out the dest mem data */

    pc.printf("start to test DMA M2M test now!\r\n");
    wait(2);				
    size_t size = sizeof (src);
    char *dst1  = (char *) malloc(size);
		char *dst2  = (char *) malloc(size);
    memset (dst1, '\0', size);
		memset (dst2, '\0', size);
		t.start();
  	memcpy (dst1,src,size);
	  t.stop();
		printf("The source size is %d\r\n", size);
		printf("The time CPU took was %f seconds\r\n", t.read());
		t.reset();

    DMA dma1 (-1) ; // choose whichever free channel
    dma1.source (src,1, 8); // set source as incremental. Not need to set the transfer width as MBED dma will do it for you.
    dma1.destination (dst2, 1, 8);
    dma1.attach_TC(led_switchon_m2m) ;
		dma1.attach_Err (IRQ_err);
		dma1.init();
		
		t.start();
	  dma1.start(size);
    dma1.wait();
		
		
	  pc.printf("The time DMA took was %f seconds\r\n", t.read());
    pc.printf("dst text: %s \r\n", dst2);
    if (strcmp (dst2, src) != 0)
        pc.printf("error! \r\n");
    else
        pc.printf("correct! \r\n");

   
#endif

#if test_m2p
    /*Test m2P, send the memory data to UART;*/ 
    pc.printf ("start to test m2p now\r\n");
		pc.printf ("It will demonstrate CPU and DMA could work together\r\n");

		t.start();	
	  pc.printf(src);
  	t.stop();
 		wait(2);
		printf("The time CPU took was %f seconds\r\n", t.read());
    wait(2);
		t.reset();
		
		m2p_finishFlag = 0;
    LPC_UART0->FCR  |=  1<<3 ; //Enable UART DMA mode
		LPC_UART0->LCR &= ~(1<<3); // No parity bit genrated 
		
    DMA dma2(-1);
    dma2.destination(&(LPC_UART0->THR),0, sizeof(char)*8);
    dma2.source(src,1, sizeof(char)*8);
    dma2.TriggerDestination(_UART0_TX);
    dma2.attach_TC(led_switchon_m2p);//  m2p_finishFlag will be set when FINISH interrupt generated
		dma2.attach_Err (IRQ_err);
		dma2.init(); // must call init after have setting the DMA
		t.start();
    dma2.start(sizeof(src));
		dma2.wait();
    t.stop();
		printf("The time DMA took was %f seconds\r\n", t.read());
		
		
		// Demonstrate CPU and DMA could work together, led2 keep blinking while DMA is transferring data 
		while (! m2p_finishFlag){
		    myled2 = !myled2;
			  wait (0.2);
		}
		
		// Demonstrate Err interrupt callback also works
		// Dedicately read data from the unallocated memory range to generate error 
		wait (2);
		pc.printf ("now demonstrate error interrupt callback also works\r\n");
		wait(2);
		dma2.attach_Err (IRQ_err);
	  dma2.start((sizeof(src)+4)); 
		dma2.wait();
		
#endif

#if test_p2m
    
    pc.printf("start to test DMA P2M now!\r\n");
    wait(2);
    volatile unsigned int data = 0;
		unsigned int data2 = 0;
		volatile int *dst3  = (int *) malloc(32);
    old_ADC_init();
		NVIC_DisableIRQ(ADC_IRQn); // If ADC DMA is used, the ADC interrupt must be disabled 	
    
	
    DMA dma3(3);
    dma3.destination(dst3, 0, 32); // Some sample codes show it should be half-word as only low 16bit contains the data. 
															   	//	But I think it should be 32 as the reigster is 32-bit width 
    dma3.source(&(LPC_ADC->ADDR4),0, 32);
    dma3.TriggerSource(_ADC);
    dma3.attach_TC(led_switchon_p2m);
		
		while (1)
		{
        LPC_ADC->ADCR |= 1 << 24;
		    dma3.start(1);
			  dma3.wait();
			  data= float(dst3[0]>>4 & 0xfff); // Only bit4-bit16 contains the valid ADC data
			  pc.printf("The ADC data is: %d\r\n", data);
	   	  wait (1);
		}
			

    pc.printf("\nFinish!\r\n");
#endif
		
#if test_p2p
    
    pc.printf("start to test DMA P2P now!\r\n");
    wait(2);
    ADC_init();
		NVIC_DisableIRQ(ADC_IRQn); // If ADC DMA is used, the ADC interrupt must be disabled 	
    
    DMA dma4(4);
    dma4.destination(&(LPC_UART0->THR),32,0);  													
																            
    dma4.source(&(LPC_ADC->ADDR0),32,0);// some sample codes show it should be half-word as only low 16 bit contains the data.
																				// but I think it should be 32 as the reigster is 32-bit width 
    dma4.TriggerSource(_ADC);
		dma4.TriggerDestination(_UART0_TX);
    dma4.attach_TC(led_switchon_p2p);
		dma4.start(4);
    dma4.wait();
    
    pc.printf("\nFinish!\r\n");
#endif

#if test_all
		
    DMA dma1 (1) ; 
		DMA dma2 (2);
		DMA dma3 (3);
		
		size_t size1 = sizeof (src);
    char *dst1  = (char *) malloc(size1);
    dma1.source (src,1); // set source as incremental. Not need to set the transfer width as MBED dma will do it for you.
    dma1.destination (dst1, 1);
    dma1.attach_TC(led_switchon_m2m) ;
		dma1.attach_Err (IRQ_err);
		dma1.init();


   // char src2[] = "This message was transmitted via UART DMA from memor";
		char src2[] = "This is memory type" ;
		size_t size2 = sizeof (src2);
		pc.printf("the size is %d",size2);
		wait(1);
    LPC_UART0->FCR  |=  1<<3 ; //Enable UART DMA mode
		LPC_UART0->LCR &= ~(1<<3); // No parity bit genrated 
    dma2.destination(&(LPC_UART0->THR),0, sizeof(char)*8);
    dma2.source(src2,1, sizeof(char)*8);
    dma2.TriggerDestination(_UART0_TX);
    dma2.attach_TC(led_switchon_m2p);//  m2p_finishFlag will be set when FINISH interrupt generated
 	  dma2.attach_Err (IRQ_err);
    dma2.init();
 
		volatile unsigned int data = 0;
		volatile int *dst3  = (int *) malloc(128);
    ADC_init();
		NVIC_DisableIRQ(ADC_IRQn); // If ADC DMA is used, the ADC interrupt must be disabled 	
    dma3.destination(dst3, 1, 32); // Some sample codes show it should be half-word as only low 16bit contains the data. 
															   	//	But I think it should be 32 as the reigster is 32-bit width 
    dma3.source(&(LPC_ADC->ADDR0),0, 32);
    dma3.TriggerSource(_ADC);
    dma3.attach_TC(led_switchon_p2m);
    dma3.attach_Err (IRQ_err);
    dma3.init();

		dma1.start(size1);
		dma2.start(size2);
    dma3.start(32);

#endif

#if 0

    pc.printf("start to test DMA P2M now!\r\n");
    wait(2);
    volatile unsigned int data = 0;
		unsigned int data2 = 0;
		volatile int *dst3  = (int *) malloc(128);
    ADC_init();
		NVIC_DisableIRQ(ADC_IRQn); // If ADC DMA is used, the ADC interrupt must be disabled 	
    
	
    DMA dma3(3);
    dma3.destination(dst3, 1, 32); // Some sample codes show it should be half-word as only low 16bit contains the data. 
															   	//	But I think it should be 32 as the reigster is 32-bit width 
    dma3.source(&(LPC_ADC->ADDR0),0, 32);
    dma3.TriggerSource(_ADC);
    dma3.attach_TC(led_switchon_p2m);
		t.start();
		dma3.start(32);
		dma3.wait();
    t.stop();
		
	  printf("The time CPU took was %f seconds\r\n", t.read());

			
		for (int i =0; i<32; i++)
		{
			  data= float(dst3[i]>>4 & 0xfff); // Only bit4-bit16 contains the valid ADC data
			  pc.printf("The ADC data is: %d\r\n", data);
		}


    pc.printf("\nFinish!\r\n");



#endif 
    while (1);
		return 0;
}


void ADC_init()
{
   // ensure power is turned on
    LPC_SC->PCONP |= (1 << 12);
	
	  // set PCLK of ADC to 1
	  LPC_SC->PCLKSEL0 &= ~(0x3 << 24);
	  LPC_SC->PCLKSEL0 |= 1<<24;
	
	  // select PIN 14, ADC 0.0
  	LPC_PINCON->PINSEL1 &= (uint32_t) ~(0x3 << 14);
	  LPC_PINCON->PINSEL1 |= 0x1 << 14;
	
   	// PIN14 no pull-up, no pull-down 
    LPC_PINCON->PINMODE2 |= 0x2 << 28;
	
	  // ADC conversion not start yet
	  LPC_ADC->ADCR &= ~ (1 << 24);
	
		// the global DONE flat in ADDR must be disabled to generate an interrupt 
	  LPC_ADC->ADINTEN &= ~(1<<8); 
	  LPC_ADC->ADINTEN = 0x1;
	 
	  LPC_ADC->ADCR = (uint32_t)  ((1 << 21)  // ADC is operational  
	                              | (1 << 8) // APB clock is divided by 1
	                              | 0x01); // Select AD0.0 to be sampled and converted 
	

     LPC_ADC->ADCR |= 1 << 16 ; // BURST mode

}
	
void led_switchon_m2m(void)
{
    t.stop();
	  pc.printf("The time DMA took was %f seconds\r\n", t.read());
    myled1=1;
}

void led_switchon_m2p(void)
{
	  m2p_finishFlag = 1;
    myled2=1;
}

void led_switchon_p2m(void)
{
    myled3=1;
}

void led_switchon_p2p(void)
{
    myled4=1;
}

void IRQ_err (void)
{
	t.stop();
	pc.printf("The time DMA took was %f seconds\r\n", t.read());
	myled1 = 0;
	myled2 = 0;
	myled3 = 0;
	myled4 = 0;
  wait (0.5);	
	pc.printf ("\r\n");
  pc.printf ("Error Interrupt happens\r\n");
}


static inline int div_round_up(int x, int y)
{
    return (x + (y - 1)) / y;
}
void old_ADC_init()
{
    // ensure power is turned on
    LPC_SC->PCONP |= (1 << 12);

    /* p19 pin is set to ADC input.*/
    LPC_PINCON->PINSEL3 |= 0x30000000;
    /* no pull-up, no pull-down */
    LPC_PINCON->PINMODE3 |= 0x20000000;

    // set PCLK of ADC to 1
    LPC_SC->PCLKSEL0 &= ~(0x3 << 24);
    LPC_SC->PCLKSEL0 |= (0x1 << 24);
    uint32_t PCLK = SystemCoreClock;

    // calculate minimum clock divider
    // clkdiv = divider - 1
    uint32_t MAX_ADC_CLK = 13000000;
    uint32_t clkdiv = div_round_up(PCLK, MAX_ADC_CLK) - 1;
	
	  
	  LPC_ADC->ADCR &=  ~(1 << 0) // SEL: 0 = no channels selected  
						          & ~ (1 << 16) // No BURST mode
											& ~(1 << 17)     // CLKS: not applicable
										  & ~(1 << 24); // ADC conversion not start yet
											
		
	  LPC_ADC->ADINTEN = 1 <<8; 
    // Set the generic software-controlled ADC settings
    LPC_ADC->ADCR |= ( 24 << 8) // CLKDIV: PCLK max ~= 25MHz, /25 to give safe 1MHz at fastest         
                     | (1 << 21) ; // PDN: 1 = operational
   

    // Select the  channel 4 
    LPC_ADC->ADCR |= 1 << 4;
		// Start conversionf
    LPC_ADC->ADCR |= 1 << 24;
}

void burst_ADC_init()
{
    // ensure power is turned on
    LPC_SC->PCONP |= (1 << 12);

    /* p19 pin is set to ADC input.*/
    LPC_PINCON->PINSEL3 |= 0x30000000;
    /* no pull-up, no pull-down */
    LPC_PINCON->PINMODE3 |= 0x20000000;

    // set PCLK of ADC to 1
    LPC_SC->PCLKSEL0 &= ~(0x3 << 24);
    LPC_SC->PCLKSEL0 |= (0x1 << 24);
    uint32_t PCLK = SystemCoreClock;

    // calculate minimum clock divider
    // clkdiv = divider - 1
    uint32_t MAX_ADC_CLK = 13000000;
    uint32_t clkdiv = div_round_up(40000, MAX_ADC_CLK) - 1;
	
	  
	  LPC_ADC->ADCR &=  ~(1 << 0) // SEL: 0 = no channels selected  
											& ~(1 << 17)     // CLKS: not applicable
										  & ~(1 << 24); // ADC conversion not start yet
											
		// ADGINTEN must be set to 0 in burst mode 
	  LPC_ADC->ADINTEN &= ~(1<<8); 
    // Set the generic software-controlled ADC settings
    LPC_ADC->ADCR |= (clkdiv << 8)		// CLKDIV: PCLK max ~= 25MHz, /25 to give safe 1MHz at fastest  
										 | (1 << 16)  // Burst mode
                     | (1 << 21) ; // PDN: 1 = operational
   

    // Select the  channel 4 
    LPC_ADC->ADCR |= 1 << 4;
		// Start conversion
    LPC_ADC->ADCR |= 1 << 24;
}
