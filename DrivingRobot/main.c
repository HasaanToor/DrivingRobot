/* 
Muhammad Hasaan Toor
200449166
Feb. 13, 2023
*/

#include "util.h"


int main(){
	
	clockInit();
	GPIOInit();
	adcInit();
	tim3Init();
	
	// these output a logic high and low because in order to spin the motors one terminal needs to be high and the other needs to be low (depends on rotation direction)
	GPIOA->ODR |= GPIO_ODR_ODR1; // logic 1
	GPIOA->ODR &= ~GPIO_ODR_ODR4; // logic 0
	
	uint32_t PA0 = 0x00000000; // ADC input will be at PA0
	uint32_t adcData = 0x0;
	uint32_t lineData = 0x0;
	bool wallMode = true;
	
	while(1)
	{
		while(wallMode)
		{
			adcData = adcRead(PA0); // digital values are from 000 to FFF, with our bit resolution being 0.805 mV
		
			if (adcData < 0x3E5 || !adcData) // 3E5 * 0.805 mV = 0.80 V which is roughly 7 cm, so when the robot is farther than 7cm away from the wall, turn left
				turnLeft();
			else if (adcData >= 0x3E5 && adcData < 0x5DD) // when between 7 cm and 4.5 cm, keep straight
				goStraight();
			else if (adcData >= 0x5DD) // 5DD * 0.805 mV = 1.20V = 4.5 cm, when closer than 4.5 cm, turn right
				turnRight();
			
			if (!checkSwitch()) // toggle to line avoidance mode
				wallMode = false;
		}
		
		while(!wallMode)
		{
			lineData = lineRead();
		
			if (lineData != 0) // if black surface detected, turn right
				turnRight();
			else // if on white surface, keep straight
				goStraight();
			
			if (!checkSwitch()) // toggle to wall following mode
				wallMode = true;
		}
	}
	
	return 0;
}
