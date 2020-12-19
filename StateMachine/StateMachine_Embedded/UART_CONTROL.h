#ifndef UART_CONTROL_H
#define UART_CONTROL_H 
#include <msp.h>
#include <stdlib.h>
#include <string.h>
#define UART0R_pin1_2 (1<<2) 
#define UART0T_pin1_3 (1<<3)  

void init_UART_pins(void);
void init_UART(void);
void init_UART_interrupts(void);
void transmitor(char transmited);
#endif
