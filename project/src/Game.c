#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/driver.h>
#include <minix/sysutil.h>
#include <minix/ipc.h>
#include <stdio.h>
#include "MainMenu.h"
#include "Game.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "i8042.h"
#include "i8254.h"
#include "Timer.h"
#include "Graphics.h"
#include "Gameplay.h"
#include "GameOver.h"
#include "SerialPort.h"
#include "RTC.h"

Game* start_game() {

	Game* game = (Game*) malloc(sizeof(Game));
	mouse = (Mouse*) new_mouse();
	kbd = (Keyboard*) new_kbd();
	game->IRQ_SET_KBD = BIT(KBD_IRQ);
	game->IRQ_SET_MOUSE = BIT(MOUSE_IRQ);
	game->IRQ_SET_TIMER = BIT(TIMER0_IRQ);
	game->IRQ_SET_SERIAL = BIT(SERIAL_IRQ_COM1);
	game->IRQ_SET_RTC = BIT(RTC_IRQ);

	mouse_write_cmd(ENB_STREAM);
	mouse_write_cmd(ENB_DATA_REPORT);

	mouse_subscribe_int();
	kbd_subscribe_int();
	timer_subscribe_int();
	serial_subscribe_int();
	serial_initialize();
	//rtc_subscribe_int();
	//rtc_enable_int();
	rtc_initialize_global_variables();


	//...

	game->done = 0;
	game->draw = 1;
	game->frame_passed = 0;
	game->player_selected = 0;
	game->victory = 0;

	game->current_state = MAIN_MENU_STATE;
	game->state = new_main_menu_state();

	return game;

}

void update_game(Game* game) {

	int ipc_status, r = 0;
	message msg;

	//receive and register device interruptions

	if (driver_receive(ANY, &msg, &ipc_status) != 0)
		return;

	if (is_ipc_notify(ipc_status)) {
		switch (_ENDPOINT_P(msg.m_source)) {
		case HARDWARE:
			//Mouse interrupt
			if (msg.NOTIFY_ARG & game->IRQ_SET_MOUSE) {
				update_mouse(mouse);
			}
			//Keyboard interrupts
			if (msg.NOTIFY_ARG & game->IRQ_SET_KBD)
				game->kbd_scancode = kbd_int_handler();
			//Timer interrupts
			if (msg.NOTIFY_ARG & game->IRQ_SET_TIMER) {
				game->frame_passed = 1;
				timer_int_handler();
			}
			//Serial interrupts
			if (msg.NOTIFY_ARG & game->IRQ_SET_SERIAL)
				serial_int_handler();
			//RTC interrupts
			/*
			if (msg.NOTIFY_ARG & game->IRQ_SET_RTC) {
				//printf("rtc int handler called\n");
				//rtc_int_handler();
				//printf("after rtc int handler\n");
			}
			*/

			break;
		default:
			break;
		}
	}

	if (game->frame_passed == 1) { //an update will happen every time we switch frames (in this case, 60 times per second)

		game->frame_passed = 0;
		mouse->draw = 1;

		int draw = 0;

		switch (game->current_state) {
		case MAIN_MENU_STATE:
			draw = update_main_menu_state(game->state);
			break;

		case GAMEPLAY_STATE:
			if (game->player_selected == 1)
				draw = update_gameplay_state_player1(game->state);
			else
				draw = update_gameplay_state_player2(game->state);

			break;

		case GAME_OVER_STATE:
			if (game->victory == 1)
				draw = update_game_over_state_victory(game->state);
			else
				draw = update_game_over_state_defeat(game->state);
			break;

		default:
			break;

		}

		if (game->kbd_scancode == ESC_KEY_BREAKCODE)
		{
			game->done = 1;
		}

		update_current_state(game);

		if (game->current_state == GAMEPLAY_STATE) {
			int result = update_kbd(game->kbd_scancode);
			game->kbd_scancode = 0;
			mouse->lb_released = 0;

		}

		if (game->draw == 0)
			game->draw = draw;


	}

}

void update_current_state(Game* game) {

	switch (game->current_state) {

	case MAIN_MENU_STATE:
		if (((MainMenuState*) game->state)->done) {

			int action = ((MainMenuState*) game->state)->action;

			switch (action) {
			case PLAY_1:
				game->player_selected = 1;
				change_state(game, GAMEPLAY_STATE);
				break;
			case PLAY_2:
				game->player_selected = 2;
				change_state(game, GAMEPLAY_STATE);
				break;
			case EXIT_GAME:
				game->done = 1;
				break;

			default:
				break;
			}


		}

		break;

	case GAMEPLAY_STATE:
		if (((GameplayState*) game->state)->done) {

			int action = ((GameplayState*) game->state)->action;

			switch (action) {
			case WON:
				game->victory = 1;
				change_state(game, GAME_OVER_STATE);
				break;
			case LOST:
				game->victory = 2;
				change_state(game, GAME_OVER_STATE);
				break;

			default:
				break;
			}

		}

		break;

	case GAME_OVER_STATE:
		if (((GameOverState*) game->state)->done) {

			if (((GameOverState*) game->state)->main_menu_chosen == 1)
				change_state(game, MAIN_MENU_STATE);
			else
				game->done = 1;
		}
		break;

	default:
		break;

	}

}

void change_state(Game* game, State new_state) {

	//delete current state
	delete_current_state(game);

	//change current state
	game->current_state = new_state;

	//create new state

	switch(game->current_state) {
	case MAIN_MENU_STATE:
		game->state = new_main_menu_state();
		break;

	case GAMEPLAY_STATE:
		if (game->player_selected == 1)
			game->state = new_gameplay_state_player1();
		else
			game->state = new_gameplay_state_player2();
		break;

	case GAME_OVER_STATE:
		if (game->victory == 1)
			game->state = new_game_over_state_victory();
		else
			game->state = new_game_over_state_defeat();
		break;

	default:
		break;
	}

	game->draw = 1;
}

void draw_game(Game* game) {

	//drawing the state

	game->draw = 0;

	switch (game->current_state) {

	case MAIN_MENU_STATE:
		draw_main_menu_state(game->state);
		break;

	case GAMEPLAY_STATE:
		if(game->player_selected == 1)
			draw_gameplay_state_player1(game->state);
		else
			draw_gameplay_state_player2(game->state);
		break;

	case GAME_OVER_STATE:
		if (game->victory == 1)
			draw_game_over_state_victory(game->state);
		else
			draw_game_over_state_defeat(game->state);
		break;

	default:
		break;

	}

	if(mouse->draw == 1) {
		draw_mouse(mouse);
	}


}

void delete_current_state(Game* game) {

	switch (game->current_state) {

	case MAIN_MENU_STATE:
		delete_main_menu_state(game->state);
		break;

	case GAMEPLAY_STATE:
		if (game->player_selected == 1)
			delete_gameplay_state_player1(game->state);
		else
			delete_gameplay_state_player2(game->state);
		break;

	case GAME_OVER_STATE:
		if (game->victory == 1)
			delete_game_over_state_victory(game->state);
		else
			delete_game_over_state_defeat(game->state);
		break;

	default:
		break;

	}

}

void delete_game(Game* game) {

	rtc_delete_global_variables();

	mouse_unsubscribe_int();
	kbd_unsubscribe_int();
	timer_unsubscribe_int();
	serial_unsubscribe_int();
	//rtc_unsubscribe_int();

	free(game);

}
