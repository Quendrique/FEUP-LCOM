#include <stdio.h>
#include "SerialPort.h"

unsigned long rate = 1200; //pretty sure this is enough for our needs
int serial_hook_id = SERIAL_IRQ_COM1;

//ASSUMING ONLY COM1 WILL BE USED

unsigned long serial_get_lsr() {

	unsigned long lsr;

	if (sys_inb(COM1_BASE_ADDRESS + UART_LSR_OFFSET, &lsr) != OK)
		return 1;

	return lsr;

}

int serial_subscribe_int() {

	int hook_tmp = serial_hook_id;

	//subscribing to interrupts

	if (sys_irqsetpolicy(SERIAL_IRQ_COM1, (IRQ_REENABLE | IRQ_EXCLUSIVE), &serial_hook_id) == OK) {

		if (sys_irqenable(&serial_hook_id) == OK)

			return hook_tmp;

	}

	return 1;

}

int serial_initialize() {

	//setting DLAB accordingly

	unsigned long lcr;

	if (sys_inb(COM1_BASE_ADDRESS + UART_LCR_OFFSET, &lcr) != OK)
		return 1;

	lcr &= 0xFF7F;

	if (sys_outb(COM1_BASE_ADDRESS + UART_LCR_OFFSET, lcr) != OK)
		return 1;

	//setting the interrupt enable register accordingly

	unsigned long ier = (IER_ENABLE_RECEIVED_DATA | IER_ENABLE_TRANSMITTER_EMPTY );

	if (sys_outb(COM1_BASE_ADDRESS + UART_IER_OFFSET, ier) != OK)
		return 1;

	//setting the fifo accordingly

	unsigned long fcr = (FCR_ENABLE | FCR_CLEAR_TRANSMIT | FCR_CLEAR_RECEIVE | FCR_TRIGGER_LEVEL_8);

	if (sys_outb(COM1_BASE_ADDRESS + UART_FCR_OFFSET, fcr) != OK)
		return 1;

	//setting the queues

	serial_transmitter_queue = new_queue();

	if (serial_transmitter_queue == NULL)
		return 1;

	serial_receiver_queue = new_queue();

	if (serial_receiver_queue == NULL)
		return 1;

	return 0;
}

int serial_unsubscribe_int() {

	//deleting queues

	queue_delete(serial_receiver_queue);
	queue_delete(serial_transmitter_queue);

	//setting the ier accordingly

	if (sys_outb(COM1_BASE_ADDRESS + UART_IER_OFFSET, 0) != OK)
		return 1;

	//unsubscribing to the interrupts

	if (sys_irqdisable(&serial_hook_id) == OK) {

		if (sys_irqrmpolicy(&serial_hook_id) == OK) {
			return 0;
		}
	} else {

		printf("Invalid unsubscription");
		return 1;
	}

}

int serial_set_conf() {

	unsigned long lcr;
	unsigned long aux_lsb;
	unsigned long aux_msb;

	//configuring the number of stop bits and parity

	if (sys_inb(COM1_BASE_ADDRESS + UART_LCR_OFFSET, &lcr) != OK)
		return 1;

	lcr &= (LCR_WORD_LENGTH_8 | LCR_PARITY_ODD | LCR_LOAD_DL);

	if (sys_outb(COM1_BASE_ADDRESS + UART_LCR_OFFSET, lcr) != OK)
		return 1;

	//configuring the bitrate

	aux_lsb = (0x00FF & (MAX_BITRATE/rate));
	aux_msb = (0xFF00 & (MAX_BITRATE/rate));

	if (sys_outb(COM1_BASE_ADDRESS, aux_lsb) != OK)
		return 1;

	if (sys_outb(COM1_BASE_ADDRESS + UART_DLH_OFFSET, aux_msb) != OK)
		return 1;

	//resetting the DLAB to 0

	lcr &= 0xFF7F;

	if (sys_outb(COM1_BASE_ADDRESS + UART_LCR_OFFSET, lcr) != OK)
		return 1;

	return 0;

}

int serial_int_handler() {

	printf("interrupt handler called\n");

	unsigned long iir;

	if (sys_inb(COM1_BASE_ADDRESS + UART_IIR_OFFSET, &iir) != OK)
		return 1;

	if ((iir & IIR_INTERRUPT_PENDING) == 0) {

		printf("%x\n", iir);

		if ((iir & IIR_TRANSMITTER_EMPTY) != 0) {
			return serial_transmitter_send_string();

		} else if ((iir & IIR_RECEIVED_DATA) != 0) {
			return serial_receiver_receive_string();

		}

	}

}

int serial_transmitter_send_string() {

	unsigned char* to_send;
	unsigned long lsr;

	if (sys_inb(COM1_BASE_ADDRESS + UART_LSR_OFFSET, &lsr) != OK)
		return 1;

	while(!is_queue_empty(serial_transmitter_queue)) {

		//check the character which to send and removing it from the queue

		if (lsr & LSR_TRANSMITTER_EMPTY == 0)
			break;

		to_send = queue_pop(serial_transmitter_queue);

		if (sys_outb(COM1_BASE_ADDRESS, *to_send) != OK)
			return 1;
		free(to_send);

		//update lsr

		if((lsr = serial_get_lsr()) == 1)
			return 1;


	}

	return 0;
}

int serial_receiver_receive_string() {

	unsigned long lsr;

	lsr = serial_get_lsr();

	void* elem;

	while (lsr & LSR_DATA_READY != 0) {

		elem = malloc(sizeof(unsigned long));

		if (elem == NULL)
			return 1;

		//check for errors

		if ((lsr & (LSR_FRAMING_ERROR | LSR_PARITY_ERROR | LSR_OVERRUN_ERROR)) != 0)
			return 1;

		//update lsr


		if ((lsr = serial_get_lsr()) == 1) {
			free(elem);
			return 1;
		}


		//get the element in the fifo

		if (sys_inb(COM1_BASE_ADDRESS, elem) != OK)
			return 1;

		//put the element in the queue

		if (!queue_push(serial_receiver_queue, elem)) {
			free(elem);
			return 1;
		}

	}


	return 0;
}

int serial_transmitter_string_to_queue(unsigned char* string) {

	unsigned char* elem;

	while (strlen(string) > 0) {

		elem = malloc(sizeof(unsigned char));

		if (elem == NULL)
			return 1;

		*elem = *string;

		if (!queue_push(serial_transmitter_queue, elem))
			return 1;
		string++;

	}

	unsigned char* null_terminator = (unsigned char*) malloc (sizeof(unsigned char));
	*null_terminator = '\0';

	if (!queue_push(serial_transmitter_queue, null_terminator))
		return 1;

	if(serial_transmitter_send_string() == 1)
		return 1;

	return 0;

}

int serial_receiver_string_from_queue(unsigned char** string) {

	unsigned int i = 0;
	unsigned char* elem;
	*string = NULL;

	do {
		*string = realloc(*string, (i+1)*sizeof(**string));

		if ((*string) == NULL)
			return 1;

		elem = queue_pop(serial_receiver_queue);
		if (elem == NULL)
			break;

		(*string)[i] = *elem;
		free(elem);
		i++;

	} while ((*string)[i] != '\0');


	return 0;

}

