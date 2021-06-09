/*	Author: lab
 *  Partner(s) Name: Jhaymar Sabino 
 *	Lab Section: 023
 *	Assignment: Lab #11 Exercise #3
 *      Demo Link: https://drive.google.com/file/d/1cH11ZQZ0MLxmZqDtfVczdDFvAruH1KWT/view?usp=sharing
 *	Exercise Description: Extra addition function,
If '#' is pressed then PB3 will light up and the button is ready for the combination, else nothing happens.
If the right combination is being inputted PB2 will light up as long as it's pressed then goes back to PB3

When inputting a new combination PB0 and PB1 lights up when a button on the keypad is being pressed, it takes in 4 inputs and replaces the old password.
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "scheduler.h"
#include <time.h>
#include <stdlib.h>
#include "keypad.h"

#endif
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


//   Global Variable Initialization    //
unsigned char ai_pos = 0x0E; 
unsigned char player_pos = 0x0E;
unsigned char level = 1;
static unsigned char ball_row = 0x04;
static unsigned char ball_col = 0x40;
unsigned char movingRight = 0;
unsigned char movingUp = 0;
static unsigned char speed = 500;
unsigned char spin = 0;
static unsigned char play = 0;
unsigned char player1_score = 0;
unsigned char player2_score = 0;
unsigned char player1_won, player2_won;
unsigned char display_score = 0;
unsigned char display_menu = 1;
unsigned char restart = 0;
unsigned char isMulti = 0;
////////////////////////////////////////


enum SM_States { SM_Start, SM_Play };
int AI_Tick(int state) {
	int r = rand();
	if (~PINA & 0x04) {
	   level = level + 1;
	} else { }
	switch(state) {
	   case(SM_Start):
		if (play) {
		   state = SM_Play;
		} else { }
		ai_pos = 0x0E;
		break;
	   default:
		break;
	}

	switch(state) {
	   case(SM_Start):
		break;
	   case(SM_Play):
		//Row Shifting
	 	if ((r % 100) <= (level * 30)) {
		   if ((ball_row & ai_pos) == 0) { //if ai pong is not lined up
			if ((ai_pos << 1) & ball_row) {
			   if (ai_pos == 0x1C) {
				ai_pos = 0x1C;
			   } else {
			   	ai_pos = ai_pos << 1;
			   }
			} else {
			   if (ai_pos == 0x07) {
				ai_pos = 0x07;
			   } else {
			   	ai_pos = ai_pos >> 1;
			   }
			} 
		   } else { }
		} else {
		   
		}
		if (!play) {
		   //ai_pos = 0x0E;
		   state = SM_Start;
		} else { }
		break;
	   default:
		break;
	}
	//ai_pos = row;
	//ai = col;
	return state;
}

enum playerSM {player_Start, player_Play};
int Player_Tick(int state) {
	switch(state) {
	   case(player_Start):
		if (play) {
		  state = player_Play;
		} else { }
		player_pos = 0x0E;
		break;
	   default:
		break;
	}

	switch(state) {
	   case(player_Play):
		if (~PINA & 0x01) {
		    if (player_pos == 0x1C) {
			player_pos = 0x1C;
		     } else {
			player_pos = player_pos << 1;
		    }
		} else if (~PINA & 0x02) {
	 	   if (player_pos == 0x07) {
			player_pos = 0x07;
		   } else {
			player_pos = player_pos >> 1;
		   }
		} else {
		  
		}
		if (!play) {
		   state = player_Start;
		} else { }
		break;
	   default:
		break;
	} 
	//player_pos = col;
	//PORTC = 0x01;
	//PORTD = ~player_pos;
	return state;
}

enum player2SM {player2_Start, player2_Play};
int Player2_Tick(int state) {
	switch(state) {
	   case(player2_Start):
		if (play) {
		  state = player_Play;
		} else { }
		ai_pos = 0x0E;
		break;
	   default:
		break;
	}

	switch(state) {
	   case(player2_Play):
		if (GetKeypadKey() == '1') {
		    if (ai_pos == 0x1C) {
			ai_pos = 0x1C;
		     } else {
			ai_pos = ai_pos << 1;
		    }
		} else if (GetKeypadKey() == '2') {
	 	   if (ai_pos == 0x07) {
			ai_pos = 0x07;
		   } else {
			ai_pos = ai_pos >> 1;
		   }
		} else {
		  
		}
		if (!play) {
		   state = player2_Start;
		} else { }
		break;
	   default:
		break;
	} 
	//player_pos = col;
	//PORTC = 0x01;
	//PORTD = ~player_pos;
	return state;
}

enum ballSM_State { ballSM_Start, ballSM_Play };
int Ball_Tick(int state) {
    unsigned char row = ball_row;
    unsigned char col = ball_col;
     switch (state) {
     case(ballSM_Start):
	if (play) {
	  state = ballSM_Play;
	} else { }
	ball_row = 0x04;
	ball_col = 0x40;
	break;
     default:
	//state = ballSM_Start;
	break;
}

     switch (state) {
     case(ballSM_Play):
	    //vertical
	    if (movingUp) {
	    	if ((col == 0x02) && (ai_pos & row)) { //if I hit the ping pong
		    movingUp = 0;
		    col = col << 1;
		    if (!((row << 1) & ai_pos) || !((row >> 1) & ai_pos)) {
			if (movingRight) {
			   movingRight = 0;
			} else {
			   movingRight = 1;
			}
		    spin = 1;
		    } else {
		    spin = 0;
		    }
		} else if (col == 0x01) {
		    col = col >> 1;
		    play = 0; //stop game;
		    player1_score = player1_score + 1;
		    display_score = 1;
		    if (player1_score >= 3) {
			player1_won = 1;
			display_score = 0;
			display_menu = 1;
		    } else { }
	    	} else {
		    col = col >> 1;
	    	}
	    } else {  //moving left
	    	if ((col == 0x40) && (player_pos & row)) { //if I hit the ping pong
		    movingUp = 1;
		    col = col >> 1;
		    if (!((row << 1) & player_pos) || !((row >> 1) & player_pos)) {
			if (movingRight) {
			   movingRight = 0;
			} else {
			   movingRight = 1;
			}
		    	spin = 1; 
		    } else {
		    	spin = 0;
		    }
		} else if (col == 0x80) {
		    col = col << 1;
		    play = 0; //stop game;
		    player2_score = player2_score + 1;
		    display_score = 1;
		    if (player2_score >= 3) {
			player2_won = 1;
			display_score = 0;
			display_menu = 1;
		    } else { }
	    	} else {
		    col = col << 1;
	    	}
	    }

	    //sideways
	    //If ball hits wall it no longer spins
	    if (movingRight) {
		if (spin) {
		    	if (row == 0x01) { //if I hit the right wall
			    movingRight = 0;
			    row = row << 1;
			    speed = 500;
			    spin = 0;
			    //col = col >> 1;
		    	} else {
			    row = row >> 1;
			    movingRight = 0;
			    spin = 0;
			    speed = speed - 100;
		    	}
		} else {
			speed = 500;
		    	if (row == 0x01) { //if I hit the right wall
			    movingRight = 0;
			    row = row << 1;
			    //col = col >> 1;
		    	} else {
			    row = row >> 1;
		    	}
		}
	    } else {  //moving left
		if (spin) {
		    	if (row == 0x10) { //if I hit the left wall
			    movingRight = 0;
			    row = row >> 1;
			    speed = 500;
			    spin = 0;
		    	} else {
			    row = row << 1;
			    spin = 0;
			    movingRight = 1;
			    speed = speed - 100;
		    	}
		} else {
			speed = 500;
		    	if (row == 0x10) { //if I hit the left wall
			    movingRight = 1;
			    row = row >> 1;
			    //col = col >> 1;
		    	} else {
			    row = row << 1;
		    	}
		}
	    }

	    if (~PINA & 0x08) {
		play = 0;
		display_menu = 1;
	 	display_score = 0;
	    } else { }

	    /*if (!play) {
		ball_row = 0x04;
		ball_col = 0x40;
		state = ballSM_Start;
	    } else { }*/
	break;
     default:
	//state = ballSM_Start;
	break;
}
     ball_row = row;
     ball_col = col;
     //PORTC = col;
     //PORTD = ~row;
     return state;
}

//void test_ball(unsigned char *ballRow, unsigned char *ballCol) {
void test_ball() {
    unsigned char row = ball_row;
    unsigned char col = ball_col;
    //test move up
    //vertical
    if (movingUp) {
    	if ((col == 0x02) && (ai_pos & row)) { //if I hit the ping pong
	    movingUp = 0;
	    col = col << 1;
	    //col = col >> 1;
	} else if (col == 0x01) {
	    col = col >> 1;
	    //stop game;
    	} else {
	    col = col >> 1;
    	}
    } else {  //moving left
    	if ((col == 0x40) && (player_pos & row)) { //if I hit the ping pong
	    movingUp = 1;
	    col = col >> 1;
	} else if (col == 0x10) {
	    col = col << 1;
	    //stop game;
    	} else {
	    col = col << 1;
    	}
    }

    //sideways
    if (movingRight) {
    	if (row == 0x01) { //if I hit the right wall
	    movingRight = 0;
	    row = row << 1;
	    //col = col >> 1;
    	} else {
	    row = row >> 1;
    	}
    } else {  //moving left
    	if (row == 0x10) { //if I hit the left wall
	    movingRight = 1;
	    row = row >> 1;
    	} else {
	    row = row << 1;
    	}
    }
    
    
    //*ballRow = ~row;
    //row = ( row << 1);
    ball_row = row;
    ball_col = col;
    PORTD = ~row;
    PORTC = col;
}

int sz = 5;
unsigned char pattern[3] = {0x07, 0x0E};
unsigned char cols[5] = { 0x01, 0x80, 0xFF};
unsigned char j = 0;
enum game_Tick { game_Start, game_Play, game_Score, game_Menu };
int Game_Tick(int state) {
   if ((player1_won || player2_won) & !(display_menu)) {
	player1_won = 0;
	player2_won = 0;
	player1_score = 0;
	player2_score = 0;
	display_menu = 1;
	display_score = 0;
	state = game_Menu;
   } else { }

   if (~PINA & 0x08) {
	player1_won = 0;
	player2_won = 0;
	player1_score = 0;
	player2_score = 0;
	display_menu = 1;
	display_score = 0;
	state = game_Menu;
   } else { }

   /*if (display_score) {
	state = game_Score;
   } else { }*/

   switch(state) {
     case(game_Start):
	//state = game_Start;
	if (~PINA & 0x01) {
	  state = game_Play;
	  play = 1;
	} else if (display_menu) {
	  state = game_Menu;
	} else {
	  state = game_Start;
	  play = 0;
	}
	level = 1;
	PORTC = cols[(j % 3)];
	//PORTC = ai_pos;
	if (cols[j % 3] == 0x01) {
	   PORTD = ~0x0E; 
	} else if (cols[j % 3] == 0x80) {
	   PORTD = ~0x0E;
	} else {
	   PORTD = ~(0x04);
	   PORTC = 0x40;
	}
	player_pos = 0x0E;
	ai_pos = 0x0E;
	ball_row = 0x04;
	ball_col = 0x40;
	//PORTC = pattern[(j % 2)];
	j = j + 1;

	/*if (display_score) {
	   state = game_Score;
	} else { }*/
	break;
     case(game_Score):
	if (!display_score) {
	   state = game_Start;
	} else {
	   state = game_Score;
	} 
	break;
     case(game_Menu):
	if (!display_menu) {
	   state = game_Start;
	} else {
	 
	} 
	break;
     default:
	//state = game_Start;
	break;
   }

   switch(state) {
     case(game_Play):
	PORTC = cols[(j % 3)];
	//PORTC = ai_pos;
	if (cols[j % 3] == 0x01) {
	   PORTD = ~ai_pos; 
	} else if (cols[j % 3] == 0x80) {
	   PORTD = ~player_pos;
	} else {
	   PORTD = ~ball_row;
	   PORTC = ball_col;
	}
	j = j + 1;
	if (!play) {
	  state = game_Score;
	} else { }

	if (display_score) {
	  state = game_Score;
	} else { } 

	if (display_menu) {
	  state = game_Menu;
	} else { }
	break;
     default:
	break;
   }
   return state;
}

unsigned char scores[10] = {0x80, 0x40, 0x20, 0x04, 0x02, 0x01};
unsigned char patterns[30] = { 0x1F, 0x11, 0x1F, 0x00, 0x1F, 0x00, 0x1D, 0x15, 0x17, 0x11, 0x15, 0x1F, 0x07, 0x04, 0x1F, 0x17, 0x15, 0x1D, 0x1F, 0x15, 0x1D, 0x01, 0x01, 0x1F, 0x1F, 0x15, 0x1F, 0x07, 0x05, 0x1F };  
//unsigned char one[3] = {0x00, 0x1F, 0x00};   //1
//unsigned char two[3] = {0x1D, 0x15, 0x17};  //2
//unsigned char three[3] = {0x15, 0x15, 0x1F};  //3
//unsigned char four[3] = {0x07, 0x04, 0x1F};  //4
//unsigned char five[3] = {0x17, 0x15, 0x1D,}; //5
//unsigned char six[3] = {0x1F, 0x15, 0x1D,}; //6
//unsigned char seven[3] = {0x01, 0x01, 0x1F,}; //7
//unsigned char eight[3] = {0x1F, 0x15, 0x1F,}; //8
//unsigned char nine[30] = {0x07, 0x05, 0x1F,}; //9
unsigned char counter_row = 0;
unsigned char counter_col = 0;

enum score_SM {score_Start, score_Display};
int score_Tick(int state) {
	unsigned char player1_tmp = 0;
	unsigned char player2_tmp = 0;
	player1_tmp = player1_score * 3;
	player2_tmp = player2_score * 3;
	switch (state) {
	   case(score_Start):
		if (display_score) {
		   state = score_Display;
		} else {
		   state = score_Start;
		   display_score = 0;
		}
		break;
	   default:
		break;
	}

	switch (state) {
	   case(score_Display):
		PORTC = scores[counter_row];
		if (counter_row < 3) {
		    PORTD = ~patterns[counter_row + (player1_tmp)];
		} else {
		    PORTD = ~patterns[counter_row - 3 + (player2_tmp)];
		}
		counter_row = counter_row + 1;
		if (GetKeypadKey() == '#' || !display_score) {
		    display_score = 0;
		    state = score_Start;
		} else { }
		break;
	   default:
		break;
	}
	return state;
}

enum display_SM {display_Start, display_Display};
int display_Tick(int state) {
	unsigned multi_tmp = 0;
	switch (state) {
	   case(display_Start):
		if (display_menu) {
		   state = display_Display;
		} else {
		   state = display_Start;
		   //display_score = 0;
		}
		break;
	   default:
		break;
	}

	switch (state) {
	   case(display_Display):
		if (GetKeypadKey() == '3') {
		    isMulti = 0;
		} else if (GetKeypadKey() == '4') {
		    isMulti = 1;
		} else { }
		multi_tmp = (isMulti + 1) * 3;
		PORTC = scores[counter_row % 3] >> 3;
		PORTD = ~patterns[counter_row % 3 + (multi_tmp)];
		counter_row = counter_row + 1;
		if (GetKeypadKey() == '0') {
		    display_menu = 0;
		    state = display_Start;
		} else { }
		break;
	   default:
		break;
	}
	return state;
}

void display_test() {
	unsigned multi_tmp = 0;
	//3 in keypad selects how many multipplayers
	if (GetKeypadKey() == '3') {
	    isMulti = 0;
	} else if (GetKeypadKey() == '4') {
	    isMulti = 1;
	} else { }
	multi_tmp = (isMulti + 1) * 3;
	PORTC = scores[counter_row % 3] >> 3;
	PORTD = ~patterns[counter_row % 3 + (multi_tmp)];
	counter_row = counter_row + 1;
}


/*void display_score() {
		unsigned char player1_tmp = 0;
		unsigned char player2_tmp = 0;
		player1_tmp = player1_score * 3;
		player2_tmp = player2_score * 3;
		PORTC = scores[counter_row];
		if (counter_row < 3) {
		    PORTD = ~patterns[counter_row + (player1_tmp)];
		} else {
		    PORTD = ~patterns[counter_row - 3 + (player2_tmp)];
		}
		counter_row = counter_row + 1;
		state = score_Start;
}*/

/*
unsigned char menu[10] = { }
unsigned char menu_counter = 0;
void display_menu() {
	PORTC = 0x80 >> menu_counter
	PORTD = ~menu[menu_counter];
}*/

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRB = 0xF0; PORTB = 0x0F;
	DDRC = 0xFF; PORTC = 0x00;
	DDRA = 0x00; PORTA = 0xFF;
	DDRD = 0xFF; PORTD = 0x00;
	//SM Initializations

	//Declare an array of tasks
	static task task1, task2, task3, task4, task5, task6;
	task *tasks[] = { &task1, &task2, &task3, &task4, &task5, &task6};
	const unsigned char numTasks = sizeof(tasks)/sizeof(task*);

	const char start = 0;
	//Task 1 
	task1.state = start;
	task1.period = 1;
	task1.elapsedTime = task1.period;
	task1.TickFct = &Game_Tick;

	//Task 2
	task2.state = start;
	task2.period = speed;
	task2.elapsedTime = task1.period;
	task2.TickFct = &Ball_Tick;

	//Task 3 
	task3.state = start;
	task3.period = 300;
	task3.elapsedTime = task1.period;
	task3.TickFct = &Player_Tick;

	//Task 4 
	task4.state = start;
	task4.period = 300;
	task4.elapsedTime = task1.period;
	task4.TickFct = &Player2_Tick;

	//Task 5 
	task5.state = start;
	task5.period = 1;
	task5.elapsedTime = task1.period;
	task5.TickFct = &score_Tick;

	//Task 6 
	task6.state = start;
	task6.period = 1;
	task6.elapsedTime = task1.period;
	task6.TickFct = &display_Tick;

	//Variable Initialization
	unsigned long GCD = tasks[0]->period;
	unsigned char i = 0;
	srand(time(NULL));

	//Timer Initialization
	TimerSet(GCD);
	TimerOn();
    /* Insert your solution below */
    while (1) {
	if (display_menu) {
		if (isMulti) {
			task4.TickFct = &Player2_Tick;
		} else {
			task4.TickFct = &AI_Tick;
		}
	}

	for ( int i = 0; i < numTasks; i++ ) {
		if ( tasks[i]->elapsedTime == tasks[i]->period ) {
			tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
			tasks[i]->elapsedTime = 0;
		}
		GCD = findGCD(GCD, tasks[i]->period);
		tasks[i]->elapsedTime += GCD;
	}
	//display_test();


	//void display_menu();
	/*if (menu_counter >= 8) {
		menu_counter = 0;
	} else {
		menu_counter = menu_counter + 1;
	}*/
	/*player1_score = 3;
	player2_score = 9;
	display_score();*/
	if (counter_row == 6) {
		counter_row = 0;
	}
	//counter_ = 0x80;
	while(!TimerFlag);
	TimerFlag = 0;
    }
    return 1;
}
