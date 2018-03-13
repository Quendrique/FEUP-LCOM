#include <stdio.h>
#include "Gameplay.h"
#include "Timer.h"
#include "Keyboard.h"
#include "Puzzle.h"
#include "Mouse.h"
#include "SerialPort.h"

GameplayState* new_gameplay_state_player1() {

	GameplayState* gameplay = (GameplayState*) malloc(sizeof(GameplayState));

	gameplay->puzzle_1 = new_wire_puzzle();
	gameplay->puzzle_2 = new_button_puzzle();
	gameplay->puzzle_3 = new_word_puzzle();
	gameplay->puzzle_4 = new_numbers_puzzle();
	gameplay->puzzle_5 = new_puzzle5_puzzle();
	gameplay->puzzle_6 = new_puzzle6_puzzle();

	gameplay->puzzle_1_done = 0;
	gameplay->puzzle_2_done = 0;
	gameplay->puzzle_3_done = 0;
	gameplay->puzzle_4_done = 0;
	gameplay->puzzle_5_done = 0;
	gameplay->puzzle_6_done = 0;

	gameplay->all_puzzles = 0;

	gameplay->draw = 0;
	gameplay->done = 0;
	gameplay->strike_num = 0;

	gameplay->timer = new_timer();

	gameplay->clear_screen = loadBitmap("/home/lcom/svn/project/src/images/white_menu_test.bmp");
	gameplay->screen_cleared = 0;

	return gameplay;
}

GameplayState* new_gameplay_state_player2() {

	GameplayState* gameplay = (GameplayState*) malloc(sizeof(GameplayState));

	gameplay->background = loadBitmap("/home/lcom/svn/project/src/images/background_pl_2.bmp");

	gameplay->previous_page = new_button(100, 236, 316, 452);
	gameplay->next_page = new_button(802, 938, 316, 452);
	gameplay->previous_page->image_normal = loadBitmap("/home/lcom/svn/project/src/images/previous.bmp");
	gameplay->next_page->image_normal = loadBitmap("/home/lcom/svn/project/src/images/next.bmp");

	gameplay->mouse_on_next_page = 0;
	gameplay->mouse_on_previous_page = 0;

	gameplay->page_viewed = 0;

	gameplay->pages_available[0] = loadBitmap("/home/lcom/svn/project/src/images/manual_page_1.bmp");
	gameplay->pages_available[1] = loadBitmap("/home/lcom/svn/project/src/images/manual_page_2.bmp");
	gameplay->pages_available[2] = loadBitmap("/home/lcom/svn/project/src/images/manual_page_3.bmp");

	gameplay->draw = 0;
	gameplay->done = 0;
	gameplay->timer = new_timer();

	gameplay->clear_screen = loadBitmap("/home/lcom/svn/project/src/images/white_menu_test.bmp");
	gameplay->screen_cleared = 0;

	return gameplay;


}

void draw_gameplay_state_player1(GameplayState* gameplay_state) {

	gameplay_state->draw = 0;

	if (gameplay_state->screen_cleared == 0) {
		draw_bitmap_to_buffer(gameplay_state->clear_screen, 0, 0, ALIGN_LEFT, background_buffer);
		gameplay_state->screen_cleared = 1;
	}

	//check if each puzzle needs drawing first

	if (gameplay_state->puzzle_1->draw == 1) {
		draw_wire_puzzle(gameplay_state->puzzle_1);
	}

	if (gameplay_state->puzzle_2->draw == 1) {
		draw_button_puzzle(gameplay_state->puzzle_2);
	}

	if (gameplay_state->puzzle_3->draw == 1) {
		draw_word_puzzle(gameplay_state->puzzle_3);
	}

	if (gameplay_state->puzzle_4->draw == 1) {
		draw_numbers_puzzle(gameplay_state->puzzle_4);
	}

	if (gameplay_state->puzzle_5->draw == 1) {
		draw_puzzle5_puzzle(gameplay_state->puzzle_5);
	}

	if (gameplay_state->puzzle_6->draw == 1) {
		draw_puzzle6_puzzle(gameplay_state->puzzle_6);
	}

	//draw timer, if need be

	draw_timer(gameplay_state->timer);

}

void draw_gameplay_state_player2(GameplayState* gameplay_state) {

	gameplay_state->draw = 0;

	if (gameplay_state->screen_cleared == 0) {
		draw_bitmap_to_buffer(gameplay_state->background, 0, 0, ALIGN_LEFT, background_buffer);
		gameplay_state->screen_cleared = 1;
	}

	//draw the buttons

	draw_bitmap_to_buffer(gameplay_state->next_page->image_normal,gameplay_state->next_page->xi,gameplay_state->next_page->yi,ALIGN_LEFT, background_buffer);
	draw_bitmap_to_buffer(gameplay_state->previous_page->image_normal,gameplay_state->previous_page->xi,gameplay_state->previous_page->yi,ALIGN_LEFT, background_buffer);

	//draw the manual

	draw_bitmap_to_buffer(gameplay_state->pages_available[gameplay_state->page_viewed], 192, 0, ALIGN_LEFT, background_buffer);

	//draw timer

	if (gameplay_state->timer->draw == 1)
		draw_timer(gameplay_state->timer);

}

int update_gameplay_state_player1(GameplayState* gameplay_state) {

	int draw = 0;

	//check if a second has passed

	if (timer_counter % 60 == 0) {
		update_timer(gameplay_state->timer);

		if (gameplay_state->timer->done == 1) {
			gameplay_state->done = 1;
			gameplay_state->action = LOST;
			unsigned char to_send_defeat_timer[] = "4";
			serial_transmitter_string_to_queue(to_send_defeat_timer);
		}

		gameplay_state->timer->draw = 1;
		draw = 1;
	}

	//call the update function for the puzzles and check if any need redrawing

	if (update_wire_puzzle(gameplay_state->puzzle_1) == 1 || update_button_puzzle(gameplay_state->puzzle_2,1) == 1 ||
			update_word_puzzle(gameplay_state->puzzle_3) == 1 || update_numbers_puzzle(gameplay_state->puzzle_4) == 1) {

		draw = 1;
	}

	if (gameplay_state->puzzle_1->wrong || gameplay_state->puzzle_2->wrong || gameplay_state->puzzle_3->wrong || gameplay_state->puzzle_4->wrong) {

		gameplay_state->strike_num++;

		gameplay_state->puzzle_1->wrong = 0;
		gameplay_state->puzzle_2->wrong = 0;
		gameplay_state->puzzle_3->wrong = 0;
		gameplay_state->puzzle_4->wrong = 0;

		//return;

	}



	//check if all puzzles are done

	if (gameplay_state->puzzle_1->completed && gameplay_state->puzzle_2->completed && gameplay_state->puzzle_3->completed && gameplay_state->puzzle_4->completed) {
		gameplay_state->done = 1;
		gameplay_state->action = WON;

		unsigned char to_send_victory[] = "3";
		serial_transmitter_string_to_queue(to_send_victory);
		gameplay_state->all_puzzles = 1;
	//	return;

	}

	if(gameplay_state->strike_num == 3)
	{
		//draw strikes
		gameplay_state->done = 1;
		gameplay_state->action = LOST;

		unsigned char to_send_defeat[] = "4";
		serial_transmitter_string_to_queue(to_send_defeat);

	}

	return draw;
}

int update_gameplay_state_player2(GameplayState* gameplay_state) {

	int draw = 0;

	//check if a second has passed

	if (timer_counter % 60 == 0) {
		update_timer(gameplay_state->timer);

		if (gameplay_state->timer->done == 1)
			gameplay_state->done = 1;

		gameplay_state->timer->draw = 1;
		draw = 1;
	}

	if (!is_queue_empty(serial_receiver_queue)) {

		unsigned char* temp;

		serial_receiver_string_from_queue(&temp);
		printf("string received: %s", temp);

		unsigned char compare_3[] = "3";
		unsigned char compare_4[] = "4";

		if (strcmp(temp,compare_3) == 0) {
			gameplay_state->action = WON;
			gameplay_state->done = 1;
		}
		else if (strcmp(temp,compare_4) == 0) {
			gameplay_state->action = LOST;
			gameplay_state->done = 1;
		}

		free(temp);


	}

	int mouse_on_next_page_before = gameplay_state->mouse_on_next_page;
	int mouse_on_previous_page_before = gameplay_state->mouse_on_previous_page;

	// check if mouse in on top of any of the buttons (and if the left key was released while on top of them)

	if (is_mouse_inside_button(mouse, gameplay_state->next_page)) {

		gameplay_state->mouse_on_next_page = 1;
		draw = 1;

		if (mouse->lb_released == 1) {

			if (gameplay_state->page_viewed == 2)
				gameplay_state->page_viewed = 0;
			else
				gameplay_state->page_viewed++;

		}



	} else {

		if (mouse_on_next_page_before == 1)
			draw = 1;

		gameplay_state->mouse_on_next_page = 0;

	}

	if (is_mouse_inside_button(mouse, gameplay_state->previous_page)) {

		gameplay_state->mouse_on_previous_page = 1;
		draw = 1;

		if (mouse->lb_released == 1) {

			if (gameplay_state->page_viewed == 0)
				gameplay_state->page_viewed = 2;
			else
				gameplay_state->page_viewed--;


		}



	} else {

		if (mouse_on_previous_page_before == 1)
			draw = 1;

		gameplay_state->mouse_on_previous_page = 0;

	}

	return draw;

}

void delete_gameplay_state_player1(GameplayState* gameplay_state) {

	deleteBitmap(gameplay_state->clear_screen);
	delete_wire_puzzle(gameplay_state->puzzle_1);
	delete_button_puzzle(gameplay_state->puzzle_2);
	delete_word_puzzle(gameplay_state->puzzle_3);
	delete_numbers_puzzle(gameplay_state->puzzle_4);
	delete_puzzle5_puzzle(gameplay_state->puzzle_5);
	delete_puzzle6_puzzle(gameplay_state->puzzle_6);
	delete_timer(gameplay_state->timer);
	free(gameplay_state);

}

void delete_gameplay_state_player2(GameplayState* gameplay_state) {

	deleteBitmap(gameplay_state->clear_screen);
	deleteBitmap(gameplay_state->background);
	deleteBitmap(gameplay_state->next_page->image_normal);
	deleteBitmap(gameplay_state->previous_page->image_normal);
	delete_button(gameplay_state->next_page);
	delete_button(gameplay_state->previous_page);
	delete_timer(gameplay_state->timer);

	unsigned int i;

	for (i = 0; i <= 2; i++) {
		deleteBitmap(gameplay_state->pages_available[i]);
	}

	free(gameplay_state);

}
