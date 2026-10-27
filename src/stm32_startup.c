#include <stdint.h>

#define SRAM_START 		0x20000000U
#define SRAM_SIZE 		(128 * 1024) // 128KB
#define SRAM_END 		((SRAM_START) + (SRAM_SIZE))

#define STACK_START 	SRAM_END

extern uint32_t _etext;
extern uint32_t _sdata;
extern uint32_t _edata;
extern uint32_t _la_data;
extern uint32_t _sbss;
extern uint32_t _ebss;

// Prototype of main()
int main(void);

/* function prototypes of STM32F407x system exception and IRQ handlers */

void reset_isr(void);

void nmi_isr 					(void) __attribute__ ((weak, interrupt, alias("Default_Handler")));
void hardfault_isr 				(void) __attribute__ ((weak, interrupt, alias("Default_Handler")));
void memmanage_isr 				(void) __attribute__ ((weak, interrupt, alias("Default_Handler")));
void busfault_isr 				(void) __attribute__ ((weak, interrupt, alias("Default_Handler")));
void usagefault_isr 			(void) __attribute__ ((weak, interrupt, alias("Default_Handler")));
void svc_isr 					(void) __attribute__ ((weak, interrupt, alias("Default_Handler")));
void debugmon_isr 				(void) __attribute__ ((weak, interrupt, alias("Default_Handler")));
void pendsv_isr   				(void) __attribute__ ((weak, interrupt, alias("Default_Handler")));
void systick_isr  				(void) __attribute__ ((weak, interrupt, alias("Default_Handler")));
void wwdg_isr 				    (void) __attribute__ ((weak, interrupt, alias("Default_Handler")));
void pvd_isr 				    (void) __attribute__ ((weak, interrupt, alias("Default_Handler")));             
void tamp_stamp_isr 		    (void) __attribute__ ((weak, interrupt, alias("Default_Handler")));      
void rtc_wkup_isr 			    (void) __attribute__ ((weak, interrupt, alias("Default_Handler"))); 
void flash_isr				    (void) __attribute__ ((weak, interrupt, alias("Default_Handler"))); 
void rcc_isr 				    (void) __attribute__ ((weak, interrupt, alias("Default_Handler")));             
void exti0_isr 				    (void) __attribute__ ((weak, interrupt, alias("Default_Handler")));           
void exti1_isr 				    (void) __attribute__ ((weak, interrupt, alias("Default_Handler")));           
void exti2_isr 				    (void) __attribute__ ((weak, interrupt, alias("Default_Handler")));           
void exti3_isr 				    (void) __attribute__ ((weak, interrupt, alias("Default_Handler")));           
void exti4_isr 				    (void) __attribute__ ((weak, interrupt, alias("Default_Handler")));           
void dma1_stream0_isr 		    (void) __attribute__ ((weak, interrupt, alias("Default_Handler")));    
void dma1_stream1_isr 		    (void) __attribute__ ((weak, interrupt, alias("Default_Handler")));    
void dma1_stream2_isr 		    (void) __attribute__ ((weak, interrupt, alias("Default_Handler")));    
void dma1_stream3_isr 		    (void) __attribute__ ((weak, interrupt, alias("Default_Handler")));    
void dma1_stream4_isr 		    (void) __attribute__ ((weak, interrupt, alias("Default_Handler")));    
void dma1_stream5_isr 		    (void) __attribute__ ((weak, interrupt, alias("Default_Handler")));    
void dma1_stream6_isr 		    (void) __attribute__ ((weak, interrupt, alias("Default_Handler")));    
void adc_isr 				    (void) __attribute__ ((weak, interrupt, alias("Default_Handler")));             
void can1_tx_isr 			    (void) __attribute__ ((weak, interrupt, alias("Default_Handler")));         
void can1_rx0_isr 			    (void) __attribute__ ((weak, interrupt, alias("Default_Handler")));        
void can1_rx1_isr 			    (void) __attribute__ ((weak, interrupt, alias("Default_Handler")));        
void can1_sce_isr 			    (void) __attribute__ ((weak, interrupt, alias("Default_Handler")));        
void exti9_5_isr 			    (void) __attribute__ ((weak, interrupt, alias("Default_Handler")));         
void tim1_brk_tim9_isr 		    (void) __attribute__ ((weak, interrupt, alias("Default_Handler")));   
void tim1_up_tim10_isr 		    (void) __attribute__ ((weak, interrupt, alias("Default_Handler")));   
void tim1_trg_com_tim11_isr     (void) __attribute__ ((weak, interrupt, alias("Default_Handler")));
void tim1_cc_isr 			    (void) __attribute__ ((weak, interrupt, alias("Default_Handler")));         
void tim2_isr 				    (void) __attribute__ ((weak, interrupt, alias("Default_Handler")));            
void tim3_isr 				    (void) __attribute__ ((weak, interrupt, alias("Default_Handler")));            
void tim4_isr 				    (void) __attribute__ ((weak, interrupt, alias("Default_Handler")));            
void i2c1_ev_isr 			    (void) __attribute__ ((weak, interrupt, alias("Default_Handler")));         
void i2c1_er_isr 			    (void) __attribute__ ((weak, interrupt, alias("Default_Handler")));         
void i2c2_ev_isr 			    (void) __attribute__ ((weak, interrupt, alias("Default_Handler")));         
void i2c2_er_isr 			    (void) __attribute__ ((weak, interrupt, alias("Default_Handler")));         
void spi1_isr  				    (void) __attribute__ ((weak, interrupt, alias("Default_Handler")));           
void spi2_isr 				    (void) __attribute__ ((weak, interrupt, alias("Default_Handler")));            
void usart1_isr  			    (void) __attribute__ ((weak, interrupt, alias("Default_Handler")));         
void usart2_isr  			    (void) __attribute__ ((weak, interrupt, alias("Default_Handler")));        
void usart3_isr   			    (void) __attribute__ ((weak, interrupt, alias("Default_Handler")));        
void exti15_10_isr   		    (void) __attribute__ ((weak, interrupt, alias("Default_Handler")));     
void rtc_alarm_isr    		    (void) __attribute__ ((weak, interrupt, alias("Default_Handler")));    
void otg_fs_wkup_isr     	    (void) __attribute__ ((weak, interrupt, alias("Default_Handler"))); 
void tim8_brk_tim12_isr   	    (void) __attribute__ ((weak, interrupt, alias("Default_Handler")));
void tim8_up_tim13_isr    	    (void) __attribute__ ((weak, interrupt, alias("Default_Handler")));
void tim8_trg_com_tim14_isr     (void) __attribute__ ((weak, interrupt, alias("Default_Handler")));
void tim8_cc_isr          	    (void) __attribute__ ((weak, interrupt, alias("Default_Handler")));
void dma1_stream7_isr     	    (void) __attribute__ ((weak, interrupt, alias("Default_Handler")));
void fsmc_isr             	    (void) __attribute__ ((weak, interrupt, alias("Default_Handler")));
void sdio_isr             	    (void) __attribute__ ((weak, interrupt, alias("Default_Handler")));
void tim5_isr             	    (void) __attribute__ ((weak, interrupt, alias("Default_Handler")));
void spi3_isr             	    (void) __attribute__ ((weak, interrupt, alias("Default_Handler")));
void uart4_isr            	    (void) __attribute__ ((weak, interrupt, alias("Default_Handler")));
void uart5_isr            	    (void) __attribute__ ((weak, interrupt, alias("Default_Handler")));
void tim6_dac_isr         	    (void) __attribute__ ((weak, interrupt, alias("Default_Handler")));
void tim7_isr             	    (void) __attribute__ ((weak, interrupt, alias("Default_Handler")));
void dma2_stream0_isr     	    (void) __attribute__ ((weak, interrupt, alias("Default_Handler")));
void dma2_stream1_isr     	    (void) __attribute__ ((weak, interrupt, alias("Default_Handler")));
void dma2_stream2_isr     	    (void) __attribute__ ((weak, interrupt, alias("Default_Handler")));
void dma2_stream3_isr     	    (void) __attribute__ ((weak, interrupt, alias("Default_Handler")));
void dma2_stream4_isr     	    (void) __attribute__ ((weak, interrupt, alias("Default_Handler")));
void eth_isr              	    (void) __attribute__ ((weak, interrupt, alias("Default_Handler")));
void eth_wkup_isr         	    (void) __attribute__ ((weak, interrupt, alias("Default_Handler")));
void can2_tx_isr          	    (void) __attribute__ ((weak, interrupt, alias("Default_Handler")));
void can2_rx0_isr         	    (void) __attribute__ ((weak, interrupt, alias("Default_Handler")));
void can2_rx1_isr         	    (void) __attribute__ ((weak, interrupt, alias("Default_Handler")));
void can2_sce_isr         	    (void) __attribute__ ((weak, interrupt, alias("Default_Handler")));
void otg_fs_isr           	    (void) __attribute__ ((weak, interrupt, alias("Default_Handler")));
void dma2_stream5_isr     	    (void) __attribute__ ((weak, interrupt, alias("Default_Handler")));
void dma2_stream6_isr     	    (void) __attribute__ ((weak, interrupt, alias("Default_Handler")));
void dma2_stream7_isr     	    (void) __attribute__ ((weak, interrupt, alias("Default_Handler")));
void usart6_isr           	    (void) __attribute__ ((weak, interrupt, alias("Default_Handler")));
void i2c3_ev_isr          	    (void) __attribute__ ((weak, interrupt, alias("Default_Handler")));
void i2c3_er_isr          	    (void) __attribute__ ((weak, interrupt, alias("Default_Handler")));
void otg_hs_ep1_out_isr   	    (void) __attribute__ ((weak, interrupt, alias("Default_Handler")));
void otg_hs_ep1_in_isr    	    (void) __attribute__ ((weak, interrupt, alias("Default_Handler")));
void otg_hs_wkup_isr      	    (void) __attribute__ ((weak, interrupt, alias("Default_Handler")));
void otg_hs_isr           	    (void) __attribute__ ((weak, interrupt, alias("Default_Handler")));
void dcmi_isr             	    (void) __attribute__ ((weak, interrupt, alias("Default_Handler")));
void cryp_isr             	    (void) __attribute__ ((weak, interrupt, alias("Default_Handler")));
void hash_rng_isr         	    (void) __attribute__ ((weak, interrupt, alias("Default_Handler")));
void fpu_isr              	    (void) __attribute__ ((weak, interrupt, alias("Default_Handler"))); 

uint32_t vectors[] __attribute__ ((section (".isr_vector"))) = {
	STACK_START,
	(uint32_t) &reset_isr,
	(uint32_t) &hardfault_isr,
	(uint32_t) &nmi_isr,
	(uint32_t) &memmanage_isr,
	(uint32_t) &busfault_isr,
	(uint32_t) &usagefault_isr,
	0,
	0,
	0,
	0,
	(uint32_t) &svc_isr,
	(uint32_t) &debugmon_isr,
	0,
	(uint32_t) &pendsv_isr,
	(uint32_t) &systick_isr,
	(uint32_t) &wwdg_isr ,
	(uint32_t) &pvd_isr,
	(uint32_t) &tamp_stamp_isr,
	(uint32_t) &rtc_wkup_isr,
	(uint32_t) &flash_isr,
	(uint32_t) &rcc_isr,
	(uint32_t) &exti0_isr,
	(uint32_t) &exti1_isr,
	(uint32_t) &exti2_isr,
	(uint32_t) &exti3_isr,
	(uint32_t) &exti4_isr,
	(uint32_t) &dma1_stream0_isr,
	(uint32_t) &dma1_stream1_isr,
	(uint32_t) &dma1_stream2_isr,
	(uint32_t) &dma1_stream3_isr,
	(uint32_t) &dma1_stream4_isr,
	(uint32_t) &dma1_stream5_isr,
	(uint32_t) &dma1_stream6_isr,
	(uint32_t) &adc_isr,
	(uint32_t) &can1_tx_isr,
	(uint32_t) &can1_rx0_isr,
	(uint32_t) &can1_rx1_isr,
	(uint32_t) &can1_sce_isr,
	(uint32_t) &exti9_5_isr,
	(uint32_t) &tim1_brk_tim9_isr,
	(uint32_t) &tim1_up_tim10_isr,
	(uint32_t) &tim1_trg_com_tim11_isr,
	(uint32_t) &tim1_cc_isr,
	(uint32_t) &tim2_isr,
	(uint32_t) &tim3_isr,
	(uint32_t) &tim4_isr,
	(uint32_t) &i2c1_ev_isr,
	(uint32_t) &i2c1_er_isr,
	(uint32_t) &i2c2_ev_isr,
	(uint32_t) &i2c2_er_isr,
	(uint32_t) &spi1_isr,
	(uint32_t) &spi2_isr,
	(uint32_t) &usart1_isr,
	(uint32_t) &usart2_isr,
	(uint32_t) &usart3_isr,
	(uint32_t) &exti15_10_isr,
	(uint32_t) &rtc_alarm_isr,
	(uint32_t) &otg_fs_wkup_isr,
	(uint32_t) &tim8_brk_tim12_isr,
	(uint32_t) &tim8_up_tim13_isr,
	(uint32_t) &tim8_trg_com_tim14_isr,
	(uint32_t) &tim8_cc_isr,
	(uint32_t) &dma1_stream7_isr,
	(uint32_t) &fsmc_isr,
	(uint32_t) &sdio_isr,
	(uint32_t) &tim5_isr,
	(uint32_t) &spi3_isr,
	(uint32_t) &uart4_isr,
	(uint32_t) &uart5_isr,
	(uint32_t) &tim6_dac_isr,
	(uint32_t) &tim7_isr,
	(uint32_t) &dma2_stream0_isr,
	(uint32_t) &dma2_stream1_isr,
	(uint32_t) &dma2_stream2_isr,
	(uint32_t) &dma2_stream3_isr,
	(uint32_t) &dma2_stream4_isr,
	(uint32_t) &eth_isr,
	(uint32_t) &eth_wkup_isr,
	(uint32_t) &can2_tx_isr,
	(uint32_t) &can2_rx0_isr,
	(uint32_t) &can2_rx1_isr,
	(uint32_t) &can2_sce_isr,
	(uint32_t) &otg_fs_isr,
	(uint32_t) &dma2_stream5_isr,
	(uint32_t) &dma2_stream6_isr,
	(uint32_t) &dma2_stream7_isr,
	(uint32_t) &usart6_isr,
	(uint32_t) &i2c3_ev_isr,
	(uint32_t) &i2c3_er_isr,
	(uint32_t) &otg_hs_ep1_out_isr,
	(uint32_t) &otg_hs_ep1_in_isr,
	(uint32_t) &otg_hs_wkup_isr,
	(uint32_t) &otg_hs_isr,
	(uint32_t) &dcmi_isr,
	(uint32_t) &cryp_isr,
	(uint32_t) &hash_rng_isr,
	(uint32_t) &fpu_isr,
	
	
};

void Default_Handler(void) {
    for(;;);
}

void reset_isr(void) {
	// Copy .data section to SRAM
	uint32_t size = (uint32_t) &_edata - (uint32_t)&_sdata;
	
	uint8_t *p_dst = (uint8_t*) &_sdata; // SRAM
	uint8_t *p_src = (uint8_t*) &_la_data; // Flash
	for (uint32_t pos = 0; pos < size; pos++) 
	{
		*p_dst++ = *p_src++;
	}
	
	// Init the .bss section to zero in SRAM
	size = (uint32_t)&_ebss - (uint32_t)&_sbss;
	p_dst = (uint8_t*) &_sbss;
	for (uint32_t pos = 0; pos < size; pos++) 
	{
		*p_dst++ = 0;
	}
	
	main();
}

// *End of file*

