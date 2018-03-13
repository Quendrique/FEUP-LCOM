#include "GameOver.h"
#include "Graphics.h"
#include "Mouse.h"
#include "RTC.h"

GameOverState* new_game_over_state_victory() {

	GameOverState* game_over = (GameOverState*) malloc(sizeof(GameOverState));

	game_over->done = 0;
	game_over->draw = 0;

	game_over->numbers[0] = loadBitmap("/home/lcom/svn/project/src/images/0.bmp");
	game_over->numbers[1] = loadBitmap("/home/lcom/svn/project/src/images/1.bmp");
	game_over->numbers[2] = loadBitmap("/home/lcom/svn/project/src/images/2.bmp");
	game_over->numbers[3] = loadBitmap("/home/lcom/svn/project/src/images/3.bmp");
	game_over->numbers[4] = loadBitmap("/home/lcom/svn/project/src/images/4.bmp");
	game_over->numbers[5] = loadBitmap("/home/lcom/svn/project/src/images/5.bmp");
	game_over->numbers[6] = loadBitmap("/home/lcom/svn/project/src/images/6.bmp");
	game_over->numbers[7] = loadBitmap("/home/lcom/svn/project/src/images/7.bmp");
	game_over->numbers[8] = loadBitmap("/home/lcom/svn/project/src/images/8.bmp");
	game_over->numbers[9] = loadBitmap("/home/lcom/svn/project/src/images/9.bmp");

	game_over->background = loadBitmap("/home/lcom/svn/project/src/images/background_player2.bmp");
	game_over->clear_screen = loadBitmap("/home/lcom/svn/project/src/images/white_menu_test.bmp");

	game_over->victory_screen = loadBitmap("/home/lcom/svn/project/src/images/victory_screen.bmp");
	game_over->defeat_screen = loadBitmap("/home/lcom/svn/project/src/images/defeat_screen.bmp");

	game_over->play_button = new_button(200, 500, 550, 650);
	game_over->exit_button = new_button(524, 824, 550, 650);
	game_over->play_button->image_normal = loadBitmap("/home/lcom/svn/project/src/images/play_button_gameover.bmp");
	game_over->play_button->image_highlighted =  loadBitmap("/home/lcom/svn/project/src/images/play_button_gameover_h.bmp");
	game_over->exit_button->image_normal = loadBitmap("/home/lcom/svn/project/src/images/exit_button_gameover.bmp");
	game_over->exit_button->image_highlighted =  loadBitmap("/home/lcom/svn/project/src/images/exit_button_gameover_h.bmp");

	game_over->main_menu_chosen = 0;

	game_over->screen_cleared = 0;

	return game_over;


}
void draw_game_over_state_victory(GameOverState* game_over_state) {

	game_over_state->draw = 0;

	if (game_over_state->screen_cleared == 0) {

		rtc_get_time_and_date();

		unsigned long day_1 = (*day % 10);
		unsigned long day_2 = ((*day % 100) - day_1)/10;

		unsigned long month_1 = (*month % 10);
		unsigned long month_2 = ((*month % 100) - month_1)/10;

		unsigned long year_1 = (*year % 10);
		unsigned long year_2 = ((*year % 100) - year_1)/10;

		draw_bitmap_to_buffer(game_over_state->victory_screen, 0, 0, ALIGN_LEFT, background_buffer);
		draw_bitmap_to_buffer(game_over_state->numbers[day_2],220,475,ALIGN_LEFT,background_buffer);
		draw_bitmap_to_buffer(game_over_state->numbers[day_1],254,475,ALIGN_LEFT,background_buffer);
		draw_bitmap_to_buffer(game_over_state->numbers[month_2],294,475,ALIGN_LEFT,background_buffer);
		draw_bitmap_to_buffer(game_over_state->numbers[month_1],324,475,ALIGN_LEFT,background_buffer);
		draw_bitmap_to_buffer(game_over_state->numbers[year_2],364,475,ALIGN_LEFT,background_buffer);
		draw_bitmap_to_buffer(game_over_state->numbers[year_1],394,475,ALIGN_LEFT,background_buffer);
		game_over_state->screen_cleared = 1;
	}

	if (game_over_state->mouse_on_play_button) {
		draw_bitmap_to_buffer(game_over_state->play_button->image_highlighted, game_over_state->play_button->xi, game_over_state->play_button->yi, ALIGN_LEFT, background_buffer);
		draw_bitmap_to_buffer(game_over_state->exit_button->image_normal, game_over_state->exit_button->xi, game_over_state->exit_button->yi, ALIGN_LEFT, background_buffer);
	} else if (game_over_state->mouse_on_exit_button) {
		draw_bitmap_to_buffer(game_over_state->play_button->image_normal, game_over_state->play_button->xi, game_over_state->play_button->yi, ALIGN_LEFT, background_buffer);
		draw_bitmap_to_buffer(game_over_state->exit_button->image_highlighted, game_over_state->exit_button->xi, game_over_state->exit_button->yi, ALIGN_LEFT, background_buffer);

	} else {
		draw_bitmap_to_buffer(game_over_state->play_button->image_normal, game_over_state->play_button->xi, game_over_state->play_button->yi, ALIGN_LEFT, background_buffer);
		draw_bitmap_to_buffer(game_over_state->exit_button->image_normal, game_over_state->exit_button->xi, game_over_state->exit_button->yi, ALIGN_LEFT, background_buffer);

	}



}
int update_game_over_state_victory(GameOverState* game_over_state) {

	int draw = 0;

	int mouse_on_play_before = game_over_state->mouse_on_play_button;
	int mouse_on_exit_before = game_over_state->mouse_on_exit_button;

	if(is_mouse_inside_button(mouse, game_over_state->play_button)) {

		game_over_state->mouse_on_play_button = 1;
		draw = 1;

		if (mouse->lb_released == 1) {

			game_over_state->main_menu_chosen = 1;
			game_over_state->done = 1;

		}


	} else {

		if (mouse_on_play_before == 1)
			draw = 1;

		game_over_state->mouse_on_play_button = 0;

	}

	if(is_mouse_inside_button(mouse, game_over_state->exit_button)) {

		game_over_state->mouse_on_exit_button = 1;
		draw = 1;

		if (mouse->lb_released == 1) {

			game_over_state->done = 1;

		}


	} else {

		if (mouse_on_exit_before == 1)
			draw = 1;

		game_over_state->mouse_on_exit_button = 0;

	}

	return draw;


}
void delete_game_over_state_victory(GameOverState* game_over_state) {
	deleteBitmap(game_over_state->background);
	deleteBitmap(game_over_state->clear_screen);
	deleteBitmap(game_over_state->play_button->image_normal);
	deleteBitmap(game_over_state->play_button->image_highlighted);
	deleteBitmap(game_over_state->exit_button->image_normal);
	deleteBitmap(game_over_state->exit_button->image_highlighted);
	deleteBitmap(game_over_state->victory_screen);
	deleteBitmap(game_over_state->defeat_screen);
	delete_button(game_over_state->play_button);
	delete_button(game_over_state->exit_button);

	unsigned int i;

	for (i = 0; i < 10; i++)
		deleteBitmap(game_over_state->numbers[i]);

	free(game_over_state);

}

GameOverState* new_game_over_state_defeat() {

	GameOverState* game_over = (GameOverState*) malloc(sizeof(GameOverState));

	game_over->done = 0;
	game_over->draw = 0;

	game_over->numbers[0] = loadBitmap("/home/lcom/svn/project/src/images/0.bmp");
	game_over->numbers[1] = loadBitmap("/home/lcom/svn/project/src/images/1.bmp");
	game_over->numbers[2] = loadBitmap("/home/lcom/svn/project/src/images/2.bmp");
	game_over->numbers[3] = loadBitmap("/home/lcom/svn/project/src/images/3.bmp");
	game_over->numbers[4] = loadBitmap("/home/lcom/svn/project/src/images/4.bmp");
	game_over->numbers[5] = loadBitmap("/home/lcom/svn/project/src/images/5.bmp");
	game_over->numbers[6] = loadBitmap("/home/lcom/svn/project/src/images/6.bmp");
	game_over->numbers[7] = loadBitmap("/home/lcom/svn/project/src/images/7.bmp");
	game_over->numbers[8] = loadBitmap("/home/lcom/svn/project/src/images/8.bmp");
	game_over->numbers[9] = loadBitmap("/home/lcom/svn/project/src/images/9.bmp");

	game_over->background = loadBitmap("/home/lcom/svn/project/src/images/background_player2.bmp");
	game_over->clear_screen = loadBitmap("/home/lcom/svn/project/src/images/white_menu_test.bmp");

	game_over->victory_screen = loadBitmap("/home/lcom/svn/project/src/images/victory_screen.bmp");
	game_over->defeat_screen = loadBitmap("/home/lcom/svn/project/src/images/defeat_screen.bmp");

	game_over->play_button = new_button(200, 500, 550, 650);
	game_over->exit_button = new_button(524, 824, 550, 650);
	game_over->play_button->image_normal = loadBitmap("/home/lcom/svn/project/src/images/play_button_gameover.bmp");
	game_over->play_button->image_highlighted =  loadBitmap("/home/lcom/svn/project/src/images/play_button_gameover_h.bmp");
	game_over->exit_button->image_normal = loadBitmap("/home/lcom/svn/project/src/images/exit_button_gameover.bmp");
	game_over->exit_button->image_highlighted =  loadBitmap("/home/lcom/svn/project/src/images/exit_button_gameover_h.bmp");

	game_over->main_menu_chosen = 0;

	game_over->screen_cleared = 0;

	return game_over;


}
void draw_game_over_state_defeat(GameOverState* game_over_state) {

	game_over_state->draw = 0;

	if (game_over_state->screen_cleared == 0) {

		rtc_get_time_and_date();

		unsigned long day_1 = (*day % 10);
		unsigned long day_2 = ((*day % 100) - day_1)/10;

		//printf("day 1: %d\nday 2: %d\n", day_1, day_2);

		unsigned long month_1 = (*month % 10);
		unsigned long month_2 = ((*month % 100) - month_1)/10;

		//printf("month 1: %d\nmonth 2: %d\n", month_1, month_2);

		unsigned long year_1 = (*year % 10);
		unsigned long year_2 = ((*year % 100) - year_1)/10;

		//printf("year 1: %d\year 2: %d\n", year_1, year_2);

		draw_bitmap_to_buffer(game_over_state->defeat_screen, 0, 0, ALIGN_LEFT, background_buffer);
		draw_bitmap_to_buffer(game_over_state->numbers[day_2],220,475,ALIGN_LEFT,background_buffer);
		draw_bitmap_to_buffer(game_over_state->numbers[day_1],254,475,ALIGN_LEFT,background_buffer);
		draw_bitmap_to_buffer(game_over_state->numbers[month_2],294,475,ALIGN_LEFT,background_buffer);
		draw_bitmap_to_buffer(game_over_state->numbers[month_1],324,475,ALIGN_LEFT,background_buffer);
		draw_bitmap_to_buffer(game_over_state->numbers[year_2],364,475,ALIGN_LEFT,background_buffer);
		draw_bitmap_to_buffer(game_over_state->numbers[year_1],394,475,ALIGN_LEFT,background_buffer);
		game_over_state->screen_cleared = 1;
	}

	if (game_over_state->mouse_on_play_button) {
		draw_bitmap_to_buffer(game_over_state->play_button->image_highlighted, game_over_state->play_button->xi, game_over_state->play_button->yi, ALIGN_LEFT, background_buffer);
		draw_bitmap_to_buffer(game_over_state->exit_button->image_normal, game_over_state->exit_button->xi, game_over_state->exit_button->yi, ALIGN_LEFT, background_buffer);
	} else if (game_over_state->mouse_on_exit_button) {
		draw_bitmap_to_buffer(game_over_state->play_button->image_normal, game_over_state->play_button->xi, game_over_state->play_button->yi, ALIGN_LEFT, background_buffer);
		draw_bitmap_to_buffer(game_over_state->exit_button->image_highlighted, game_over_state->exit_button->xi, game_over_state->exit_button->yi, ALIGN_LEFT, background_buffer);

	} else {
		draw_bitmap_to_buffer(game_over_state->play_button->image_normal, game_over_state->play_button->xi, game_over_state->play_button->yi, ALIGN_LEFT, background_buffer);
		draw_bitmap_to_buffer(game_over_state->exit_button->image_normal, game_over_state->exit_button->xi, game_over_state->exit_button->yi, ALIGN_LEFT, background_buffer);

	}



}
int update_game_over_state_defeat(GameOverState* game_over_state) {

	int draw = 0;

	int mouse_on_play_before = game_over_state->mouse_on_play_button;
	int mouse_on_exit_before = game_over_state->mouse_on_exit_button;

	if(is_mouse_inside_button(mouse, game_over_state->play_button)) {

		game_over_state->mouse_on_play_button = 1;
		draw = 1;

		if (mouse->lb_released == 1) {

			game_over_state->main_menu_chosen = 1;
			game_over_state->done = 1;

		}


	} else {

		if (mouse_on_play_before == 1)
			draw = 1;

		game_over_state->mouse_on_play_button = 0;

	}

	if(is_mouse_inside_button(mouse, game_over_state->exit_button)) {

		game_over_state->mouse_on_exit_button = 1;
		draw = 1;

		if (mouse->lb_released == 1) {

			game_over_state->done = 1;

		}


	} else {

		if (mouse_on_exit_before == 1)
			draw = 1;

		game_over_state->mouse_on_exit_button = 0;

	}

	return draw;


}
void delete_game_over_state_defeat(GameOverState* game_over_state) {
	deleteBitmap(game_over_state->background);
	deleteBitmap(game_over_state->clear_screen);
	deleteBitmap(game_over_state->play_button->image_normal);
	deleteBitmap(game_over_state->play_button->image_highlighted);
	deleteBitmap(game_over_state->exit_button->image_normal);
	deleteBitmap(game_over_state->exit_button->image_highlighted);
	deleteBitmap(game_over_state->victory_screen);
	deleteBitmap(game_over_state->defeat_screen);
	delete_button(game_over_state->play_button);
	delete_button(game_over_state->exit_button);

	unsigned int i;

	for (i = 0; i < 10; i++)
		deleteBitmap(game_over_state->numbers[i]);

	free(game_over_state);

}
