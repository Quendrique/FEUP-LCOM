#include "Mouse.h"
#include "Aux.h"

int mouse_hook_id = MOUSE_IRQ;

Mouse* new_mouse() {

	Mouse* mouse = (Mouse*) malloc(sizeof(Mouse));

	mouse->packet_byte1_check = 0;
	mouse->packet_byte2_check = 0;

	mouse->cursor = loadBitmap("/home/lcom/svn/project/src/images/mouse_cursor.bmp");
	mouse->x = 0;
	mouse->y = 0;

	return mouse;
}

Mouse* get_mouse(Mouse* mouse) {

	if (mouse == NULL)
		mouse = new_mouse();

	return mouse;
}

void draw_mouse(Mouse* mouse) {

	//call a function to draw the cursor at the current position of the mouse

	mouse->draw = 0;

	draw_bitmap_to_buffer(mouse->cursor, mouse->x ,mouse->y , ALIGN_CENTER, mouse_buffer);
}

void update_mouse(Mouse* mouse) {

	//IMPORTANT the origin of the referential used here is the top left corner of the screen

	if (mouse->lb_released == 1)
		mouse->lb_released = 0;

	unsigned int test_byte = mouse_handler();

	if (mouse->packet_byte1_check == 0) {

		if (test_byte & BIT(3)) {

			mouse->packet_byte1_check = 1;
			mouse->packet[0] = test_byte;
			return;

		}

	} else if (mouse->packet_byte2_check == 0) {

		mouse->packet_byte2_check = 1;
		mouse->packet[1] = test_byte;
		return;

	} else {

		mouse->packet_byte1_check = 0;
		mouse->packet_byte2_check = 0;
		mouse->packet[2] = test_byte;

		//checks if the position changed

		if (mouse->packet[1] != 0 || mouse->packet[2] != 0) {
			mouse->draw = 1;
		}

		//all packets have been received, time to update the mouse

		mouse->x_sign = (mouse->packet[0] & BIT(4) >> 4);
		mouse->y_sign = (mouse->packet[0] & BIT(5) >> 5);
		int length_x = mouse->packet[1];
		int length_y = mouse->packet[2];


		// if x negative, subtract it to the current value, else add it



		if (mouse->packet[1] & BIT(7))
		{
			length_x = 256 - length_x;
			mouse->x -= length_x;
		}
		else
		{
			mouse->x += length_x;
		}

		if (mouse->packet[2] & BIT(7))
		{
			length_y = 256 - length_y;
			mouse->y += length_y;
		}
		else
		{
			mouse->y -= length_y;
		}


		//should any of the positions go over the screen, set the corresponding variable to the maximum value allowed

		if(mouse->x < 0)
			mouse->x = 0;

		if(mouse->y < 0)
			mouse->y = 0;

		if(mouse->x > SCREEN_X_RESOLUTION) {
			mouse->x = SCREEN_X_RESOLUTION;
		}

		if(mouse->y > SCREEN_Y_RESOLUTION)
		{
			mouse->y = SCREEN_Y_RESOLUTION;
		}

		//checking if any of the buttons are pressed down (and saving the previous value of lb)

		int lb_temp = mouse->lb;

		mouse->lb = (mouse->packet[0] & BIT(0));

		//checking if lb was released

		if (lb_temp == 1 && mouse->lb == 0)
			mouse->lb_released = 1;
		else
			mouse->lb_released = 0;

		return;
	}

}

int is_lb_pressed(Mouse* mouse) {
	return mouse->lb;
}

int was_lb_released(Mouse* mouse) {
	return mouse->lb_released;
}

int is_mouse_inside_button(Mouse* mouse, Button* button) {

	//presupposing the button is a rectangle

	return (mouse->x > button->xi && mouse->x < button->xf
			&& mouse->y > button->yi && mouse->y < button->yf);
}

void delete_mouse(Mouse* mouse) {
	free(mouse);
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

long unsigned int mouse_read_cmd_byte(void) {

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

unsigned int mouse_write_byte(unsigned char cmd) {

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

unsigned int mouse_write_cmd_byte(unsigned char arg) {

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

		if((status & KBD_STAT_IBF) == 0) {
			sys_outb(OUT_BUFFER, cmd);
			sys_inb(OUT_BUFFER, &byte);
		}



		if (byte == NACK || byte == ERROR){
			count++;
			continue;
		} else
			return 0;

		tickdelay(micros_to_ticks(DELAY_US));
	}

	return 1;

}
