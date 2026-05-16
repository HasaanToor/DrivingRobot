
 
 #include "util.h"
 

bool checkSwitch(void)
{
	volatile unsigned int checkIDR = (GPIOC->IDR & (uint32_t)0x00002000);
	if (checkIDR == (uint32_t)0x00002000)
		return true;
	else
		return false;
}

//**************************** Utility ************************************************************
// delay = 1800 is approximately 1 ms @ SYSCLK = 24 MHz (ymmv)

void delay(uint32_t delay)
{
  	 while (delay--)
	{
		}
}

//**************************** Clock Configuration ************************************************************
void clockInit(void)
{
  
//* enable HSI and wait for it to be ready

		RCC->CR |= RCC_CR_HSION;
    while (((RCC->CR) & (RCC_CR_HSION | RCC_CR_HSIRDY)) == 0);
			
//* enable HSE with Bypass and wait for it to be ready

		RCC->CR |= RCC_CR_HSEON | RCC_CR_HSEBYP;
    while (((RCC->CR) & (RCC_CR_HSEON | RCC_CR_HSEBYP | RCC_CR_HSERDY)) == 0);
				
    
////SET HSI as SYSCLK and wait for it to be recognized

//		RCC->CFGR = RCC_CFGR_SW_HSI;
//    while (((RCC->CFGR) & (RCC_CFGR_SW_HSI | RCC_CFGR_SWS_HSI)) != 0);
			
//SET HSE as SYSCLK and wait for it to be recognized

		RCC->CFGR = RCC_CFGR_SW_HSE;
    while (((RCC->CFGR) & (RCC_CFGR_SW_HSE | RCC_CFGR_SWS_HSE)) == 0);

//****************To Use PLL as SYSCLK	
// Disable PLL. 
		
		RCC->CR &= ~RCC_CR_PLLON;
			
//Change PLL source and set the PLL multiplier
			
//These are the SYSCLK values when using the PLL with HSI/2 as the input. The max value is 64 MHz
		//RCC->CFGR = 0x00000000;// 8MHz
		//RCC->CFGR = 0x00040000;// 12 MHz
		//RCC->CFGR = 0x00080000;// 16 MHz
		//RCC->CFGR = 0x000c0000;// 20 MHz			
		//RCC->CFGR = 0x00100000;// 24 MHz
		//RCC->CFGR = 0x00140000;//	28 MHz
		//RCC->CFGR = 0x00180000;// 32 MHz
		//RCC->CFGR = 0x001C0000;// 36 MHz			
		//RCC->CFGR = 0x00200000;// 40 MHz
		//RCC->CFGR = 0x00240000;//	44 MHz
		//RCC->CFGR = 0x00280000;// 48 MHz
		//RCC->CFGR = 0x002C0000;// 52 MHz
		//RCC->CFGR = 0x003C0000;// 64 MHz			

//These are the SYSCLK values when using the PLL with HSE/Bypass as the input. The max value is 72 MHz
		//RCC->CFGR = 0x00010000;// 16 MHz
		RCC->CFGR = 0x00050000;// 24 MHz 
		//RCC->CFGR = 0x00090000;// 32 MHz
		//RCC->CFGR = 0x000d0000;// 40 MHz			
		//RCC->CFGR = 0x00110000;// 48 MHz
		//RCC->CFGR = 0x00150000;//	56 MHz
		//RCC->CFGR = 0x00190000;// 64 MHz
		//RCC->CFGR = 0x001d0000;// 72 MHz		


//ENABLE PLL and wait for it to be ready

    RCC->CR |= RCC_CR_PLLON;
    while (((RCC->CR) & (RCC_CR_PLLON | RCC_CR_PLLRDY)) == 0);
   
// Set PLL as SYSCLK and wait for it to be ready
			
    RCC->CFGR |= RCC_CFGR_SW_PLL;// 0x00000002;
    while (((RCC->CFGR) & (RCC_CFGR_SW_PLL | RCC_CFGR_SWS_PLL)) == 0);

}

//**************************** I/O ************************************************************

void GPIOInit(void)
{
	//Enable Port A B C AFIO ADC1 and TIM3
    
	RCC->APB2ENR |=  RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPBEN | RCC_APB2ENR_IOPCEN | RCC_APB2ENR_ADC1EN | RCC_APB2ENR_AFIOEN;
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	
		
	//****************NUCLEO-64*******************************************************************
  // Set the config and mode bits for:
	// PA6, PA7, PB0, PB1 (PWM outputs -> CNF MODE = 1011 -> AF push pull output)
	// PA1, PA4 (Logic 1's or 0's -> CNF MODE = 0011 -> push pull output)
	// PA5 (Green LED -> CNF MODE = 0011 -> push pull output)
	// PA0 (Distance Sensor analog input ADC -> CNF MODE = 0000 -> analog input)
	// PB7 (Line Sensor digital input -> CNF MODE = 1000 -> input with pull up / pull down)
	
	GPIOA->CRL |= 0xBB330030;
	GPIOA->CRL &= ~0x44CC00CF;
	
	GPIOB->CRL |= 0x000000BB;
	GPIOB->CRL &= ~0xF0000044; // 0000 xxxx xxxx xxxx xxxx xxxx 1011 1011
	
	GPIOB->ODR |= GPIO_ODR_ODR7; // clearing floating inputs
	
	GPIOB->CRL |= 0x80000000; // 1000 xxxx xxxx xxxx xxxx xxxx 1011 1011
	
	
	//****************NUCLEO-64*******************************************************************
}	

void adcInit(void)
{
	ADC1->CR2 = 0x00000001;
}

uint32_t adcRead(uint32_t channel)
{
	ADC1->SQR3 = channel;
	ADC1->CR2 = 0x00000001;
	
	while ((ADC1->SR & ADC_SR_EOC) == 0x0)
	{
		;
	}
	
	return (ADC1->DR & ADC_DR_DATA);
}

uint32_t lineRead(void)
{
	return (GPIOB->IDR & GPIO_IDR_IDR7);
}

void tim3Init(void)
{
	TIM3->CR1 |= TIM_CR1_CEN; // clock enable
	TIM3->EGR |= TIM_EGR_UG; // re initalize
	TIM3->CCMR1 |= TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1; // channel 1 init
	TIM3->CCMR1 |= TIM_CCMR1_OC1PE | TIM_CCMR1_OC1FE;
	TIM3->CCMR1 |= TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1; // channel 2 init
	TIM3->CCMR1 |= TIM_CCMR1_OC2PE | TIM_CCMR1_OC2FE;
	TIM3->CCMR2 |= TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1; // channel 3 init
	TIM3->CCMR2 |= TIM_CCMR2_OC3PE | TIM_CCMR2_OC3FE;
	TIM3->CCMR2 |= TIM_CCMR2_OC4M_2 | TIM_CCMR2_OC4M_1; // channel 4 init
	TIM3->CCMR2 |= TIM_CCMR2_OC4PE | TIM_CCMR2_OC4FE;
	TIM3->CCER |= TIM_CCER_CC1E | TIM_CCER_CC2E | TIM_CCER_CC3E | TIM_CCER_CC4E; // channel enables
	TIM3->PSC = 0x095F; // 2399 -> 24 mhz / 2400 = 1 count = 0.1 ms
	TIM3->ARR = 100; // 100 counts = 10 ms
	TIM3->CCR1 = 80; // 80 counts = 8 ms -> duty cycle = 80%
	TIM3->CCR2 = 80;
	TIM3->CCR3 = 80;
	TIM3->CCR4 = 80;
	TIM3->CR1 |= TIM_CR1_ARPE | TIM_CR1_CEN;
}

void tim3ch1Update(int counts)
{
	TIM3->CCR1 = counts; // change duty cycle
	TIM3->CR1 |= TIM_CR1_ARPE | TIM_CR1_CEN;
}

void tim3ch2Update(int counts)
{
	TIM3->CCR2 = counts; // change duty cycle
	TIM3->CR1 |= TIM_CR1_ARPE | TIM_CR1_CEN;
}

void tim3ch3Update(int counts)
{
	TIM3->CCR3 = counts; // change duty cycle
	TIM3->CR1 |= TIM_CR1_ARPE | TIM_CR1_CEN;
}

void tim3ch4Update(int counts)
{
	TIM3->CCR4 = counts; // change duty cycle
	TIM3->CR1 |= TIM_CR1_ARPE | TIM_CR1_CEN;
}

void tim3ch1UpdateEGR(int counts)
{
	TIM3->CCR1 = counts; // change duty cycle
	TIM3->EGR |= TIM_EGR_UG; // re initialize
}

void tim3ch2UpdateEGR(int counts)
{
	TIM3->CCR2 = counts; // change duty cycle
	TIM3->EGR |= TIM_EGR_UG; // re initialize
}

void tim3ch3UpdateEGR(int counts)
{
	TIM3->CCR3 = counts; // change duty cycle
	TIM3->EGR |= TIM_EGR_UG; // re initialize
}

void tim3ch4UpdateEGR(int counts)
{
	TIM3->CCR4 = counts; // change duty cycle
	TIM3->EGR |= TIM_EGR_UG; // re initialize
}

void turnLeft(void)
{
	tim3ch1Update(0);
	tim3ch2Update(75);
	tim3ch3Update(0);
	tim3ch4UpdateEGR(100);
}

void turnRight(void)
{
	tim3ch1Update(100);
	tim3ch2Update(0);
	tim3ch3Update(100);
	tim3ch4Update(0);
}

void goStraight(void)
{
	tim3ch1Update(75);
	tim3ch2Update(75);
	tim3ch3Update(75);
	tim3ch4Update(75);
}
