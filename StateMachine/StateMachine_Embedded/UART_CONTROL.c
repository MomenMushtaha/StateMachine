#include "UART_CONTROL.h"


/*
Initialize Tx & Rx, 
Primary Module Function select in PXSEL0 and PXSEL1
*/
void init_UART_pins(void){
	P1SEL0 |= (uint8_t)(UART0R_pin1_2 | UART0T_pin1_3);
	P1SEL1 &= (uint8_t)~(UART0R_pin1_2 | UART0T_pin1_3); 
}


/*
Initialize UART, using even bit parity, SMCLK (3.0MHZ) source clock, 8 bit data mode
*/
void init_UART(void){
	EUSCI_A0->CTLW0 = 0x0000;
	// Enable bit parity
	// Enable software reset so baud rate can be changed.
	EUSCI_A0->CTLW0 |= (UCSSEL_2 | UCSWRST);
	// CHANGE BAUD RATE //
	EUSCI_A0->BRW = 0x0001;  // = INT(26/16)   N = 3MHz / 115200 = 26.041666 / 16 = 1.6276    BRW = 1
	EUSCI_A0->MCTLW |= UCOS16;  // Need to use over sampling 
	EUSCI_A0->MCTLW |= (0x00A0); 
	EUSCI_A0->MCTLW |= (0x0100); 
	EUSCI_A0->CTLW0 &= ~UCSWRST; // Disable software reset once baud rate set
}

/*
Enable interrupts for UART0, GLOBAL INTERRUPTS NOT ENABLED HERE!
*/
void init_UART_interrupts(void){
	// Enable interrupts on RX and TX
	EUSCI_A0->IE |= (UCRXIE);
	NVIC_SetPriority(EUSCIA0_IRQn, 3);
	NVIC_EnableIRQ( EUSCIA0_IRQn );
	NVIC_ClearPendingIRQ(EUSCIA0_IRQn);
}

/*
Function to send a string,  
it takes char array(null terminated string)
*/
void transmitor(char transmited){
	while (!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG)); // Wait fop register to become empty
	EUSCI_A0->IFG &= ~EUSCI_A_IFG_TXIFG; // Reset flag
	EUSCI_A0->TXBUF = transmited; // Load char to TXBUF for send
}
