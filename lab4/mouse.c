#include "i8042.h"

int mouse_hook_id = MOUSE_IRQ;

int mouse_subscribe_int(void) {

        int hook_tmp = mouse_hook_id;

        if (sys_irqsetpolicy(MOUSE_IRQ, (IRQ_REENABLE | IRQ_EXCLUSIVE), &mouse_hook_id) == OK) {
        	if (sys_irqenable(&mouse_hook_id) == OK) {
        		return hook_tmp;
            }
        }

        printf("Invalid subscription");
        return 1;



}

int mouse_unsubscribe_int(void) {

        if (sys_irqdisable(&mouse_hook_id) == OK) {
        	if (sys_irqrmpolicy(&mouse_hook_id) == OK) {
        		return 0;
            }
        }

        printf("Invalid unsubscription");
        return 1;



}

int mouse_get_status(unsigned char *st) {

        long unsigned int tmpStatus;

        if(sys_inb(KBD_STAT_REG, &tmpStatus) == OK) {
        	*st= (unsigned char) tmpStatus;
            return 0;
        }
        else {
        	printf("Failed to obtain status\n");
        	return -1;
        }

}

unsigned int mouse_handler() {

	long unsigned int byte;
	unsigned int count = 0;
	unsigned char status;

	while(count < 12) {

		mouse_get_status(&status);

		if ((status & KBD_STAT_OBF) != 0) {

			if((status & (KBD_STAT_PARITY | KBD_STAT_TIMEOUT ))==0) {

				if ((status & KBD_STAT_AUX) != 0) {

					sys_inb(OUT_BUFFER, &byte);
					count++;
					return byte;
				}
			}

		}

		count++;
		tickdelay(micros_to_ticks(DELAY_US));

	}

	return 1;

}

unsigned int mouse_first_cmd() {

	unsigned int count = 0;
	unsigned char status;

	while(count < 12) {

		mouse_get_status(&status);

		if((status & KBD_STAT_IBF) == 0) {
			sys_outb(KBD_STAT_REG, MOUSE_WRITE_BYTE);
			return 0;
		}

		count++;
		tickdelay(micros_to_ticks(DELAY_US));

	}

	return 1;

}

unsigned int mouse_write_cmd(unsigned char cmd) {

	unsigned char status;
	long unsigned int byte;
	unsigned int count = 0;

	while(count < 6) {

		//FIRST COMMAND

		mouse_first_cmd();

		//SECOND COMMAND

		mouse_get_status(&status);

		//printf("Status after first command: %x\n", status);

		if((status & KBD_STAT_IBF) == 0) {
			sys_outb(OUT_BUFFER, cmd);
			sys_inb(OUT_BUFFER, &byte);
		}

		//printf("Acknowledgment byte from second written cmd: %x\n", byte);

		if (byte == NACK || byte == ERROR){
			count++;
			continue;
		} else
			return 0;

		tickdelay(micros_to_ticks(DELAY_US));
	}

	return 1;

}

void mouse_print(unsigned int *packet) {

	unsigned int B1, B2, B3, LB, RB, XOV, YOV, X, Y;
	B1 = packet[0];
	B2 = packet[1];
	B3 = packet[2];
	LB = (packet[0] & BIT(0));
	RB = ((packet[0] & BIT(1))>>(1));
	XOV = ((packet[0] & BIT(6))>>(6));
	YOV = ((packet[0] & BIT(7))>>(7));
	X = packet[1];
	Y = packet[2];
	printf("B1=%x B2=%x B3=%x LB=%lu RB=%lu XOV=%lu YOV=%lu X=%lu Y=%lu\n",
			B1,
			B2,
			B3,
			LB,
			RB,
			XOV,
			YOV,
			X,
			Y);


}
