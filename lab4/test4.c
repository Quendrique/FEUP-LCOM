#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/driver.h>
#include <minix/sysutil.h>
#include <minix/ipc.h>
#include "i8042.h"
#include "i8254.h"
#include "test4.h"

int mouse_hook_id = MOUSE_IRQ;
int hook_id = 0x00;
static long unsigned int counter = 0;


int timer_subscribe_int(void ) {

        unsigned long int hook_temp = hook_id;

        if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id) == OK)

        {

                        if (sys_irqenable(&hook_id) == OK)

                        {

                                return hook_temp;

                        }

        }

        printf("Invalid subscription");

        return 1;

}

int timer_unsubscribe_int() {

        if (sys_irqdisable(&hook_id) == OK)

        {

                if (sys_irqrmpolicy(&hook_id)== OK)

                {

                        return 0;

                }

        }

        printf("Invalid unsubscription");

        return 1;

}

void timer_int_handler() {

        counter++;

}


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


long unsigned int kbc_read_cmd_byte(void) {

	unsigned char status;
	long unsigned int byte;
	unsigned int count = 0;

	while (count < 12) {

		mouse_get_status(&status);

		if ((status & KBD_STAT_IBF) == 0) {

			sys_outb(KBD_STAT_REG, READ_CMD);
			sys_inb(OUT_BUFFER, &byte);

			return byte;

		}

		count++;
		tickdelay(micros_to_ticks(DELAY_US));
	}

	printf("mouse_read_cmd_byte() failed\n");
	return 1;

}

unsigned int kbc_write_byte(unsigned char cmd) {

	unsigned int count = 0;
		unsigned char status;

		while(count < 12) {

			mouse_get_status(&status);

			if((status & KBD_STAT_IBF) == 0) {
				sys_outb(KBD_STAT_REG, cmd);
				return 0;
			}

			count++;
			tickdelay(micros_to_ticks(DELAY_US));

		}

		return 1;


}

unsigned int kbc_write_cmd_byte(unsigned char arg) {

	unsigned int count = 0;
	unsigned char status;

	while(count < 12) {

		mouse_get_status(&status);

		if((status & KBD_STAT_IBF) == 0) {
			sys_outb(KBD_STAT_REG, WRITE_CMD);
			sys_outb(OUT_BUFFER, arg);
			return 0;
		}

		count++;
		tickdelay(micros_to_ticks(DELAY_US));

	}

	return 1;

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

	unsigned int B1, B2, B3, LB, RB, XOV, YOV;
	int X, Y;
	B1 = packet[0];
	B2 = packet[1];
	B3 = packet[2];
	LB = (packet[0] & BIT(0));
	RB = ((packet[0] & BIT(1))>>(1));
	XOV = ((packet[0] & BIT(6))>>(6));
	YOV = ((packet[0] & BIT(7))>>(7));

	if (NEGATIVE_X & packet[0]) {
		X = (packet[1]-256);
	} else {
		X = packet[1];
	}

	if (NEGATIVE_Y & packet[0]) {
		Y = (packet[2]-256);
	} else {
		Y = packet[2];
	}



	printf("B1=%x B2=%x B3=%x LB=%lu RB=%lu XOV=%lu YOV=%lu X=%d Y=%d\n",
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

int mouse_test_packet(unsigned short cnt){

	int ipc_status, irq_set = BIT(mouse_hook_id), r;
	message msg;
	unsigned int packet_count = 0;
	unsigned int packet[3];
	unsigned int test_byte = 0, byte1, byte2, byte3;
	int byte1_check = 0, byte2_check = 0;
	//long unsigned int command_byte;

	//1.Subscribe to mouse interrupts and check if it failed

	if(mouse_subscribe_int() != MOUSE_IRQ) {
		return 1;
	}

	//1.2 Enable mouse (needed?)

	kbc_write_byte(MOUSE_ENB);

	//1.3 Enable stream mode

	mouse_write_cmd(ENB_STREAM);
	//printf("After enabling stream\n");

	//1.4 Enable data reporting

	mouse_write_cmd(ENB_DATA_REPORT);
	//printf("After enabling data reporting\n");

	//2. Handling the interrupts

	while (packet_count < cnt) {

		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}

		if (is_ipc_notify(ipc_status)) {

			switch (_ENDPOINT_P(msg.m_source)) {

			case HARDWARE:

				if (msg.NOTIFY_ARG & irq_set) {

					test_byte = mouse_handler();

					if (byte1_check == 0) { // checks if byte 1 has been read

						if (test_byte & BIT(3)) {
							byte1_check = 1;
							byte1 = test_byte;
						} else
							continue;

					} else if (byte2_check == 0) {  // checks if byte 2 has been read

						byte2_check=1;
						byte2 = test_byte;

					} else {

						byte3 = test_byte;
						byte1_check=0; byte2_check=0;
						packet[0] = byte1;
						packet[1] = byte2;
						packet[2] = byte3;
						packet_count++;
						mouse_print(packet);

						}
					}

				}
			}
		}

    //3.Unsubscribe to mouse interrupts and check if it failed

    if(mouse_unsubscribe_int()!= 0) {
            return 1;
    }

    //3.1 Disable data report

    mouse_write_cmd(DIS_DATA_REPORT);

    return 0;


}

int mouse_test_remote(unsigned long period, unsigned short cnt){

	unsigned int packet_count = 0, counter = 0;
	unsigned int packet[3];
	long unsigned int test_byte = 0;
	unsigned int byte1, byte2, byte3;
	int byte1_check = 0, byte2_check = 0;
	unsigned char status;

	//1. Disable interrupts

	kbc_write_cmd_byte(MOUSE_DIS_INT_CMD);

	//printf("cmd byte after disabling interrupts: %x\n", kbc_read_cmd_byte());

	//1.1 Enable mouse (needed?)

	//kbc_write_byte(MOUSE_ENB);

	//1.2 Disable stream mode (data reporting)

	mouse_write_cmd(DIS_DATA_REPORT);

	//1.3 Enable remote mode

	mouse_write_cmd(MOUSE_REMOTE_MODE);

	while (packet_count < cnt) {

		if (counter == 0) {
			mouse_write_cmd(MOUSE_READ_DATA);
			mouse_get_status(&status);
		}



		if ((status & KBD_STAT_OBF) != 0) {

			if((status & (KBD_STAT_PARITY | KBD_STAT_TIMEOUT ))==0) {

				if ((status & KBD_STAT_AUX) != 0) {

					sys_inb(OUT_BUFFER, &test_byte);

					if (byte1_check == 0) { // checks if byte 1 has been read

						if (test_byte & BIT(3)) {
							byte1_check = 1;
							byte1 = test_byte;
							counter++;
						} else {

							tickdelay(micros_to_ticks(DELAY_US));
							continue;
						}

					} else if (byte2_check == 0) {  // checks if byte 2 has been read

						byte2_check=1;
						byte2 = test_byte;
						counter++;

					} else {

						byte3 = test_byte;
						byte1_check=0; byte2_check=0;
						packet[0] = byte1;
						packet[1] = byte2;
						packet[2] = byte3;
						counter = 0;

						if (!((packet[0] == 0x8) && (packet[1] == 0x0) && (packet[2] == 0x0))) { //if movement or mouse press is detected
							packet_count++;
							mouse_print(packet);
						}


					}



				}


			}

		}

		tickdelay(micros_to_ticks(period));

	}

	//2.1 Re-enable mouse interrupts

	kbc_write_cmd_byte(MOUSE_ENB_INT_CMD);

	//2.2 Re-enable stream mode

	mouse_write_cmd(ENB_STREAM);

	return 0;

}


int mouse_test_async(unsigned short idle_time) {

                int ipc_status, irq_set = BIT(mouse_hook_id), irq_timer0_set = BIT(TIMER0_IRQ), r, seconds_passed = 0;

                message msg;
            	unsigned int packet_count = 0;
            	unsigned int packet[3];
            	unsigned int test_byte = 0, byte1, byte2, byte3;
            	int byte1_check = 0, byte2_check = 0;

                //1.Subscribe to mouse and timer interrupts and check if it failed

                if (mouse_subscribe_int() != MOUSE_IRQ) {

                        return 1;

                }
                		kbc_write_byte(MOUSE_ENB);
                		mouse_write_cmd(ENB_STREAM);
                		mouse_write_cmd(ENB_DATA_REPORT);

                if (timer_subscribe_int() != TIMER0_IRQ) {

                        return 1;

                }


                //2. Handlers check time and print scancodes

                while (seconds_passed <= idle_time) {


                       if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {

                        printf("driver_receive failed with: %d", r);

                        continue;

                }

                if (is_ipc_notify(ipc_status)) {

                        switch (_ENDPOINT_P(msg.m_source)) {

                        case HARDWARE:

                                if (msg.NOTIFY_ARG & irq_set) {

                                        test_byte = mouse_handler();

                                        if (byte1_check == 0) { // checks if byte 1 has been read

                                                if (test_byte & BIT(3)) {

                                                        byte1_check = 1;

                                                        byte1 = test_byte;

                                                } else continue;

                                        } else if (byte2_check == 0)
                                        {  // checks if byte 2 has been read

                                                byte2_check=1;

                                                byte2 = test_byte;

                                        							}
                                        else {

                                                byte3 = test_byte;

                                                byte1_check=0; byte2_check=0;

                                                packet[0] = byte1;

                                                packet[1] = byte2;

                                                packet[2] = byte3;

                                                packet_count++;

                                                mouse_print(packet);
                                                counter=0;
                                                seconds_passed = 0;

                                                }

                                        }

                                        if (msg.NOTIFY_ARG & irq_timer0_set) {

                                                if(counter == 0)
                                                	seconds_passed = 0;

                                                if(counter % 60 == 0) {
                                                	//printf("count: %u\n", counter);
                                                	//printf("seconds passed: %u\n", seconds_passed);

                                                        seconds_passed++;

                                                }

                                                timer_int_handler();

                                        }

                                        break;

                                default:

                                        break;

                                }

                        } else { }

                }

                //3.Unsubscribe to mouse and timer interrupts

                if(timer_unsubscribe_int()!=OK) {

                        return 1;

                }

                if(mouse_unsubscribe_int()!= OK) {

                        return 1;

                }

 		mouse_write_cmd(DIS_DATA_REPORT);

                return 0;

}

typedef enum {INIT, DRAW, COMP} state_t;
typedef enum {RDOWN, RUP, HOR_LINE, VERT_LINE} event_t;

short calculate_length_x(unsigned int packet[2], short length_x, int is_mouse_rdown) {

	if(is_mouse_rdown) {

		if (NEGATIVE_X & packet[0]) {
			length_x += (packet[1] -256);
		}
		else {
			length_x +=  packet[1];
		}

	} else
		length_x = 0;

	return length_x;
}

short calculate_length_y(unsigned int packet[2], short length_y, int is_mouse_rdown) {

	if(is_mouse_rdown) {

		if (NEGATIVE_Y & packet[0]) {
			length_y += (packet[2]-256);
		}
		else {
			length_y += packet[2];
		}

	} else
		length_y = 0;

	return length_y;

}


int mouse_test_gesture(short length){

	int ipc_status, irq_set = BIT(mouse_hook_id), r;
	message msg;
	unsigned int packet_count = 0;
	unsigned int packet[3];
	unsigned int test_byte = 0, byte1, byte2, byte3;
	int byte1_check = 0, byte2_check = 0;
	unsigned int flag=0;

	event_t event;
	state_t state = INIT;
	short length_x = 0, length_y = 0;
	int is_mouse_rdown = 0;


	//1.Subscribe to mouse interrupts and check if it failed

	if(mouse_subscribe_int() != MOUSE_IRQ) {
		return 1;
	}

	//1.2 Enable mouse (needed?)

	kbc_write_byte(MOUSE_ENB);

	//1.3 Enable stream mode

	mouse_write_cmd(ENB_STREAM);
	//printf("After enabling stream\n");

	//1.4 Enable data reporting

	mouse_write_cmd(ENB_DATA_REPORT);
	//printf("After enabling data reporting\n");

	//2. Handling the interrupts

	while (!flag) {

		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}

		if (is_ipc_notify(ipc_status)) {

			switch (_ENDPOINT_P(msg.m_source)) {

			case HARDWARE:

				if (msg.NOTIFY_ARG & irq_set) {

					test_byte = mouse_handler();

					if (byte1_check == 0) { // checks if byte 1 has been read

						if (test_byte & BIT(3)) {
							byte1_check = 1;
							byte1 = test_byte;
						} else
							continue;

					} else if (byte2_check == 0) {  // checks if byte 2 has been read

						byte2_check=1;
						byte2 = test_byte;

					} else {

						byte3 = test_byte;
						byte1_check=0; byte2_check=0;
						packet[0] = byte1;
						packet[1] = byte2;
						packet[2] = byte3;
						packet_count++;
						mouse_print(packet);

						}

					// Detecting events

					//right button pressed or not



					if (RDOWN_CONST & packet[0]) {
						is_mouse_rdown = 1;
						if (event != RDOWN)
							event = RDOWN;
					} else {
						is_mouse_rdown = 0;
						event = RUP;
						state = INIT;
					}

					//horizontal line

					length_x = calculate_length_x(packet, length_x, is_mouse_rdown);
					length_y = calculate_length_y(packet, length_y, is_mouse_rdown);
					//printf("length y: %d\n", length_y);
					//printf("length x: %d\n", length_x);

					if (event != RUP) {

						/*
						if (NEGATIVE_X & packet[0]) {
							length_x += (packet[1] -256);
						}
						else {
							length_x +=  packet[1];
						}

						//vertical line

						if (NEGATIVE_Y & packet[0]) {
							length_y += (packet[2]-256);
						}
						else {
							length_y += packet[2];
						}

						 */

						//determining which event took place

						if (length < 0) {

							if ((NEGATIVE_X & packet[0]) == 0) {
								length_x = 0;
								length_y = 0;
							}
							else if (length_x <= length && length_y <= 0)
								event = VERT_LINE;
							else if (length_x > length && length_y <= 0)
								event = HOR_LINE;
							else if (length_y > 0) {
								length_x = 0;
								length_y = 0;
							}

						} else {

							if ((NEGATIVE_X & packet[0]) != 0) {
								length_x = 0;
								length_y = 0;
							}
							else if (length_x >= length && length_y > 0)
								event = VERT_LINE;
							else if (length_x < length && length_y >= 0)
								event = HOR_LINE;
							else if (length_y < 0){
								length_x = 0;
								length_y = 0;
							}

						}
					}

				}

			}
		}

		switch(state) {

		case INIT:
			if (event == RDOWN) {
				state = DRAW;
				length_y = 0; length_x = 0;
			}
		break;
		case DRAW:
			if (event == HOR_LINE)
				;
			else if (event == RUP)
				state = INIT;
			else if (event == VERT_LINE)
				state = COMP;
			break;

		case COMP:
			flag = 1;

		default:
			break;
		}

		//printf("state: %d\n", state);
		//printf("event: %d\n", event);

		if (flag)
			break;

	}


    //3.Unsubscribe to mouse interrupts and check if it failed

    if(mouse_unsubscribe_int()!= 0) {
            return 1;
    }

    //3.1 Disable data report

    mouse_write_cmd(DIS_DATA_REPORT);

    return 0;

}

