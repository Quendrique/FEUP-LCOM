#include <minix/syslib.h>
#include <minix/drivers.h>
#include <stdio.h>
#include "Timer.h"
#include "i8254.h"
#include "Graphics.h"
#include "Bitmap.h"
#include "Game.h"

int hook_id = 0x00;

#define FREQ_MIN 19 //it's 19 because if you divide TIMER_FREQ (the default frequency) by the maximum amount of representable bits (16) you'll get 18,2 remainder, so anything bellow 19 won't work.

void initialize_timer_counter() {
	timer_counter = 0;
}

Timer* new_timer() {

	Timer* timer = (Timer*) malloc(sizeof(Timer));

	timer->seconds_1 = 0;
	timer->seconds_2 = 0;
	timer->minutes_1 = 1;

	timer->draw = 1;
	timer->done = 0;

	timer->dois_pontos = loadBitmap("/home/lcom/svn/project/src/images/2pontos.bmp");
	timer->numbers[0] = loadBitmap("/home/lcom/svn/project/src/images/0.bmp");
	timer->numbers[1] = loadBitmap("/home/lcom/svn/project/src/images/1.bmp");
	timer->numbers[2] = loadBitmap("/home/lcom/svn/project/src/images/2.bmp");
	timer->numbers[3] = loadBitmap("/home/lcom/svn/project/src/images/3.bmp");
	timer->numbers[4] = loadBitmap("/home/lcom/svn/project/src/images/4.bmp");
	timer->numbers[5] = loadBitmap("/home/lcom/svn/project/src/images/5.bmp");
	timer->numbers[6] = loadBitmap("/home/lcom/svn/project/src/images/6.bmp");
	timer->numbers[7] = loadBitmap("/home/lcom/svn/project/src/images/7.bmp");
	timer->numbers[8] = loadBitmap("/home/lcom/svn/project/src/images/8.bmp");
	timer->numbers[9] = loadBitmap("/home/lcom/svn/project/src/images/9.bmp");
	timer->background = loadBitmap("/home/lcom/svn/project/src/images/timer_background.bmp");
	return timer;

}

void update_timer (Timer* timer) {

	timer->draw = 1;

	if (timer->seconds_1 == 0) {
		timer->seconds_1 = 9;

		if (timer->seconds_2 == 0) {
			timer->seconds_2 = 5;

			if (timer->minutes_1 == 0) {

				timer->done = 1;

			} else
				timer->minutes_1--;

		}  else
			timer->seconds_2--;

	} else
		timer->seconds_1--;

}

void draw_timer(Timer* timer) {

	//timer->draw = 0;

	draw_bitmap_to_buffer(timer->background, 869, 5, ALIGN_LEFT, background_buffer);
	draw_bitmap_to_buffer(timer->numbers[timer->seconds_1],934,10,ALIGN_LEFT,background_buffer);
	draw_bitmap_to_buffer(timer->numbers[timer->seconds_2],904,10,ALIGN_LEFT,background_buffer);
	draw_bitmap_to_buffer(timer->dois_pontos,904,10,ALIGN_LEFT,background_buffer);
	draw_bitmap_to_buffer(timer->numbers[timer->minutes_1],874,10,ALIGN_LEFT,background_buffer);

}

void delete_timer(Timer* timer) {
	free(timer);
}


int timer_set_frequency(unsigned char timer, unsigned long freq) {

	if(timer > 2 || freq == 0 || freq > TIMER_FREQ || freq < FREQ_MIN)
	{
		//invalid timer or frequency ..
		printf("Invalid timer or frequency.\n");
		return 1;
	}

	unsigned long mascara;
	unsigned long divisorLSB; //LSB of the divisor.
	unsigned long divisorMSB; //MSB of the divisor.
	unsigned char conf;
	unsigned int divisor;
	unsigned int freqbase;
	unsigned char porta;
	unsigned long commando;

	freqbase = TIMER_FREQ; //base frequency

	divisor = freqbase / freq;

	timer_get_conf(timer, &conf);

	/*BIT(0)
		00000001
		BIT(1)
		00000010
		BIT(2)
		00000100
		BIT(3)
		00001000*/

	/* 00001111 bit mask that will help us preserve the 4 bits of conf */

	/*mask = BIT(0) | BIT(1) | BIT(2) | BIT(3)*/

	mascara = BIT(0) | BIT(1) | BIT(2) | BIT(3);

	commando = TIMER_SEL0 | TIMER_LSB_MSB | (conf & mascara) | TIMER_SQR_WAVE | TIMER_BIN; //we select timer 0, LSB and then MSB, square wave mode and information is sent in binary.
	porta = TIMER_0;

	divisorLSB = (divisor & 0x0FF);
	divisorMSB = (divisor & 0xFF00);

	//moving the MSB into the right position.
	divisorMSB = (divisorMSB>>8);

	//if the timer actually isn't TIMER_0 (presumed by default ..)

	if(timer==1){
		porta = TIMER_1;
		commando = TIMER_SEL1 | TIMER_LSB_MSB | (conf & mascara) | TIMER_SQR_WAVE | TIMER_BIN;
	}

	if(timer==2){
		porta = TIMER_2;
		commando = TIMER_SEL2 | TIMER_LSB_MSB | (conf & mascara) | TIMER_SQR_WAVE | TIMER_BIN;
	}


	if(sys_outb(TIMER_CTRL, commando) == 0)
	{
		if((sys_outb(porta,divisorLSB)) == 0)
		{

			if((sys_outb(porta,divisorMSB)) == 0)
			{
				return 0;
			}
		}
	}

	return 1;
}

int timer_get_conf(unsigned char timer, unsigned char *st) {

	unsigned long conf;
	unsigned char porta;
	unsigned long commando;

	if(timer > 2){
		//invalid timer
		return 1;
	}


	//timer is just the number associated with it, porta will be the actual address.

	//read back command for reading status.

	commando = TIMER_RB_CMD | TIMER_RB_STATUS_ | TIMER_RB_SEL(timer);

	if(timer==0){
		porta = TIMER_0;
	}

	if(timer==1){
		porta = TIMER_1;
	}

	if(timer==2){
		porta = TIMER_2;
	}

	if(sys_outb(TIMER_CTRL, commando) == 0)
	{
		if(sys_inb(porta, &conf) == 0)
		{
			*st = (unsigned char) conf;
			return 0;
		}
	}

	return 1;
}


int timer_subscribe_int(void ) {

	if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id) == OK)
	{
		if (sys_irqenable(&hook_id) == OK)
		{
			return hook_id;
		}
	} else {

		printf("Invalid subscription");
		return 1;

	}
}

int timer_unsubscribe_int() {

	if (sys_irqrmpolicy(&hook_id) == OK)
	{
		if (sys_irqdisable(&hook_id) == OK)
		{
			return 0;
		}
	} else {

		printf("Invalid unsubscription");
		return 1;

	}

}

int timer_int_handler() {
	timer_counter++;

	//a cada segundo que passa, returns 1;

	if (timer_counter % 60 == 0)
		return 1;
	else
		return 0;

}
