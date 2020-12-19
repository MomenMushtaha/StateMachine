#include <msp.h>
#include <stdint.h>
#include "LED_SM_RUN.h"
#include "SM.h"
#include "UART_CONTROL.h"



// State machine(SM)
struct SM PSM;

/*
Initialize the swithces, set up as pull up input, Initialize the rgb led (output),Initialize the red led (output)
*/
void init_switch(void){
	// Set buttons to GPIO mode
	P1SEL0 &= ~(SWITCH_NEXT | SWITCH_PREV);
	P1SEL1 &= ~(SWITCH_NEXT | SWITCH_PREV);
	// Set Buttons to input
	P1DIR &= ~(SWITCH_NEXT | SWITCH_PREV);  // Set Port 1 inputs
	// to avoid floating input, pull up is used
	P1OUT |= (SWITCH_NEXT | SWITCH_PREV);
	P1REN |= (SWITCH_NEXT | SWITCH_PREV);
	
}
void init_led_red(void){
	P1SEL0 &= ~(LED_RED); // set as GPIO
	P1SEL1 &= ~(LED_RED);
	P1DIR |= (LED_RED);
}
void init_led_rgb(void){
	P2SEL0 &= ~(LED_RGB_RED | LED_RGB_GREEN | LED_RGB_BLUE); //(R,G,B) set as GPIO 
	P2SEL0 &= ~(LED_RGB_RED | LED_RGB_GREEN | LED_RGB_BLUE);
	P2DIR |= (LED_RGB_RED | LED_RGB_GREEN | LED_RGB_BLUE); // output
}

/*
Set the color of red LED, Set the color of the rgb led
*/
void set_led_red(bool SWITCH){
	if (SWITCH){//conditions
		P1OUT |= LED_RED;
	}else{
		P1OUT &= ~LED_RED;
	}
}

void set_led_rgb(uint8_t colorConfig){
	P2OUT = (uint8_t)0;
	if (colorConfig == 0){
		P2OUT = (uint8_t)0;
	}else if (colorConfig == 1){ // set colour to red
		P2OUT |= LED_RGB_RED;
	}else if (colorConfig == 2){ // set colour to green
		P2OUT |= LED_RGB_GREEN;
	}else if (colorConfig == 3){ // set colour to blue
		P2OUT |= LED_RGB_BLUE;
	}else{ // if incorrect turn off
		P2OUT = (uint8_t)0;
	}
}

/*
Enable interrupts for the switches
*/
void init_switch_interrupts(void){
	P1IES |= (SWITCH_NEXT | SWITCH_PREV); // configure falling edge event
	P1IFG &= ~(SWITCH_NEXT | SWITCH_PREV); // Clear intrrupt flags
	P1IE |= (SWITCH_NEXT | SWITCH_PREV); // Enable interrupts for switches 
	NVIC_SetPriority(PORT1_IRQn, 2); // Set priority of port 1 interrupts
	NVIC_ClearPendingIRQ(PORT1_IRQn); //  Clear pending interrupts for port1
	NVIC_EnableIRQ(PORT1_IRQn); // Enable interrupts for port 1 in nvic
}


/*
ISR for buttons, UART,
*/
void PORT1_IRQHandler(void){
	if(P1IFG & SWITCH_NEXT){ // if next state button is clicked
		P1IFG &= ~SWITCH_NEXT; // Clear the flag bit
		trgState(NEXT_EVENT,&PSM); // advance to next state in sm
	}else if(P1IFG & SWITCH_PREV){// if switch 4 is clicked
		P1IFG &= ~SWITCH_PREV; // Clear the flag bit
		trgState(PREV_EVENT, &PSM); // advance to next state in sm
	}
	transmitor(state_hex_vals[PSM.current]); // update state on PC side
}


void EUSCIA0_IRQHandler(void){
	if (EUSCI_A0->IFG & UCRXIFG){ // If a char fills receive buffer
		EUSCI_A0->IFG &= ~UCRXIFG;
		if (EUSCI_A0->RXBUF == UART_NEXT_STATE){
			trgState(NEXT_EVENT,&PSM);
		}else if (EUSCI_A0->RXBUF == UART_PREV_STATE){ // if prev state go back
			trgState(PREV_EVENT,&PSM);
		}
		transmitor(state_hex_vals[PSM.current]); // update state on PC side
	}
}



//main
	// INITIALIZE UART0, 115200 BAUD RATE, EVEN BIT PARITY,Initialize buttons and LED's, 
//initialize state machine,Enable all interrupts once initial setup is done
int main(){
	// Enable gloabl interrupts for ISR's
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;
	
	init_UART_pins();
	init_UART();
	init_switch();
	init_led_rgb();
	init_led_red();
	set_led_red(0);
	set_led_rgb(0);
	init_State_Machine(&PSM);
	
	init_UART_interrupts();
	init_switch_interrupts();
	__ASM("CPSIE I") ;
	while(1){
	}
	return 1;
}
