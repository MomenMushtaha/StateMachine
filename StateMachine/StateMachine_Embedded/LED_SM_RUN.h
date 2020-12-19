#ifndef LED_SM_RUN_H
#define LED_SM_RUN_H
#include <msp.h>
#include <stdbool.h>
#include <stdint.h>
#define RGB 256 // COLOUR FOR R G or B
// Define buttons for state transitions
#define SWITCH_NEXT (uint8_t)(1<<1)
#define SWITCH_PREV (uint8_t)(1<<4)
#define LED_RED (uint8_t)(1<<0) // p1.0
#define LED_RGB_RED (uint8_t)(1<<0) // p2.0
#define LED_RGB_GREEN (uint8_t)(1<<1) // p2.1
#define LED_RGB_BLUE (uint8_t)(1<<2) // p2.2
#define UART_NEXT_STATE 'n'// char next state
#define UART_PREV_STATE 'p' // char prev state
// Define state values to send over uart
static char state_hex_vals[] ={
	'0',
	'1',
	'2',
	'3'
};




void init_switch_interrupts(void);

void init_switch(void);

void init_led_rgb(void);

void init_led_red(void);

void set_led_red(bool SWITCH);

void set_led_rgb(uint8_t colorConfig); 

#endif
