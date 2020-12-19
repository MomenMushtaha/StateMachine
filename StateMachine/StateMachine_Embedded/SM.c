#include "SM.h"

static S_ENUM States[S_MAX][E_MAX] = {
	    {S1, S3},	// state 0	
	           {S2, S0},	// state 1
	                  {S3, S1},	// state 2
	                         {S0, S2}	// state 3
};


void S0Func(){
	set_led_red(0);
	set_led_rgb(0);
}


void S1Func(){
	set_led_red(1);
	set_led_rgb(3);
}


void S2Func(){
	set_led_red(0);
	set_led_rgb(1);
}


void S3Func(){
	set_led_red(1);
	set_led_rgb(2);
}


typedef struct {
	void (*Func)(void);
} Pntr_To_Func;


/*
Array index by state_t, 
This returns state_function_t holding pointer to function
*/
static Pntr_To_Func Func_Array[] = {
	&S0Func,
	&S1Func,
	&S2Func,
	&S3Func
};

/*
Initilaize the state machine,  starting at state 0
*/
struct SM *init_State_Machine(struct SM *PSM){
	PSM->current = S0;
	Func_Array[PSM->current].Func(); // Call the function for state 0
	return PSM; 
}

/*
Changes the state
*/
void trgState(E_ENUM event,struct SM *PSM){
	PSM->current = States[PSM->current][event];
	Func_Array[PSM->current].Func();
}


/*
Go to reset state in the state machine
*/
void rstState( S_ENUM state,struct SM *PSM){
	PSM->current = state;
}

