#include <stdio.h>
#include "Bitmap.h"
#include "Graphics.h"
#include "Game.h"
#include "Mouse.h"
#include "Timer.h"
#include "SerialPort.h"

/**
 * \callgraph
 */


int main(int argc, char **argv) {

	sef_startup();

	vg_init(VIDEO_MODE_117);
	initialize_buffers();
	initialize_timer_counter();
	serial_set_conf();

	Game* game = (Game*) start_game();

	while(!game->done) {

		update_game(game);

		if (!game->done) {

			if(game->draw)
				draw_game(game);

			if(mouse->draw) {
				background_to_mouse_buffer();
				draw_mouse(mouse);
			}

			mouse_buffer_to_video_mem();
		}

	}

	delete_game(game);
	delete_buffers();
	vg_exit();

	return 0;

}
