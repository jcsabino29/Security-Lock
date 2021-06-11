/*	Author: Jhaymar Sabino
 *      Demo Link: https://drive.google.com/file/d/1Gm1g2Y7xnYnTuK6v4kVCWoYnokp6De6U/view?usp=sharing
 *	Exercise Description: Extra addition function,
If '#' is pressed then PB3 will light up and the button is ready for the combination, else nothing happens.
If the right combination is being inputted PB2 will light up as long as it's pressed then goes back to PB3

When inputting a new combination PB0 and PB1 lights up when a button on the keypad is being pressed, it takes in 4 inputs and replaces the old password.
 *
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "keypad.h"
#include "scheduler.h"
#endif
#define C4 2616.13
#define D4 2939.66
#define E4 3290.63
#define F4 3490.23
#define G4 3920.00
#define A4 4400.00
#define B4 4930.88
#define C5 5230.25

volatile unsigned char TimerFlag = 0;

unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

void TimerOn() {
	TCCR1B = 0x0B;
	OCR1A = 125;
	TIMSK1 = 0x02;
	TCNT1 = 0;
	
	_avr_timer_cntcurr = _avr_timer_M;
	SREG |= 0x80;
}

void TimerOff() {
	TCCR1B = 0x00;
}

void TimerISR() {
	TimerFlag = 1;
}

ISR(TIMER1_COMPA_vect) {
	_avr_timer_cntcurr--;
	if (_avr_timer_cntcurr == 0) {
		TimerISR();
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

void TimerSet(unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}

void set_PWM(double frequency) {
	static double current_frequency;
	if (frequency != current_frequency) {
		if (!frequency) { TCCR3B &= 0x08;
		} else { TCCR3B |= 0x03; }

		if (frequency < 0.954) { OCR3A = 0xFFFF;
		} else if (frequency > 31250) { OCR3A = 0x000;
		} else { OCR3A = (short) (80000000 / (128 * frequency)) - 1; } 
		
		TCNT3 = 0;
		current_frequency = frequency;
	}
}

void PWM_on() {
	TCCR3A = (1 << COM3A0);
	TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30);
	set_PWM(0);
}

void PWM_off() {
	TCCR3A = 0x00;
	TCCR3B = 0x00;
}

static unsigned newCombo = 0;
static unsigned char x = 0;
unsigned char password[5] = {'1', '2', '3', '4', '5'};
static int i = 0;
enum keyPadSM_State { SM_Start, SM_Lock, SM_Unlock };
int keyPadSMTick(int state) {
	static unsigned char press = 0;
	static unsigned char sz = 0;
	switch(state) {
	   case(SM_Start):
		if (newCombo) {
		  sz = 4;
		} else { sz = 5; } 		

		if ((GetKeypadKey() == '#') && (~PINB & 0x80) != 0x80) {
		   while(GetKeypadKey() == '#');
		   state = SM_Unlock;
		} else if ((~PINB & 0x80) == 0x80) {
		   state = SM_Lock;
		} else { }
		//PORTB = 0x00;
 		break;
	   default:
		break;
	}

	switch(state) {
	   case(SM_Lock):
		PORTB = 0x00;
		state = SM_Start;
 		break;
	   case(SM_Unlock):
		PORTB = 0x08;
		//for (int i = 0; i < 5; i++) { //Increments i
		//   while(!GetKeypadKey()); //Wait for input
		   x = GetKeypadKey();
		   if (x) {
			press = 1;
			//PORTB = 0x00;
		   } else { press = 0; }
		   while(GetKeypadKey()) { //While button is being held
		   	PORTB = 0x00;
		   }

		   //It has gone through the entire array == unlock
		   if (i == sz) {
			PORTB = 0x01; //Unlock
			i = 0;
			state = SM_Start;
		   } else { }		

		   //Only run if button has been pressed
		   if (press) { 
	  		   if(password[i] == x) {
				PORTB = 0x04;
				i = i + 1;
		  	   } else {
			   	//PORTB = 0x01;
			   	i = 0;	//resets counter
				state = SM_Start;
			   }
		   } else { }
		break;
	   default:
		break;
	}
	/*
	switch(x) {
	   case('\0'):
		PORTB = 0x1F;		
	       	break;
	   case('1'):
		PORTB = 0x01;
	      	break;
	   case('2'):
		PORTB = 0x02;
	      	break;
	   case('3'):
		PORTB = 0x03;
	      	break;
	   case('4'):
		PORTB = 0x04;
	      	break;
	   case('5'):
		PORTB = 0x05;
	      	break;
	   case('6'):
		PORTB = 0x06;
	      	break;
	   case('7'):
		PORTB = 0x07;
	      	break;
	   case('8'):
		PORTB = 0x08;
	      	break;
	   case('9'):
		PORTB = 0x09;
	      	break;
	   case('#'):
		PORTB = 0x0F;
	      	break;
	   default:
		PORTB = 0x00;
		break;
	} */
	return state;
}

static unsigned char j = 0;
static unsigned char counter = 0;
static double frequency = 0;
static unsigned char enterCombo = 0;
double melody [15] = {F4, F4, C4, C4, D4, D4, D4, D4, C4, D4, E4, F4, C5, B4, F4 };
enum doorbellSM_States { DB_Start, DB_Play, DB_ChLock };
int doorbellSMTick (int state) {
	/*PORTB = 0x0F;
		if ((~PINA & 0x80) == 0x80) {
		    PORTB = 0x00;
		    state = DB_Play;
		    //playing = 1;
		} else { }*/
	switch(state) {
	   case(DB_Start):
		//PORTB = 0x04;
		if ((~PINA & 0x80) == 0x80) {
		    PORTB = 0x08;
		    state = DB_Play;
		    //playing = 1;
		} else if ((GetKeypadKey() == '#') && (~PINB & 0x80) == 0x80) {
		    while((GetKeypadKey() == '#') || (~PINB & 0x80) == 0x80);
	 	    state = DB_ChLock;
		} else { }
		break;
	   default:
		//PORTB = 0x01;
		//state = DB_Start;
		break;
	}

	switch(state) {
	   case(DB_Play):
		//while(playing) {
		if (counter < 15) {
		   frequency = melody[counter];
		   set_PWM(frequency);
		   PORTB = 0x02;
		   counter = counter + 1;
		} else {
		   set_PWM(0);
		   PORTB = 0x00;
		   state = DB_Start;
		   counter = 0;
		}
		break;
	   case(DB_ChLock):
		x = GetKeypadKey();
		if (x) {
		   enterCombo = 1;
		} else { enterCombo = 0; }

		if (j >= 4) { j = 0; PORTB = 0x0A; state = DB_Start; newCombo = 1; }
		else { }
		if (enterCombo == 1) {
			if (j < 4) {
			   while(GetKeypadKey()) {
			     password[j] = x;
			     PORTB = 0x03;
			   }
			   j = j + 1;
		  	} else { j = 0; PORTB = 0x0A; state = DB_Start; newCombo = 1; }
		} else { PORTB = 0x00; } //T
		break;
	   default:
		//state = DB_Start;
		break;
	}
	return state;
}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRB = 0x4F; PORTB = 0xB0;
	DDRC = 0xF0; PORTC = 0x0F;
	DDRA = 0x00; PORTA = 0xFF;
	//SM Initializations

	//Declare an array of tasks
	static task task1, task2, task3, task4, task5, task6;
	task *tasks[] = { &task1, &task2, &task3, &task4, &task5, &task6};
	//task *tasks[] = {&task1};
	const unsigned char numTasks = sizeof(tasks)/sizeof(task*);

	const char start = 0;
	//Task 1 pauseButtonToggleSM
	task1.state = start;
	task1.period = 200;
	task1.elapsedTime = task1.period;
	task1.TickFct = &pauseButtonToggleSMTick;

	//Task 2 toggleLED0SMTick
	task2.state = start;
	task2.period = 600;
	task2.elapsedTime = task2.period;
	task2.TickFct = &toggleLED0SMTick;

	//Task 3 toggleLED1SM1Tick
	task3.state = start;
	task3.period = 1000;
	task3.elapsedTime = task3.period;
	task3.TickFct = &toggleLED1SMTick;

	//Task 4 displaySMTick
	task4.state = start;
	task4.period = 50;
	task4.elapsedTime = task3.period;
	task4.TickFct = &displaySMTick;

	//Task 5 keyPadSMTick
	task5.state = start;
	task5.period = 200;
	task5.elapsedTime = task5.period;
	task5.TickFct = &keyPadSMTick;

	//Task 6 doorbellSMTick
	task6.state = start;
	task6.period = 200;
	task6.elapsedTime = task6.period;
	task6.TickFct = &doorbellSMTick;

	//Variable Initialization
	unsigned long GCD = tasks[0]->period;
	//unsigned char x = 0;

	//Timer Initialization
	TimerSet(GCD);
	TimerOn();
	PWM_on();

    /* Insert your solution below */
    while (1) {
	//TimerSet(10);
	for ( int i = 0; i < numTasks; i++ ) {
		if ( tasks[i]->elapsedTime == tasks[i]->period ) {
			tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
			tasks[i]->elapsedTime = 0;
		}
		GCD = findGCD(GCD, tasks[i]->period);
		tasks[i]->elapsedTime += GCD;
	}

	/*if ((GetKeypadKey() == '#') && (~PINB & 0x80) != 0x80) {
	   /*while(GetKeypadKey() == '#') {
	   //state = SM_Unlock;
		   if (~(~PINA & 0x01)) {
			PORTB = 0x02;
		   } else if (~PINA & 0x01) {
			PORTB = 0x04;
		   } else { PORTB = 0x08; }
	   }
	   PORTB = 0x02;
	} else if ((GetKeypadKey() == '#') && (~PINB & 0x80) == 0x80) {
	   //state = SM_Lock;
	   PORTB = 0x01;
	} else { PORTB = 0x06; }*/
	while(!TimerFlag);
	TimerFlag = 0;
    }
    return 1;
}
