#include <minix/syslib.h>
#include <minix/drivers.h>
#include "timer.h"
#include "i8254.h"

static long unsigned int counter = 0; //global counter
int hook_id = 0x00;

#define FREQ_MIN 19 //it's 19 because if you divide TIMER_FREQ (the default frequency) by the maximum amount of representable bits (16) you'll get 18,2 remainder, so anything bellow 19 won't work.


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

void timer_int_handler() {
	counter++;
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

void byteparaBinario(unsigned char byte){
	//this function is used to take conf status byte and spell out its binary value.
	int i = 0;

	for(i = 7; i>=0 ; i--)
	{
		if((byte>>i) & 1 == 1)
		{
			printf("1");
		}

		else
		{
			printf("0");
		}
	}
};

int timer_display_conf(unsigned char conf) {

	printf("Timer Status\n");

	printf("Value: ");

	byteparaBinario(conf);

	printf(" \n");

	printf("Last output: %x\n", (conf & BIT(7))>>(7));

	if ((conf & BIT(6)) == 1)
		printf("Null count\n");
	else
		printf("Count available for reading\n");

	printf("Type of access: ");

	if (conf & BIT(5) == 0)
		printf("LSB\n");
	else if (conf & BIT(4) == 0)
		printf("MSB\n");
	else
		printf("LSB followed by MSB\n");

	printf("Operating mode: ");

	if (conf & BIT(3) == 0){
		if (conf & BIT(2) == 0) {
			if (conf & BIT(1) == 0)
				printf("O\n"); //000 operating mode value
			else
				printf("1\n"); //001 operating mode value
		}
		else if (conf & BIT(1) == 0)
			printf("3\n"); //011 operating mode value
		else
			printf("2\n"); //010 operating mode value
	}
	else if (conf & BIT(2) == 0){
		if (conf & BIT(1) == 0)
			printf("4\n"); //100 operating mode value
		else
			printf("5\n"); //101 operating mode value
	}
	else if (conf & BIT(1) == 0)
		printf("2\n"); //110 operating mode value
	else
		printf("3\n"); //111 operating mode value

	printf("Counting mode: ");

	if (conf & BIT(0) == 0)
		printf("BCD\n");
	else
		printf("Binary\n");

	return 0;
}

int timer_test_time_base(unsigned long freq){

	//all this does is call the function we already made.

	if(timer_set_frequency(0, freq) == 0)
	{
		return 0;
	}

	return 1;
}

int timer_test_int(unsigned long time) {

	int ipc_status, r, irq_set = BIT(hook_id), msgCounter=0; //msgCounter determines the amount of messages displayed per function call
	message msg;

	//1.Subscribe timer 0 interrupts.

	if (timer_subscribe_int() != hook_id){ //subscription failed
		return 1;
	}

	//2.Handler should print message.

	while(msgCounter <= time) { //since one message is written per second, once the amount of registered messages reaches the time interval given by the "time" parameter, the cycle should stop

		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}

		if (is_ipc_notify(ipc_status)) {

			switch (_ENDPOINT_P(msg.m_source)) {

			case HARDWARE:/* TIMER0 interrompe */
				if (msg.NOTIFY_ARG & irq_set) {

					//it prints one message per second, in a given interval ..
					//the amount of seconds passed each time is given by dividing the amount stored in "counter" global by the default frequency.

					if(counter % 60 == 0) { //the amount of seconds must be a whole number, so check if you can divide it with remainder 0 first.

					printf("Seconds passed: %d\n", counter/60);
					msgCounter++;

					}

					timer_int_handler(); //the handler is placed after the printf so that the first run-through of the cycle prints 0 instead of 1, which would cause the final count to be one unit above the expected

				}

				break;

			default:

				break;

			}

		} else { }
	}

	//3.Unsubscribe Timer 0 at the end.

	if(timer_unsubscribe_int()!=OK) {

		return 1;
	}

	printf("Interrupt loop was successful!\n");

	return 0;

}

int timer_test_config(unsigned char timer) {

	if(timer > 2)
	{
		//invalid timer
		printf("Invalid timer.");
		return 1;
	}

	unsigned char st;

	if ( (timer_get_conf(timer, &st)) == 0)
	{
		if ( (timer_display_conf(st)) == 0)
		{
			return 0;
		}
	}

	return 1;
}
