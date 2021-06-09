// Permission to copy is granted provided that this header remains intact. 
// This software is provided with no warranties.

////////////////////////////////////////////////////////////////////////////////

#ifndef SCHEDULER_H
#define SCHEDULER_H

//Shared Variable
unsigned char led0_output = 0x00;
unsigned char led1_output = 0x00;
unsigned char pause = 0;

enum PauseButtonSM_States { pauseButton_wait, pauseButton_release, pauseButton_press };
int pauseButtonToggleSMTick(int state) {
	unsigned char press = ~PINA & 0x01;
	switch(state) {
	   case(pauseButton_wait):
	  	state = press == 0x01 ? pauseButton_press : pauseButton_wait;
		break;
	   case(pauseButton_release):
		state = press == 0x00 ? pauseButton_wait : pauseButton_press;
		break;
	   case(pauseButton_press):
		state = pauseButton_release;
		break;
	   default:
		break;
	}

	switch(state) {
	   case(pauseButton_wait):
		break;
	   case(pauseButton_release):
		break;
	   case(pauseButton_press):
		pause = (pause == 0) ? 1:0;
		break;
	   default:
		break;
	}
	return state;
} 

enum toggleLED0_States { toggleLED0_wait, toggleLED0_blink };
int toggleLED0SMTick(int state) {
	switch(state) {
	   case(toggleLED0_wait):
		state = !pause ? toggleLED0_blink : toggleLED0_wait;
		break;
	   case(toggleLED0_blink):
		state = pause ? toggleLED0_wait : toggleLED0_blink;
		break;
	   default:
		break;
	}

	switch(state) {
	   case(toggleLED0_wait):
		break;
	   case(toggleLED0_blink):
		led0_output = (led0_output == 00) ? 0x01 : 0x00;
		break;
	   default:
		break;
	}
	return state;
}

enum toggleLED1_States { toggleLED1_wait, toggleLED1_blink };
int toggleLED1SMTick(int state) {
	switch(state) {
	   case(toggleLED1_wait):
		state = !pause ? toggleLED1_blink : toggleLED1_wait;
		break;
	   case(toggleLED1_blink):
		state = pause ? toggleLED1_wait : toggleLED1_blink;
		break;
	   default:
		break;
	}

	switch(state) {
	   case(toggleLED1_wait):
		break;
	   case(toggleLED1_blink):
		led1_output = (led1_output == 00) ? 0x01 : 0x00;
		break;
	   default:
		break;
	}
	return state;
}

enum display_States {display_display};
int displaySMTick(int state) {
	unsigned char output;
	switch(state) {
	   case(display_display):
		state = display_display;
		break;
	   default:
		state = display_display;
		break;
	}

	switch(state) {
	   case(display_display):
		output = led0_output | led1_output << 1;
		break;
	   default:
		break;
	}
	PORTB = output;
	return state;
}

////////////////////////////////////////////////////////////////////////////////
//Functionality - finds the greatest common divisor of two values
//Parameter: Two long int's to find their GCD
//Returns: GCD else 0
unsigned long int findGCD(unsigned long int a, unsigned long int b)
{
	unsigned long int c;
	while(1){
		c = a % b;
		if( c == 0 ) { return b; }
		a = b;
		b = c;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////////
//Struct for Tasks represent a running process in our simple real-time operating system
typedef struct _task{
	// Tasks should have members that include: state, period,
	//a measurement of elapsed time, and a function pointer.
	signed 	 char state; 		//Task's current state
	unsigned long period; 		//Task period
	unsigned long elapsedTime; 	//Time elapsed since last task tick
	int (*TickFct)(int); 		//Task tick function
} task;

#endif //SCHEDULER_H
