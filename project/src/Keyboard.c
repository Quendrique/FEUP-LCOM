#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/driver.h>
#include <minix/sysutil.h>
#include <minix/ipc.h>
#include "i8042.h"
#include "Keyboard.h"
#include "Bitmap.h"
#include "Graphics.h"
#include "SerialPort.h"

int kbd_hook_id = KBD_IRQ;

Keyboard * new_kbd()
{
	Keyboard * kbd = (Keyboard*) malloc(sizeof(Keyboard));

	kbd->x_draw_t = 0;
	kbd->y_draw_t = 668;
	kbd->count = 0;


	kbd->keys[0] = loadBitmap("/home/lcom/svn/project/src/images/A.bmp");
	kbd->keys[1] = loadBitmap("/home/lcom/svn/project/src/images/B.bmp");
	kbd->keys[2] = loadBitmap("/home/lcom/svn/project/src/images/C.bmp");
	kbd->keys[3] = loadBitmap("/home/lcom/svn/project/src/images/D.bmp");
	kbd->keys[4] = loadBitmap("/home/lcom/svn/project/src/images/E.bmp");
	kbd->keys[5] = loadBitmap("/home/lcom/svn/project/src/images/F.bmp");
	kbd->keys[6] = loadBitmap("/home/lcom/svn/project/src/images/G.bmp");
	kbd->keys[7] = loadBitmap("/home/lcom/svn/project/src/images/H.bmp");
	kbd->keys[8] = loadBitmap("/home/lcom/svn/project/src/images/I.bmp");
	kbd->keys[9] = loadBitmap("/home/lcom/svn/project/src/images/J.bmp");
	kbd->keys[10] = loadBitmap("/home/lcom/svn/project/src/images/K.bmp");
	kbd->keys[11] = loadBitmap("/home/lcom/svn/project/src/images/L.bmp");
	kbd->keys[12] = loadBitmap("/home/lcom/svn/project/src/images/M.bmp");
	kbd->keys[13] = loadBitmap("/home/lcom/svn/project/src/images/N.bmp");
	kbd->keys[14] = loadBitmap("/home/lcom/svn/project/src/images/O.bmp");
	kbd->keys[15] = loadBitmap("/home/lcom/svn/project/src/images/P.bmp");
	kbd->keys[16] = loadBitmap("/home/lcom/svn/project/src/images/Q.bmp");
	kbd->keys[17] = loadBitmap("/home/lcom/svn/project/src/images/R.bmp");
	kbd->keys[18] = loadBitmap("/home/lcom/svn/project/src/images/S.bmp");
	kbd->keys[19] = loadBitmap("/home/lcom/svn/project/src/images/T.bmp");
	kbd->keys[20] = loadBitmap("/home/lcom/svn/project/src/images/U.bmp");
	kbd->keys[21] = loadBitmap("/home/lcom/svn/project/src/images/V.bmp");
	kbd->keys[22] = loadBitmap("/home/lcom/svn/project/src/images/W.bmp");
	kbd->keys[23] = loadBitmap("/home/lcom/svn/project/src/images/X.bmp");
	kbd->keys[24] = loadBitmap("/home/lcom/svn/project/src/images/Y.bmp");
	kbd->keys[25] = loadBitmap("/home/lcom/svn/project/src/images/Z.bmp");
	kbd->keys[26] = loadBitmap("/home/lcom/svn/project/src/images/spacebar.bmp");
	kbd->textbox =  loadBitmap("/home/lcom/svn/project/src/images/textbox.bmp");

	kbd->string_to_read = NULL;
	kbd->string_received_is_completed = 0;

	return kbd;

}
int update_kbd(unsigned long int scancode) {

	//draws keys pressed and updates writing coordinates according to last position where a character was written

	int result;

	if (!is_queue_empty(serial_receiver_queue)) {

		if (kbd->string_to_read != NULL)
			free(kbd->string_to_read);

		kbd->string_to_read = malloc(sizeof(unsigned char));

		serial_receiver_string_from_queue(&(kbd->string_to_read));
		printf("string to display: %s\n", kbd->string_to_read);

		unsigned char compare_3[] = "3";
		unsigned char compare_4[] = "4";

		if (strcmp(kbd->string_to_read,compare_3) == 0) {
			result = 3;
		}
		else if (strcmp(kbd->string_to_read,compare_4) == 0) {
			result = 4;
		}

		if (*(kbd->string_to_read) != '\0') {

			if(kbd->string_received_is_completed == 1) {

				kbd->string_received_is_completed = 0;

				while(kbd->string_received_count > 0) {

					kbd->string_to_display[kbd->string_received_count] = 0;
					kbd->string_received_count -- ;

				}
			}
		}

		int string_size = strlen(kbd->string_to_read);
		unsigned int i = 0;
		unsigned int elem = 1;

		while (elem != '\0') {

			unsigned char empty[] = "";

			if (strcmp(kbd->string_to_read, empty) == 0)
				break;

			elem = kbd->string_to_read[i];

			switch(elem) { //97 to 122
			case 97:
				kbd->string_to_display[kbd->string_received_count] = 0;
				break;
			case 98:
				kbd->string_to_display[kbd->string_received_count] = 1;
				break;
			case 99:
				kbd->string_to_display[kbd->string_received_count] = 2;
				break;
			case 100:
				kbd->string_to_display[kbd->string_received_count] = 3;
				break;
			case 101:
				kbd->string_to_display[kbd->string_received_count] = 4;
				break;
			case 102:
				kbd->string_to_display[kbd->string_received_count] = 5;
				break;
			case 103:
				kbd->string_to_display[kbd->string_received_count] = 6;
				break;
			case 104:
				kbd->string_to_display[kbd->string_received_count] = 7;
				break;
			case 105:
				kbd->string_to_display[kbd->string_received_count] = 8;
				break;
			case 106:
				kbd->string_to_display[kbd->string_received_count] = 9;
				break;
			case 107:
				kbd->string_to_display[kbd->string_received_count] = 10;
				break;
			case 108:
				kbd->string_to_display[kbd->string_received_count] = 11;
				break;
			case 109:
				kbd->string_to_display[kbd->string_received_count] = 12;
				break;
			case 110:
				kbd->string_to_display[kbd->string_received_count] = 13;
				break;
			case 111:
				kbd->string_to_display[kbd->string_received_count] = 14;
				break;
			case 112:
				kbd->string_to_display[kbd->string_received_count] = 15;
				break;
			case 113:
				kbd->string_to_display[kbd->string_received_count] = 16;
				break;
			case 114:
				kbd->string_to_display[kbd->string_received_count] = 17;
				break;
			case 115:
				kbd->string_to_display[kbd->string_received_count] = 18;
				break;
			case 116:
				kbd->string_to_display[kbd->string_received_count] = 19;
				break;
			case 117:
				kbd->string_to_display[kbd->string_received_count] = 20;
				 break;
			case 118:
				kbd->string_to_display[kbd->string_received_count] = 21;
				break;
			case 119:
				kbd->string_to_display[kbd->string_received_count] = 22;
				break;
			case 120:
				kbd->string_to_display[kbd->string_received_count] = 23;
				break;
			case 121:
				kbd->string_to_display[kbd->string_received_count] = 24;
				break;
			case 122:
				kbd->string_to_display[kbd->string_received_count] = 25;
				break;
			case 0:
				kbd->string_received_is_completed = 1;
				i--;
				kbd->string_received_count--;
				break;
			default:
				//continue;
				break;
			}

			kbd->string_received_count++;
			i++;


		};


	}


	if (scancode == 0x8e)
	{
		//apaga o ultimo elemento do array
		if(kbd->count > 0)
		{
			kbd->text[kbd->count] = 0;
			kbd->count -- ;

		}
	}

	if (scancode == 0x9c) { //Enter is pressed

		//sends message

		unsigned int i;
		unsigned char character;
		unsigned char* to_send = (unsigned char*) malloc(sizeof(unsigned char));

		for (i = 0; i < kbd->count; i++) {

			to_send = realloc(to_send, (i+1)*sizeof(unsigned char));

			if (to_send == NULL)
				return;

			switch(kbd->text[i]) {
			case 0:
				character = 'a';
				break;
			case 1:
				character = 'b';
				break;
			case 2:
				character = 'c';
				break;
			case 3:
				character = 'd';
				break;
			case 4:
				character = 'e';
				break;
			case 5:
				character = 'f';
				break;
			case 6:
				character = 'g';
				break;
			case 7:
				character = 'h';
				break;
			case 8:
				character = 'i';
				break;
			case 9:
				character = 'j';
				break;
			case 10:
				character = 'k';
				break;
			case 11:
				character = 'l';
				break;
			case 12:
				character = 'm';
				break;
			case 13:
				character = 'n';
				break;
			case 14:
				character = 'o';
				break;
			case 15:
				character = 'p';
				break;
			case 16:
				character = 'q';
				break;
			case 17:
				character = 'r';
				break;
			case 18:
				character = 's';
				break;
			case 19:
				character = 't';
				break;
			case 20:
				character = 'u';
				break;
			case 21:
				character = 'v';
				break;
			case 22:
				character = 'w';
				break;
			case 23:
				character = 'x';
				break;
			case 24:
				character = 'y';
				break;
			case 25:
				character = 'z';
				break;
			default:
				break;

			}

			to_send[i] = character;

		}

		to_send = realloc(to_send, (i+1)*sizeof(unsigned char));
		to_send[i] = '\0';


		printf("string to send: %s\n", to_send);
		serial_transmitter_string_to_queue(to_send);
		free(to_send);

		//apaga os elementos em text[]

		while(kbd->count > 0) {

			kbd->text[kbd->count] = 0;
			kbd->count -- ;

		}


	}

	if (kbd->count <= 74) {

		switch(scancode){
		case 0x9e:
			kbd->text[kbd->count] = 0;
			kbd->count ++;
			break;
		case 0xb0:
			kbd->text[kbd->count] = 1;
			kbd->count ++;
			break;
		case 0xae:
			kbd->text[kbd->count] = 2;
			kbd->count ++;
			break;
		case 0xa0:
			kbd->text[kbd->count] = 3;
			kbd->count ++;
			break;
		case 0x92:
			kbd->text[kbd->count] = 4;
			kbd->count ++;
			break;
		case 0xa1:
			kbd->text[kbd->count] = 5;
			kbd->count ++;
			break;
		case 0xa2:
			kbd->text[kbd->count] = 6;
			kbd->count ++;
			break;
		case 0xa3:
			kbd->text[kbd->count] = 7;
			kbd->count ++;
			break;
		case 0x97:
			kbd->text[kbd->count] = 8;
			kbd->count ++;
			break;
		case 0xa4:
			kbd->text[kbd->count] = 9;
			kbd->count ++;
			break;
		case 0xa5:
			kbd->text[kbd->count] = 10;
			kbd->count ++;
			break;
		case 0xa6:
			kbd->text[kbd->count] = 11;
			kbd->count ++;
			break;
		case 0xb2:
			kbd->text[kbd->count] = 12;
			kbd->count ++;
			break;
		case 0xb1:
			kbd->text[kbd->count] = 13;
			kbd->count ++;
			break;
		case 0x98:
			kbd->text[kbd->count] = 14;
			kbd->count ++;
			break;
		case 0x99:
			kbd->text[kbd->count] = 15;
			kbd->count ++;
			break;
		case 0x90:
			kbd->text[kbd->count] = 16;
			kbd->count ++;
			break;
		case 0x93:
			kbd->text[kbd->count] = 17;
			kbd->count ++;
			break;
		case 0x9f:
			kbd->text[kbd->count] = 18;
			kbd->count ++;
			break;
		case 0x94:
			kbd->text[kbd->count] = 19;
			kbd->count ++;
			break;
		case 0x96:
			kbd->text[kbd->count] = 20;
			kbd->count ++;
			break;
		case 0xaf:
			kbd->text[kbd->count] = 21;
			kbd->count ++;
			break;
		case 0x91:
			kbd->text[kbd->count] = 22;
			kbd->count ++;
			break;
		case 0xad:
			kbd->text[kbd->count] = 23;
			kbd->count ++;
			break;
		case 0x95:
			kbd->text[kbd->count] = 24;
			kbd->count ++;
			break;
		case 0xac:
			kbd->text[kbd->count] = 25;
			kbd->count ++;
			break;
		case 0xb9:
			kbd->text[kbd->count] = 26;
			kbd->count ++;
			break;
		case 0:
			break;


		}
	}

	manamalloc();
	return result;
}



void manamalloc()
{
	unsigned int i;
	unsigned int x_draw_test = kbd->x_draw_t;
	unsigned int y_draw_test = kbd->y_draw_t;

	draw_kbd(kbd);

	for(i = 0 ; i < (kbd->count) ; i++) {

		draw_bitmap_to_buffer(kbd->keys[kbd->text[i]],x_draw_test,y_draw_test,ALIGN_LEFT,background_buffer);

		x_draw_test += 20;

		if(x_draw_test + 20 >= 512)  {

			x_draw_test = 0;
			y_draw_test += 30; //the number of letters is limited now, so there's no need to check if y goes past the screen

		}

	}

	if (kbd->string_received_is_completed == 1) {

		x_draw_test = 512;
		y_draw_test = kbd->y_draw_t;

		for(i = 0 ; i < (kbd->string_received_count) ; i++) {

			draw_bitmap_to_buffer(kbd->keys[kbd->string_to_display[i]],x_draw_test,y_draw_test,ALIGN_LEFT,background_buffer);

			x_draw_test += 20;

			if(x_draw_test + 20 >= 1024)  {

				x_draw_test = 512;
				y_draw_test += 30; //the number of letters is limited now, so there's no need to check if y goes past the screen

			}

		}
	}

}

void draw_kbd(Keyboard * kbd)
{

	draw_bitmap_to_buffer(kbd->textbox,kbd->x_draw_t,kbd->y_draw_t,ALIGN_LEFT,background_buffer);
	return;
}

int kbd_subscribe_int(void) {

	int hook_tmp = kbd_hook_id;

	if (sys_irqsetpolicy(KBD_IRQ, (IRQ_REENABLE | IRQ_EXCLUSIVE), &kbd_hook_id) == OK) {

		if (sys_irqenable(&kbd_hook_id) == OK) {

			return hook_tmp;

		}
	}

	printf("Invalid subscription");
	return 1;

}

int kbd_unsubscribe_int(void) {

	if (sys_irqdisable(&kbd_hook_id) == OK) {

		if (sys_irqrmpolicy(&kbd_hook_id) == OK) {

			return 0;

		}

	}

	printf("Invalid unsubscription");
	return 1;

}

int kbd_get_status(unsigned char *st) {

	unsigned long int tmpStatus;

	if(sys_inb(KBD_STAT_REG, &tmpStatus) == OK) {

		*st= (unsigned char) tmpStatus;
		return 0;

	}

	else {

		printf("Failed to obtain status\n");
		return -1;

	}

}

long int kbd_int_handler() {

	unsigned long int scancode;
	unsigned char status;

	unsigned int count=0;

	while(count < 12) {

		kbd_get_status(&status);

		if(status & KBD_STAT_OBF) {

			if((status & (KBD_STAT_PARITY | KBD_STAT_TIMEOUT ))==0) {

				sys_inb(OUT_BUFFER, &scancode);
				return scancode;

			}

			else
				return -1;

		} else
			count++;

		tickdelay(micros_to_ticks(DELAY_US));

	}

	return 0;

}
