#ifndef SM_H
#include <msp.h>
#include <stdint.h>
#include <stdlib.h>
#include "LED_SM_RUN.h"
#define E_MAX 2 // # of events for state table
#define S_MAX 4 // # of states for state table


typedef enum{
	NEXT_EVENT,
	PREV_EVENT,
} E_ENUM;


typedef enum{
	S0,
	S1, 
	S2,
	S3,
} S_ENUM;

//create a 
struct SM { 
	S_ENUM current;
};


struct SM *init_State_Machine(struct SM *PSM);


void trgState(E_ENUM event, struct SM *PSM);


void rstState(S_ENUM state,struct SM *PSM);

#endif
