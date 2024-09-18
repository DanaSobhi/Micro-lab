#define SYSCTL_RCGCGPIO_R (*((volatile unsigned long *)0x400FE608))
#define GPIO_PORTF_DATA_R (*((volatile unsigned long *)0x400253FC))
#define GPIO_PORTF_DIR_R (*((volatile unsigned long *)0x40025400))
#define GPIO_PORTF_DEN_R (*((volatile unsigned long *)0x4002551C))
#include "TM4C123GH6PM.h"
#include <stdio.h>
volatile unsigned int adc_value;

void ADC0SS3_Handler(void){
adc_value = ADC0->SSFIFO3; /* read adc coversion result from SS3 FIFO*/
ADC0->ISC = 8; /* clear coversion clear flag bit*/
ADC0->PSSI |= (1<<3); /* Enable SS3 conversion or start sampling data from AN0 */
}

int main ( void )
{
volatile unsigned long ulLoop ;	
volatile float voltage;

/* Enable Clock to ADC0 and GPIO pins*/
SYSCTL->RCGCGPIO |= (1<<4); /* Enable Clock to GPIOE or PE3/AN0 */
SYSCTL->RCGCADC |= (1<<0); /* AD0 clock enable*/
/* initialize PE3 for AIN0 input */
GPIOE->AFSEL |= (1<<3); /* enable alternate function */
GPIOE->DEN &= ~(1<<3); /* disable digital function */
GPIOE->AMSEL |= (1<<3); /* enable analog function */
/* initialize sample sequencer3 */
ADC0->ACTSS &= ~(1<<3); /* disable SS3 during configuration */
ADC0->EMUX &= ~0xF000; /* software trigger conversion */
ADC0->SSMUX3 = 0; /* get input from channel 0 */
ADC0->SSCTL3 |= (1<<1)|(1<<2); /* take one sample at a time, set flag at 1st sample */
ADC0->ACTSS |= (1<<3); /* enable ADC0 sequencer 3 */
// Enable the GPIO port that is used for the onboard LED.
SYSCTL_RCGCGPIO_R = 0x20;
// Do a dummy read to insert a few cycles after enabling the peripheral.
GPIO_PORTF_DIR_R = 0x1e;
GPIO_PORTF_DEN_R = 0x1e;
ulLoop = SYSCTL_RCGCGPIO_R;	
//_ Enable the GPIO pin for the LED (PF3). Set the direction as output and enable the GPIO pin for digital //function. _/

// Loop for ever.
// PF4 PF3 PF2 PF1 0x10 , 0x8 , 0x4 ,0x2
 
while(1)
{
ADC0->PSSI |= (1<<3); /* Enable SS3 conversion or start sampling data from AN0 */
while((ADC0->RIS & 8) == 0) ; /* Wait untill sample conversion completed*/
adc_value = ADC0->SSFIFO3; /* read adc coversion result from SS3 FIFO*/
ADC0->ISC = 8; /* clear coversion clear flag bit*/
if(adc_value >= 3095)
	GPIOF->DATA = 0x10; /* turn on green LED*/
else if(adc_value < 3095){
		if (adc_value > 2095){
		GPIOF->DATA = 0x00; /* turn off LEDs*/
		GPIOF->DATA = 0x08;	
		}
		else if(adc_value < 2095){
		if (adc_value > 1095){
		GPIOF->DATA = 0x00; /* turn off LEDs*/
		GPIOF->DATA = 0x04;	
		}
		else if(adc_value < 1095){

		GPIOF->DATA = 0x00; /* turn off LEDs*/
		GPIOF->DATA = 0x02;					
	}
GPIOF->DATA = 0x00;		
}

}
}
}