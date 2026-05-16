#include <stdint.h>
#include <stdbool.h>
#include "stm32f10x.h"

bool checkSwitch(void);

void delay(uint32_t delay); // A delay loop which blocks while a value is decremented to 0
void clockInit(void); // Clock initialization providing a default SYSCLK of 24 MHz using the PLL and clock visibility of PLL/2 on PA8
void GPIOInit (void); // Configuration for all ports (more specific info in the comments of the function itself)

void adcInit(void); // initialize internal ADC
uint32_t adcRead(uint32_t channel); // read the analog value at the given channel and convert to digital then return
uint32_t lineRead(void); // read the digital value at the predefined GPIO port

void tim3Init(void); // initalize and configure timer 3 and all 4 of its channels as PWMs
void tim3ch1Update(int counts); // update duty cycle of channel 1 (top left wheel)
void tim3ch2Update(int counts); // update duty cycle of channel 2 (top right wheel)
void tim3ch3Update(int counts); // update duty cycle of channel 3 (bot left wheel)
void tim3ch4Update(int counts); // update duty cycle of channel 4 (bot right wheel)
void tim3ch1UpdateEGR(int counts); // same as above just updating using a slightly different method
void tim3ch2UpdateEGR(int counts);
void tim3ch3UpdateEGR(int counts);
void tim3ch4UpdateEGR(int counts);

void turnLeft(void); // turn the robot left by locking the left wheels and increasing the speed of the right wheels
void turnRight(void); // turn the robot right by locking the right wheels and increasing the speed of the left wheels
void goStraight(void); // keep straight at a decent pace
