#include "MainMenu.h"
#include "Mouse.h"
#include "Graphics.h"
#include "SerialPort.h"


MainMenuState* new_main_menu_state() {

	MainMenuState* main_menu = (MainMenuState*) malloc(sizeof(MainMenuState));

	main_menu->done = 0;

	main_menu->mm_background = loadBitmap("/home/lcom/svn/project/src/images/background_menu.bmp");
	main_menu->player1_button = new_button(165, 507, 484, 540);
	main_menu->player1_button->image_normal = loadBitmap("/home/lcom/svn/project/src/images/player_1.bmp");
	main_menu->player1_button->image_highlighted = loadBitmap("/home/lcom/svn/project/src/images/player_1_h.bmp");
	main_menu->player2_button = new_button(517,859, 484, 540);
	main_menu->player2_button->image_normal = loadBitmap("/home/lcom/svn/project/src/images/player_2.bmp");
	main_menu->player2_button->image_highlighted = loadBitmap("/home/lcom/svn/project/src/images/player_2_h.bmp");
	main_menu->exit_button = new_button(341,683, 550, 606);
	main_menu->exit_button->image_normal = loadBitmap("/home/lcom/svn/project/src/images/exit.bmp");
	main_menu->exit_button->image_highlighted = loadBitmap("/home/lcom/svn/project/src/images/exit_1_h.bmp");

	main_menu->mouse_on_exit = 0;
	main_menu->mouse_on_player1 = 0;
	main_menu->mouse_on_player2 = 0;

	main_menu->player_selected = 0;
	main_menu->other_player_selected = 0;

	main_menu->player_id_sent = 0;

	return main_menu;

}

int update_main_menu_state(MainMenuState* main_menu) {

	int draw = 0;

	//int mouse_on_play_before = main_menu->mouse_on_play;
	int mouse_on_player1_before = main_menu->mouse_on_player1;
	int mouse_on_player2_before = main_menu->mouse_on_player2;
	int mouse_on_exit_before = main_menu->mouse_on_exit;

	//check if any string was received

	if (!is_queue_empty(serial_receiver_queue)) {

		unsigned char* temp;

		serial_receiver_string_from_queue(&temp);
		printf("string received: %s", temp);

		unsigned char compare_1[] = "1";
		unsigned char compare_1_alt[] = "1.1";
		unsigned char compare_2_alt[] = "2.1";
		unsigned char compare_2[] = "2";

		if (strcmp(temp,compare_1) == 0 || strcmp(temp,compare_1_alt) == 0) {
			printf("recognizing the other player\n");
			main_menu->other_player_selected = 1;
		}
		else if (strcmp(temp,compare_2) == 0 || strcmp(temp,compare_2_alt) == 0) {
			printf("recognizing the other player\n");
			main_menu->other_player_selected = 2;
		}

		free(temp);


	}


	//check if mouse is on the player1 button (and if the left key was released)

	if (is_mouse_inside_button(mouse, main_menu->player1_button)) {

		main_menu->mouse_on_player1 = 1;
		draw = 1;

		//change button to highlighted version

		if (mouse->lb_released == 1) {

			if (main_menu->other_player_selected != 1) {

				//send the selected player to the other user

				if(main_menu->player_id_sent != 1) {

				unsigned char to_send[] = "1";

				serial_transmitter_string_to_queue(to_send);
				printf("to send: %s\n", to_send);

				main_menu->player_selected = 1;
				main_menu->player_id_sent = 1;
				}
			}

		}

	} else {

		if (mouse_on_player1_before == 1) {
			draw = 1;
		}

		main_menu->mouse_on_player1 = 0;

	}

	//check if mouse is on the player2 button (and if the left key was released)

	if (is_mouse_inside_button(mouse, main_menu->player2_button)) {

		main_menu->mouse_on_player2 = 1;
		draw = 1;

		//change button to highlighted version

		if (mouse->lb_released == 1) {

			if (main_menu->other_player_selected != 2) {

				//send the selected player to the other user

				if (main_menu->player_id_sent != 1) {

				unsigned char to_send[] = "2";

				serial_transmitter_string_to_queue(to_send);
				printf("to send: %s\n", to_send);

				main_menu->player_selected = 2;
				main_menu->player_id_sent = 1;

				}

			}

		}

	} else {

		if (mouse_on_player2_before == 1) {
			draw = 1;
		}

		main_menu->mouse_on_player2 = 0;

	}

	if (main_menu->player_selected != 0 && main_menu->other_player_selected != 0) {

		if(main_menu->player_selected == 1)
			main_menu->action = PLAY_1;
		else
			main_menu->action = PLAY_2;
		main_menu->done = 1;
	}

	//check if mouse is on the exit button (and if the left key was released)

	if (is_mouse_inside_button(mouse, main_menu->exit_button)) {

		main_menu->mouse_on_exit = 1;
		draw = 1;

		//change button to highlighted version

		if (mouse->lb_released == 1) {
			main_menu->action = EXIT_GAME;
			main_menu->done = 1;
		}

	} else {

		if (mouse_on_exit_before == 1) {
			draw = 1;
		}

		main_menu->mouse_on_exit = 0;

	}

	return draw;

}

void draw_main_menu_state(MainMenuState* main_menu) {

	//everything is working properly, but if we have time, maybe we should implement functions in button.c that do this

	draw_bitmap_to_buffer(main_menu->mm_background, 0, 0, ALIGN_LEFT, background_buffer);

	//draw the button corresponding to the highlighted option

	if (main_menu->mouse_on_player1) {
		draw_bitmap_to_buffer(main_menu->player1_button->image_highlighted,main_menu->player1_button->xi,main_menu->player1_button->yi,ALIGN_LEFT, background_buffer);
		draw_bitmap_to_buffer(main_menu->player2_button->image_normal,main_menu->player2_button->xi,main_menu->player2_button->yi,ALIGN_LEFT, background_buffer);
		draw_bitmap_to_buffer(main_menu->exit_button->image_normal,main_menu->exit_button->xi,main_menu->exit_button->yi,ALIGN_LEFT, background_buffer);
	} else if (main_menu->mouse_on_player2) {
		draw_bitmap_to_buffer(main_menu->player1_button->image_normal,main_menu->player1_button->xi,main_menu->player1_button->yi,ALIGN_LEFT, background_buffer);
		draw_bitmap_to_buffer(main_menu->player2_button->image_highlighted,main_menu->player2_button->xi,main_menu->player2_button->yi,ALIGN_LEFT, background_buffer);
		draw_bitmap_to_buffer(main_menu->exit_button->image_normal,main_menu->exit_button->xi,main_menu->exit_button->yi,ALIGN_LEFT, background_buffer);
	} else if (main_menu->mouse_on_exit) {
		draw_bitmap_to_buffer(main_menu->player1_button->image_normal,main_menu->player1_button->xi,main_menu->player1_button->yi,ALIGN_LEFT, background_buffer);
		draw_bitmap_to_buffer(main_menu->player2_button->image_normal,main_menu->player2_button->xi,main_menu->player2_button->yi,ALIGN_LEFT, background_buffer);
		draw_bitmap_to_buffer(main_menu->exit_button->image_highlighted,main_menu->exit_button->xi,main_menu->exit_button->yi,ALIGN_LEFT, background_buffer);
	} else {
		draw_bitmap_to_buffer(main_menu->player1_button->image_normal,main_menu->player1_button->xi,main_menu->player1_button->yi,ALIGN_LEFT, background_buffer);
		draw_bitmap_to_buffer(main_menu->player2_button->image_normal,main_menu->player2_button->xi,main_menu->player2_button->yi,ALIGN_LEFT, background_buffer);
		draw_bitmap_to_buffer(main_menu->exit_button->image_normal,main_menu->exit_button->xi,main_menu->exit_button->yi,ALIGN_LEFT, background_buffer);
	}

}

void delete_main_menu_state(MainMenuState* main_menu) {

	//delete the buttons and the background

	deleteBitmap(main_menu->mm_background);
	delete_button(main_menu->exit_button);
	delete_button(main_menu->player1_button);
	delete_button(main_menu->player2_button);

	free(main_menu);
}



