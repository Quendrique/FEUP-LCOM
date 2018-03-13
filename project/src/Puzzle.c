#include "Puzzle.h"
#include "Graphics.h"
#include "Mouse.h"

//WIRE PUZZLE

WirePuzzle* new_wire_puzzle() {

	WirePuzzle* wire_puzzle = (WirePuzzle*) malloc(sizeof(WirePuzzle));

	wire_puzzle->completed = 0;
	wire_puzzle->mouse_on_w1 = 0;
	wire_puzzle->mouse_on_w2 = 0;
	wire_puzzle->mouse_on_w3 = 0;

	//WIRE 1
	wire_puzzle->wire_1 = new_button(60,275,50,80);
	wire_puzzle->wire_1->image_normal = loadBitmap("/home/lcom/svn/project/src/images/wire_red.bmp");
	wire_puzzle->wire_1->image_highlighted = loadBitmap("/home/lcom/svn/project/src/images/wire_red_cut.bmp");

	//WIRE 2
	wire_puzzle->wire_2 = new_button(60,275,120,150);
	wire_puzzle->wire_2->image_normal = loadBitmap("/home/lcom/svn/project/src/images/wire_green.bmp");
	wire_puzzle->wire_2->image_highlighted = loadBitmap("/home/lcom/svn/project/src/images/wire_green_cut.bmp");

	//WIRE 3

	wire_puzzle->wire_3 = new_button(60,275,260,290);
	wire_puzzle->wire_3->image_normal = loadBitmap("/home/lcom/svn/project/src/images/wire_orange.bmp");
	wire_puzzle->wire_3->image_highlighted = loadBitmap("/home/lcom/svn/project/src/images/wire_orange_cut.bmp");


	wire_puzzle->draw = 1;
	wire_puzzle->wrong = 0;
	wire_puzzle->test = loadBitmap("/home/lcom/svn/project/src/images/wire_puzzle.bmp");
	wire_puzzle->led = loadBitmap("/home/lcom/svn/project/src/images/not_done.bmp");
	return wire_puzzle;
}

int update_wire_puzzle(WirePuzzle* puzzle) {

	if (puzzle->wrong == 1)
		puzzle->wrong = 0;

	int draw = 0;
	if (puzzle->completed == 1)
	{
		return draw;
	}
	int mouse_on_w1_before = puzzle->mouse_on_w1;
	int mouse_on_w2_before = puzzle->mouse_on_w2;
	int mouse_on_w3_before = puzzle->mouse_on_w3;



	//check if mouse is on the  button 1 (and if the left key was released)

	if (is_mouse_inside_button(mouse, puzzle->wire_1)) {

		puzzle->mouse_on_w1 = 1;
		draw = 1;

		//change button to highlighted version

		if (mouse->lb_released == 1) {
			puzzle->wire_1->image_normal = loadBitmap("/home/lcom/svn/project/src/images/wire_red_cut.bmp");
			puzzle->completed = 1;
			puzzle->led = loadBitmap("/home/lcom/svn/project/src/images/done.bmp");
		}

	} else {

		if (mouse_on_w1_before == 1) {
			draw = 1;
		}

		puzzle->mouse_on_w1 = 0;

	}

	//check if mouse is on the  button 2 (and if the left key was released)

	if (is_mouse_inside_button(mouse, puzzle->wire_2)) {

		puzzle->mouse_on_w2 = 1;
		draw = 1;

		//change button to highlighted version

		if (mouse->lb_released == 1) {
			puzzle->wire_2->image_normal = loadBitmap("/home/lcom/svn/project/src/images/wire_green_cut.bmp");

			puzzle->wrong = 1;
			puzzle->led = loadBitmap("/home/lcom/svn/project/src/images/bad_done.bmp");
		}

	} else {

		if (mouse_on_w2_before == 1) {
			draw = 1;
		}

		puzzle->mouse_on_w2 = 0;

	}

	//check if mouse is on the  button 3 (and if the left key was released)

	if (is_mouse_inside_button(mouse, puzzle->wire_3)) {

		puzzle->mouse_on_w3 = 1;
		draw = 1;

		//change button to highlighted version

		if (mouse->lb_released == 1) {

			puzzle->wire_3->image_normal = loadBitmap("/home/lcom/svn/project/src/images/wire_orange_cut.bmp");

			puzzle->wrong = 1;
			puzzle->led = loadBitmap("/home/lcom/svn/project/src/images/bad_done.bmp");
		}

	} else {

		if (mouse_on_w3_before == 1) {
			draw = 1;
		}

		puzzle->mouse_on_w3 = 0;

	}

	return draw;


}

void draw_wire_puzzle(WirePuzzle* puzzle) {



	draw_bitmap_to_buffer(puzzle->test, 0, 0, ALIGN_LEFT, background_buffer);
	draw_bitmap_to_buffer(puzzle->led, 693,409,ALIGN_LEFT,background_buffer);



	if (puzzle->mouse_on_w1) {

		draw_bitmap_to_buffer(puzzle->wire_1->image_highlighted, puzzle->wire_1->xi, puzzle->wire_1->yi, ALIGN_LEFT, background_buffer);

		draw_bitmap_to_buffer(puzzle->wire_2->image_normal,puzzle->wire_2->xi,puzzle->wire_2->yi,ALIGN_LEFT, background_buffer);

		draw_bitmap_to_buffer(puzzle->wire_3->image_normal,puzzle->wire_3->xi,puzzle->wire_3->yi,ALIGN_LEFT, background_buffer);
	} else if (puzzle->mouse_on_w2) {

		draw_bitmap_to_buffer(puzzle->wire_1->image_normal, puzzle->wire_1->xi, puzzle->wire_1->yi, ALIGN_LEFT, background_buffer);

		draw_bitmap_to_buffer(puzzle->wire_2->image_highlighted,puzzle->wire_2->xi,puzzle->wire_2->yi,ALIGN_LEFT, background_buffer);

		draw_bitmap_to_buffer(puzzle->wire_3->image_normal,puzzle->wire_3->xi,puzzle->wire_3->yi,ALIGN_LEFT, background_buffer);
	} else if (puzzle->mouse_on_w3) {

		draw_bitmap_to_buffer(puzzle->wire_1->image_normal, puzzle->wire_1->xi, puzzle->wire_1->yi, ALIGN_LEFT, background_buffer);

		draw_bitmap_to_buffer(puzzle->wire_2->image_normal,puzzle->wire_2->xi,puzzle->wire_2->yi,ALIGN_LEFT, background_buffer);

		draw_bitmap_to_buffer(puzzle->wire_3->image_highlighted,puzzle->wire_3->xi,puzzle->wire_3->yi,ALIGN_LEFT, background_buffer);
	}
	else {

		draw_bitmap_to_buffer(puzzle->wire_1->image_normal, puzzle->wire_1->xi, puzzle->wire_1->yi, ALIGN_LEFT, background_buffer);
		draw_bitmap_to_buffer(puzzle->wire_2->image_normal,puzzle->wire_2->xi,puzzle->wire_2->yi,ALIGN_LEFT, background_buffer);
		draw_bitmap_to_buffer(puzzle->wire_3->image_normal,puzzle->wire_3->xi,puzzle->wire_3->yi,ALIGN_LEFT, background_buffer);
	}

}

void delete_wire_puzzle(WirePuzzle* puzzle) {

	deleteBitmap(puzzle->test);
	delete_button(puzzle->wire_1);
	delete_button(puzzle->wire_2);
	delete_button(puzzle->wire_3);
	deleteBitmap(puzzle->led);
	free(puzzle);

}

//BUTTON PUZZLE

ButtonPuzzle* new_button_puzzle() {

	ButtonPuzzle* button_puzzle = (ButtonPuzzle*) malloc(sizeof(ButtonPuzzle));

	button_puzzle->completed = 0;
	button_puzzle->mouse_on_b1 = 0;
	button_puzzle->mouse_on_b2 = 0;
	button_puzzle->mouse_on_b3 = 0;


	//BUTTON 1
	button_puzzle->button_1 = new_button (469,537,15,83);
	button_puzzle->button_1->image_normal = loadBitmap("home/lcom/svn/project/src/images/green.bmp");
	button_puzzle->button_1->image_highlighted = loadBitmap("home/lcom/svn/project/src/images/green_h.bmp");


	//BUTTON 2
	button_puzzle->button_2 = new_button (469,537,113,181);
	button_puzzle->button_2->image_normal = loadBitmap("home/lcom/svn/project/src/images/red.bmp");
	button_puzzle->button_2->image_highlighted = loadBitmap("home/lcom/svn/project/src/images/red_h.bmp");

	//BUTTON 3
	button_puzzle->button_3 = new_button (469,537,211,279);
	button_puzzle->button_3->image_normal = loadBitmap("home/lcom/svn/project/src/images/blue.bmp");
	button_puzzle->button_3->image_highlighted = loadBitmap("home/lcom/svn/project/src/images/blue_h.bmp");

	button_puzzle->draw = 1;
	button_puzzle->wrong =0 ;
	button_puzzle->test = loadBitmap("/home/lcom/svn/project/src/images/puzzle.bmp");
	button_puzzle->led = loadBitmap("/home/lcom/svn/project/src/images/not_done.bmp");


	return button_puzzle;

}

int update_button_puzzle(ButtonPuzzle* puzzle, int solution) {

	if (puzzle->wrong == 1)
		puzzle->wrong = 0;

	int draw = 0;
	if (puzzle->completed == 1)
	{
		return draw;
	}
	int mouse_on_b1_before = puzzle->mouse_on_b1;
	int mouse_on_b2_before = puzzle->mouse_on_b2;
	int mouse_on_b3_before = puzzle->mouse_on_b3;

	switch(solution){
	case 0:
		if (is_mouse_inside_button(mouse, puzzle->button_1)) {

			puzzle->mouse_on_b1 = 1;
			draw = 1;

			//change button to highlighted version

			if (mouse->lb_released == 1) {

				puzzle->wrong = 1;
				puzzle->led = loadBitmap("/home/lcom/svn/project/src/images/bad_done.bmp");

			}

		} else {

			if (mouse_on_b1_before == 1) {
				draw = 1;
			}

			puzzle->mouse_on_b1 = 0;

		}

		//check if mouse is on the  button 2 (and if the left key was released)

		if (is_mouse_inside_button(mouse, puzzle->button_2)) {

			puzzle->mouse_on_b2 = 1;
			draw = 1;

			//change button to highlighted version

			if (mouse->lb_released == 1) {

				puzzle->wrong = 1;
				puzzle->led = loadBitmap("/home/lcom/svn/project/src/images/bad_done.bmp");
			}

		} else {

			if (mouse_on_b2_before == 1) {
				draw = 1;
			}

			puzzle->mouse_on_b2 = 0;

		}

		//check if mouse is on the  button 3 (and if the left key was released)

		if (is_mouse_inside_button(mouse, puzzle->button_3)) {

			puzzle->mouse_on_b3 = 1;
			draw = 1;

			//change button to highlighted version

			if (mouse->lb_released == 1) {

				puzzle->completed = 1;
				puzzle->led = loadBitmap("/home/lcom/svn/project/src/images/done.bmp");			}

		} else {

			if (mouse_on_b3_before == 1) {
				draw = 1;
			}

			puzzle->mouse_on_b3 = 0;

		}
		break;




	case 1:
		if (is_mouse_inside_button(mouse, puzzle->button_1)) {

			puzzle->mouse_on_b1 = 1;
			draw = 1;

			//change button to highlighted version

			if (mouse->lb_released == 1) {

				puzzle->wrong = 1;
				puzzle->led = loadBitmap("/home/lcom/svn/project/src/images/bad_done.bmp");

			}

		} else {

			if (mouse_on_b1_before == 1) {
				draw = 1;
			}

			puzzle->mouse_on_b1 = 0;

		}

		//check if mouse is on the  button 2 (and if the left key was released)

		if (is_mouse_inside_button(mouse, puzzle->button_2)) {

			puzzle->mouse_on_b2 = 1;
			draw = 1;

			//change button to highlighted version

			if (mouse->lb_released == 1) {

				puzzle->completed = 1;
				puzzle->led = loadBitmap("/home/lcom/svn/project/src/images/done.bmp");

			}

		} else {

			if (mouse_on_b2_before == 1) {
				draw = 1;
			}

			puzzle->mouse_on_b2 = 0;

		}

		//check if mouse is on the  button 3 (and if the left key was released)

		if (is_mouse_inside_button(mouse, puzzle->button_3)) {

			puzzle->mouse_on_b3 = 1;
			draw = 1;

			//change button to highlighted version

			if (mouse->lb_released == 1) {

				puzzle->wrong = 1;
				puzzle->led = loadBitmap("/home/lcom/svn/project/src/images/bad_done.bmp");
			}

		} else {

			if (mouse_on_b3_before == 1) {
				draw = 1;
			}

			puzzle->mouse_on_b3 = 0;

		}
		break;







	case 2:
		if (is_mouse_inside_button(mouse, puzzle->button_1)) {

			puzzle->mouse_on_b1 = 1;
			draw = 1;

			//change button to highlighted version

			if (mouse->lb_released == 1) {

				puzzle->completed = 1;
				puzzle->led = loadBitmap("/home/lcom/svn/project/src/images/done.bmp");

			}

		} else {

			if (mouse_on_b1_before == 1) {
				draw = 1;
			}

			puzzle->mouse_on_b1 = 0;

		}

		//check if mouse is on the  button 2 (and if the left key was released)

		if (is_mouse_inside_button(mouse, puzzle->button_2)) {

			puzzle->mouse_on_b2 = 1;
			draw = 1;

			//change button to highlighted version

			if (mouse->lb_released == 1) {

				puzzle->wrong = 1;
				puzzle->led = loadBitmap("/home/lcom/svn/project/src/images/bad_done.bmp");

			}

		} else {

			if (mouse_on_b2_before == 1) {
				draw = 1;
			}

			puzzle->mouse_on_b2 = 0;

		}

		//check if mouse is on the  button 3 (and if the left key was released)

		if (is_mouse_inside_button(mouse, puzzle->button_3)) {

			puzzle->mouse_on_b3 = 1;
			draw = 1;

			//change button to highlighted version

			if (mouse->lb_released == 1) {

				puzzle->wrong = 1;
				puzzle->led = loadBitmap("/home/lcom/svn/project/src/images/bad_done.bmp");
			}

		} else {

			if (mouse_on_b3_before == 1) {
				draw = 1;
			}

			puzzle->mouse_on_b3 = 0;

		}
		break;

	}

	return draw;

}

void draw_button_puzzle(ButtonPuzzle* puzzle) {



	draw_bitmap_to_buffer(puzzle->test, 341, 0, ALIGN_LEFT, background_buffer);
	draw_bitmap_to_buffer(puzzle->led,791,409,ALIGN_LEFT,background_buffer);

	if (puzzle->mouse_on_b1) {
		draw_bitmap_to_buffer(puzzle->button_1->image_highlighted,puzzle->button_1->xi,puzzle->button_1->yi,ALIGN_LEFT, background_buffer);
		draw_bitmap_to_buffer(puzzle->button_2->image_normal,puzzle->button_2->xi,puzzle->button_2->yi,ALIGN_LEFT, background_buffer);
		draw_bitmap_to_buffer(puzzle->button_3->image_normal,puzzle->button_3->xi,puzzle->button_3->yi,ALIGN_LEFT, background_buffer);
	} else if (puzzle->mouse_on_b2) {
		draw_bitmap_to_buffer(puzzle->button_1->image_normal,puzzle->button_1->xi,puzzle->button_1->yi,ALIGN_LEFT, background_buffer);
		draw_bitmap_to_buffer(puzzle->button_2->image_highlighted,puzzle->button_2->xi,puzzle->button_2->yi,ALIGN_LEFT, background_buffer);
		draw_bitmap_to_buffer(puzzle->button_3->image_normal,puzzle->button_3->xi,puzzle->button_3->yi,ALIGN_LEFT, background_buffer);

	} else if (puzzle->mouse_on_b3) {
		draw_bitmap_to_buffer(puzzle->button_1->image_normal,puzzle->button_1->xi,puzzle->button_1->yi,ALIGN_LEFT, background_buffer);
		draw_bitmap_to_buffer(puzzle->button_2->image_normal,puzzle->button_2->xi,puzzle->button_2->yi,ALIGN_LEFT, background_buffer);
		draw_bitmap_to_buffer(puzzle->button_3->image_highlighted,puzzle->button_3->xi,puzzle->button_3->yi,ALIGN_LEFT, background_buffer);

	}
	else {

		draw_bitmap_to_buffer(puzzle->button_1->image_normal,puzzle->button_1->xi,puzzle->button_1->yi,ALIGN_LEFT, background_buffer);
		draw_bitmap_to_buffer(puzzle->button_2->image_normal,puzzle->button_2->xi,puzzle->button_2->yi,ALIGN_LEFT, background_buffer);
		draw_bitmap_to_buffer(puzzle->button_3->image_normal,puzzle->button_3->xi,puzzle->button_3->yi,ALIGN_LEFT, background_buffer);
	}


}

void delete_button_puzzle(ButtonPuzzle* puzzle) {

	deleteBitmap(puzzle->test);
	delete_button(puzzle->button_1);
	delete_button(puzzle->button_2);
	delete_button(puzzle->button_3);
	deleteBitmap(puzzle->led);
	free(puzzle);

}

//WORD PUZZLE

WordPuzzle* new_word_puzzle() {

	WordPuzzle* word_puzzle = (WordPuzzle*) malloc(sizeof(WordPuzzle));

	word_puzzle->completed = 0;


	word_puzzle->mouse_on_b1 = 0;
	word_puzzle->mouse_on_b2 = 0;
	word_puzzle->mouse_on_b3 = 0;
	word_puzzle->mouse_on_b4 = 0;
	word_puzzle->mouse_on_b5 = 0;
	word_puzzle->mouse_on_b6 = 0;



	//BUTTON 1
	word_puzzle->button_1 = new_button (714,794,143,175);
	word_puzzle->button_1->image_normal = loadBitmap("home/lcom/svn/project/src/images/no.bmp");
	word_puzzle->button_1->image_highlighted = loadBitmap("home/lcom/svn/project/src/images/no_h.bmp");


	//BUTTON 2
	word_puzzle->button_2 = new_button (854,934,143,175);
	word_puzzle->button_2->image_normal = loadBitmap("home/lcom/svn/project/src/images/okay.bmp");
	word_puzzle->button_2->image_highlighted = loadBitmap("home/lcom/svn/project/src/images/okay_h.bmp");

	//BUTTON 3
	word_puzzle->button_3 = new_button (714,794,207,239);
	word_puzzle->button_3->image_normal = loadBitmap("home/lcom/svn/project/src/images/left.bmp");
	word_puzzle->button_3->image_highlighted = loadBitmap("home/lcom/svn/project/src/images/left_h.bmp");

	//BUTTON 4
	word_puzzle->button_4 = new_button (854,934,207,239);
	word_puzzle->button_4->image_normal = loadBitmap("home/lcom/svn/project/src/images/wait.bmp");
	word_puzzle->button_4->image_highlighted = loadBitmap("home/lcom/svn/project/src/images/wait_h.bmp");

	//BUTTON 5
	word_puzzle->button_5 = new_button (714,794,271,303);
	word_puzzle->button_5->image_normal = loadBitmap("home/lcom/svn/project/src/images/blank.bmp");
	word_puzzle->button_5->image_highlighted = loadBitmap("home/lcom/svn/project/src/images/blank_h.bmp");

	//BUTTON 6
	word_puzzle->button_6 = new_button (854,934,271,303);
	word_puzzle->button_6->image_normal = loadBitmap("home/lcom/svn/project/src/images/right.bmp");
	word_puzzle->button_6->image_highlighted = loadBitmap("home/lcom/svn/project/src/images/right_h.bmp");





	word_puzzle->draw = 1;
	word_puzzle->wrong = 0;
	word_puzzle->test = loadBitmap("/home/lcom/svn/project/src/images/puzzle.bmp");
	word_puzzle->led = loadBitmap("/home/lcom/svn/project/src/images/not_done.bmp");
	//word_puzzle->hint = loadBitmap("home/lcom/svn/project/src/images/yes.bmp");

	return word_puzzle;

}

int update_word_puzzle(WordPuzzle* puzzle) {

	//SOLUTION WILL BE PRESSING BUTTON 6

	if (puzzle->wrong == 1)
		puzzle->wrong = 0;

		int draw = 0;
		if (puzzle->completed == 1)
		{
			return draw;
		}

		int mouse_on_b1_before = puzzle->mouse_on_b1;
		int mouse_on_b2_before = puzzle->mouse_on_b2;
		int mouse_on_b3_before = puzzle->mouse_on_b3;
		int mouse_on_b4_before = puzzle->mouse_on_b4;
		int mouse_on_b5_before = puzzle->mouse_on_b5;
		int mouse_on_b6_before = puzzle->mouse_on_b6;


		//check if mouse is on the  button 1 (and if the left key was released)

		if (is_mouse_inside_button(mouse, puzzle->button_1)) {

			puzzle->mouse_on_b1 = 1;
			draw = 1;

			//change button to highlighted version

			if (mouse->lb_released == 1) {


				puzzle->wrong = 1;
				puzzle->led = loadBitmap("/home/lcom/svn/project/src/images/bad_done.bmp");
			}

		} else {

			if (mouse_on_b1_before == 1) {
				draw = 1;
			}

			puzzle->mouse_on_b1 = 0;

		}

		//check if mouse is on the  button 2 (and if the left key was released)

		if (is_mouse_inside_button(mouse, puzzle->button_2)) {

			puzzle->mouse_on_b2 = 1;
			draw = 1;

			//change button to highlighted version

			if (mouse->lb_released == 1) {


				puzzle->wrong = 1;
				puzzle->led = loadBitmap("/home/lcom/svn/project/src/images/bad_done.bmp");		}

		} else {

			if (mouse_on_b2_before == 1) {
				draw = 1;
			}

			puzzle->mouse_on_b2 = 0;

		}

		//check if mouse is on the  button 3 (and if the left key was released)

		if (is_mouse_inside_button(mouse, puzzle->button_3)) {

			puzzle->mouse_on_b3 = 1;
			draw = 1;

			//change button to highlighted version

			if (mouse->lb_released == 1) {

				puzzle->wrong = 1;
				puzzle->led = loadBitmap("/home/lcom/svn/project/src/images/bad_done.bmp");
			}

		} else {

			if (mouse_on_b3_before == 1) {
				draw = 1;
			}

			puzzle->mouse_on_b3 = 0;

		}

		//check if mouse is on the  button 4 (and if the left key was released)

		if (is_mouse_inside_button(mouse, puzzle->button_4)) {

			puzzle->mouse_on_b4 = 1;
			draw = 1;

			//change button to highlighted version

			if (mouse->lb_released == 1) {


				puzzle->wrong = 1;
				puzzle->led = loadBitmap("/home/lcom/svn/project/src/images/bad_done.bmp");
			}

		} else {

			if (mouse_on_b4_before == 1) {
				draw = 1;
			}

			puzzle->mouse_on_b4 = 0;

		}

		//check if mouse is on the  button 5 (and if the left key was released)

		if (is_mouse_inside_button(mouse, puzzle->button_5)) {

			puzzle->mouse_on_b5 = 1;
			draw = 1;

			//change button to highlighted version

			if (mouse->lb_released == 1) {


				puzzle->wrong = 1;
				puzzle->led = loadBitmap("/home/lcom/svn/project/src/images/bad_done.bmp");
			}

		} else {

			if (mouse_on_b5_before == 1) {
				draw = 1;
			}

			puzzle->mouse_on_b5 = 0;

		}

		//check if mouse is on the  button 6 (and if the left key was released)

		if (is_mouse_inside_button(mouse, puzzle->button_6)) {

			puzzle->mouse_on_b6 = 1;
			draw = 1;

			//change button to highlighted version

			if (mouse->lb_released == 1) {


				puzzle->completed = 1;
				puzzle->led = loadBitmap("/home/lcom/svn/project/src/images/done.bmp");
			}

		} else {

			if (mouse_on_b6_before == 1) {
				draw = 1;
			}

			puzzle->mouse_on_b6 = 0;

		}

		return draw;

}

void draw_word_puzzle(WordPuzzle* puzzle) {



	draw_bitmap_to_buffer(puzzle->test,684,0, ALIGN_LEFT, background_buffer);
	draw_bitmap_to_buffer(puzzle->led,693,507,ALIGN_LEFT,background_buffer);
	//draw_bitmap_to_buffer(puzzle->hint,775,10,ALIGN_LEFT,background_buffer);


	if (puzzle->mouse_on_b1) {
		draw_bitmap_to_buffer(puzzle->button_1->image_highlighted,puzzle->button_1->xi,puzzle->button_1->yi,ALIGN_LEFT, background_buffer);
		draw_bitmap_to_buffer(puzzle->button_2->image_normal,puzzle->button_2->xi,puzzle->button_2->yi,ALIGN_LEFT, background_buffer);
		draw_bitmap_to_buffer(puzzle->button_3->image_normal,puzzle->button_3->xi,puzzle->button_3->yi,ALIGN_LEFT, background_buffer);
		draw_bitmap_to_buffer(puzzle->button_4->image_normal,puzzle->button_4->xi,puzzle->button_4->yi,ALIGN_LEFT, background_buffer);
		draw_bitmap_to_buffer(puzzle->button_5->image_normal,puzzle->button_5->xi,puzzle->button_5->yi,ALIGN_LEFT, background_buffer);
		draw_bitmap_to_buffer(puzzle->button_6->image_normal,puzzle->button_6->xi,puzzle->button_6->yi,ALIGN_LEFT, background_buffer);
	} else if (puzzle->mouse_on_b2) {
		draw_bitmap_to_buffer(puzzle->button_1->image_normal,puzzle->button_1->xi,puzzle->button_1->yi,ALIGN_LEFT, background_buffer);
		draw_bitmap_to_buffer(puzzle->button_2->image_highlighted,puzzle->button_2->xi,puzzle->button_2->yi,ALIGN_LEFT, background_buffer);
		draw_bitmap_to_buffer(puzzle->button_3->image_normal,puzzle->button_3->xi,puzzle->button_3->yi,ALIGN_LEFT, background_buffer);
		draw_bitmap_to_buffer(puzzle->button_4->image_normal,puzzle->button_4->xi,puzzle->button_4->yi,ALIGN_LEFT, background_buffer);
		draw_bitmap_to_buffer(puzzle->button_5->image_normal,puzzle->button_5->xi,puzzle->button_5->yi,ALIGN_LEFT, background_buffer);
		draw_bitmap_to_buffer(puzzle->button_6->image_normal,puzzle->button_6->xi,puzzle->button_6->yi,ALIGN_LEFT, background_buffer);

	} else if (puzzle->mouse_on_b3) {
		draw_bitmap_to_buffer(puzzle->button_1->image_normal,puzzle->button_1->xi,puzzle->button_1->yi,ALIGN_LEFT, background_buffer);
		draw_bitmap_to_buffer(puzzle->button_2->image_normal,puzzle->button_2->xi,puzzle->button_2->yi,ALIGN_LEFT, background_buffer);
		draw_bitmap_to_buffer(puzzle->button_3->image_highlighted,puzzle->button_3->xi,puzzle->button_3->yi,ALIGN_LEFT, background_buffer);
		draw_bitmap_to_buffer(puzzle->button_4->image_normal,puzzle->button_4->xi,puzzle->button_4->yi,ALIGN_LEFT, background_buffer);
		draw_bitmap_to_buffer(puzzle->button_5->image_normal,puzzle->button_5->xi,puzzle->button_5->yi,ALIGN_LEFT, background_buffer);
		draw_bitmap_to_buffer(puzzle->button_6->image_normal,puzzle->button_6->xi,puzzle->button_6->yi,ALIGN_LEFT, background_buffer);

	} else if (puzzle->mouse_on_b4) {
		draw_bitmap_to_buffer(puzzle->button_1->image_normal,puzzle->button_1->xi,puzzle->button_1->yi,ALIGN_LEFT, background_buffer);
		draw_bitmap_to_buffer(puzzle->button_2->image_normal,puzzle->button_2->xi,puzzle->button_2->yi,ALIGN_LEFT, background_buffer);
		draw_bitmap_to_buffer(puzzle->button_3->image_normal,puzzle->button_3->xi,puzzle->button_3->yi,ALIGN_LEFT, background_buffer);
		draw_bitmap_to_buffer(puzzle->button_4->image_highlighted,puzzle->button_4->xi,puzzle->button_4->yi,ALIGN_LEFT, background_buffer);
		draw_bitmap_to_buffer(puzzle->button_5->image_normal,puzzle->button_5->xi,puzzle->button_5->yi,ALIGN_LEFT, background_buffer);
		draw_bitmap_to_buffer(puzzle->button_6->image_normal,puzzle->button_6->xi,puzzle->button_6->yi,ALIGN_LEFT, background_buffer);

	} else if (puzzle->mouse_on_b5) {
		draw_bitmap_to_buffer(puzzle->button_1->image_normal,puzzle->button_1->xi,puzzle->button_1->yi,ALIGN_LEFT, background_buffer);
		draw_bitmap_to_buffer(puzzle->button_2->image_normal,puzzle->button_2->xi,puzzle->button_2->yi,ALIGN_LEFT, background_buffer);
		draw_bitmap_to_buffer(puzzle->button_3->image_normal,puzzle->button_3->xi,puzzle->button_3->yi,ALIGN_LEFT, background_buffer);
		draw_bitmap_to_buffer(puzzle->button_4->image_normal,puzzle->button_4->xi,puzzle->button_4->yi,ALIGN_LEFT, background_buffer);
		draw_bitmap_to_buffer(puzzle->button_5->image_highlighted,puzzle->button_5->xi,puzzle->button_5->yi,ALIGN_LEFT, background_buffer);
		draw_bitmap_to_buffer(puzzle->button_6->image_normal,puzzle->button_6->xi,puzzle->button_6->yi,ALIGN_LEFT, background_buffer);

	} else if (puzzle->mouse_on_b6) {
		draw_bitmap_to_buffer(puzzle->button_1->image_normal,puzzle->button_1->xi,puzzle->button_1->yi,ALIGN_LEFT, background_buffer);
		draw_bitmap_to_buffer(puzzle->button_2->image_normal,puzzle->button_2->xi,puzzle->button_2->yi,ALIGN_LEFT, background_buffer);
		draw_bitmap_to_buffer(puzzle->button_3->image_normal,puzzle->button_3->xi,puzzle->button_3->yi,ALIGN_LEFT, background_buffer);
		draw_bitmap_to_buffer(puzzle->button_4->image_normal,puzzle->button_4->xi,puzzle->button_4->yi,ALIGN_LEFT, background_buffer);
		draw_bitmap_to_buffer(puzzle->button_5->image_normal,puzzle->button_5->xi,puzzle->button_5->yi,ALIGN_LEFT, background_buffer);
		draw_bitmap_to_buffer(puzzle->button_6->image_highlighted,puzzle->button_6->xi,puzzle->button_6->yi,ALIGN_LEFT, background_buffer);
	}
	else {

		draw_bitmap_to_buffer(puzzle->button_1->image_normal,puzzle->button_1->xi,puzzle->button_1->yi,ALIGN_LEFT, background_buffer);
		draw_bitmap_to_buffer(puzzle->button_2->image_normal,puzzle->button_2->xi,puzzle->button_2->yi,ALIGN_LEFT, background_buffer);
		draw_bitmap_to_buffer(puzzle->button_3->image_normal,puzzle->button_3->xi,puzzle->button_3->yi,ALIGN_LEFT, background_buffer);
		draw_bitmap_to_buffer(puzzle->button_4->image_normal,puzzle->button_4->xi,puzzle->button_4->yi,ALIGN_LEFT, background_buffer);
		draw_bitmap_to_buffer(puzzle->button_5->image_normal,puzzle->button_5->xi,puzzle->button_5->yi,ALIGN_LEFT, background_buffer);
		draw_bitmap_to_buffer(puzzle->button_6->image_normal,puzzle->button_6->xi,puzzle->button_6->yi,ALIGN_LEFT, background_buffer);
	}


}

void delete_word_puzzle(WordPuzzle* puzzle) {

	deleteBitmap(puzzle->test);
	delete_button(puzzle->button_1);
	delete_button(puzzle->button_2);
	delete_button(puzzle->button_3);
	delete_button(puzzle->button_4);
	delete_button(puzzle->button_5);
	delete_button(puzzle->button_6);
	deleteBitmap(puzzle->led);
	deleteBitmap(puzzle->hint);
	free(puzzle);

}

//NUMBERS PUZZLE

NumbersPuzzle* new_numbers_puzzle() {

	NumbersPuzzle* numbers_puzzle = (NumbersPuzzle*) malloc(sizeof(NumbersPuzzle));

	numbers_puzzle->completed = 0;
	numbers_puzzle->mouse_on_b1 = 0;
	numbers_puzzle->mouse_on_b2 = 0;
	numbers_puzzle->mouse_on_b3 = 0;
	numbers_puzzle->mouse_on_b4 = 0;


	// random generator to decide which button takes which color - need to do

	//BUTTON 1
	numbers_puzzle->button_1 = new_button (5,75,564,638);
	numbers_puzzle->button_1->image_normal = loadBitmap("home/lcom/svn/project/src/images/button1.bmp");
	numbers_puzzle->button_1->image_highlighted = loadBitmap("home/lcom/svn/project/src/images/button1_h.bmp");


	//BUTTON 2
	numbers_puzzle->button_2 = new_button (90,160,564,638);
	numbers_puzzle->button_2->image_normal = loadBitmap("home/lcom/svn/project/src/images/button2.bmp");
	numbers_puzzle->button_2->image_highlighted = loadBitmap("home/lcom/svn/project/src/images/button2_h.bmp");

	//BUTTON 3
	numbers_puzzle->button_3 = new_button (174,244,564,638);
	numbers_puzzle->button_3->image_normal = loadBitmap("home/lcom/svn/project/src/images/button3.bmp");
	numbers_puzzle->button_3->image_highlighted = loadBitmap("home/lcom/svn/project/src/images/button3_h.bmp");

	//BUTTON 4
	numbers_puzzle->button_4 = new_button (258,328,564,638);
	numbers_puzzle->button_4->image_normal = loadBitmap("home/lcom/svn/project/src/images/button4.bmp");
	numbers_puzzle->button_4->image_highlighted = loadBitmap("home/lcom/svn/project/src/images/button4_h.bmp");



	numbers_puzzle->draw = 1;
	numbers_puzzle->wrong = 0;
	numbers_puzzle->test = loadBitmap("/home/lcom/svn/project/src/images/puzzle.bmp");
	numbers_puzzle->led = loadBitmap("/home/lcom/svn/project/src/images/not_done.bmp");
	numbers_puzzle->hint = loadBitmap("home/lcom/svn/project/src/images/hint_2.bmp");

	return numbers_puzzle;

}

int update_numbers_puzzle(NumbersPuzzle* puzzle) {

	//SOLUTION WILL BE PRESSING BUTTON 3

	if (puzzle->wrong == 1)
		puzzle->wrong = 0;

	int draw = 0;
	if (puzzle->completed == 1)
	{
		return draw;
	}

	int mouse_on_b1_before = puzzle->mouse_on_b1;
	int mouse_on_b2_before = puzzle->mouse_on_b2;
	int mouse_on_b3_before = puzzle->mouse_on_b3;
	int mouse_on_b4_before = puzzle->mouse_on_b4;


	//check if mouse is on the  button 1 (and if the left key was released)

	if (is_mouse_inside_button(mouse, puzzle->button_1)) {

		puzzle->mouse_on_b1 = 1;
		draw = 1;

		//change button to highlighted version

		if (mouse->lb_released == 1) {


			puzzle->wrong = 1;
			puzzle->led = loadBitmap("/home/lcom/svn/project/src/images/bad_done.bmp");
		}

	} else {

		if (mouse_on_b1_before == 1) {
			draw = 1;
		}

		puzzle->mouse_on_b1 = 0;

	}

	//check if mouse is on the  button 2 (and if the left key was released)

	if (is_mouse_inside_button(mouse, puzzle->button_2)) {

		puzzle->mouse_on_b2 = 1;
		draw = 1;

		//change button to highlighted version

		if (mouse->lb_released == 1) {


			puzzle->wrong = 1;
			puzzle->led = loadBitmap("/home/lcom/svn/project/src/images/bad_done.bmp");		}

	} else {

		if (mouse_on_b2_before == 1) {
			draw = 1;
		}

		puzzle->mouse_on_b2 = 0;

	}

	//check if mouse is on the  button 3 (and if the left key was released)

	if (is_mouse_inside_button(mouse, puzzle->button_3)) {

		puzzle->mouse_on_b3 = 1;
		draw = 1;

		//change button to highlighted version

		if (mouse->lb_released == 1) {

			puzzle->completed = 1;
			puzzle->led = loadBitmap("/home/lcom/svn/project/src/images/done.bmp");
		}

	} else {

		if (mouse_on_b3_before == 1) {
			draw = 1;
		}

		puzzle->mouse_on_b3 = 0;

	}

	//check if mouse is on the  button 1 (and if the left key was released)

	if (is_mouse_inside_button(mouse, puzzle->button_4)) {

		puzzle->mouse_on_b4 = 1;
		draw = 1;

		//change button to highlighted version

		if (mouse->lb_released == 1) {


			puzzle->wrong = 1;
			puzzle->led = loadBitmap("/home/lcom/svn/project/src/images/bad_done.bmp");
		}

	} else {

		if (mouse_on_b4_before == 1) {
			draw = 1;
		}

		puzzle->mouse_on_b4 = 0;

	}

	return draw;

}

void draw_numbers_puzzle(NumbersPuzzle* puzzle) {



	draw_bitmap_to_buffer(puzzle->test, 0, 334, ALIGN_LEFT, background_buffer);
	draw_bitmap_to_buffer(puzzle->led,791,507,ALIGN_LEFT,background_buffer);
	//draw_bitmap_to_buffer(puzzle->hint,99,344,ALIGN_LEFT,background_buffer);

	if (puzzle->mouse_on_b1) {
		draw_bitmap_to_buffer(puzzle->button_1->image_highlighted,puzzle->button_1->xi,puzzle->button_1->yi,ALIGN_LEFT, background_buffer);
		draw_bitmap_to_buffer(puzzle->button_2->image_normal,puzzle->button_2->xi,puzzle->button_2->yi,ALIGN_LEFT, background_buffer);
		draw_bitmap_to_buffer(puzzle->button_3->image_normal,puzzle->button_3->xi,puzzle->button_3->yi,ALIGN_LEFT, background_buffer);
		draw_bitmap_to_buffer(puzzle->button_4->image_normal,puzzle->button_4->xi,puzzle->button_4->yi,ALIGN_LEFT, background_buffer);
	} else if (puzzle->mouse_on_b2) {
		draw_bitmap_to_buffer(puzzle->button_1->image_normal,puzzle->button_1->xi,puzzle->button_1->yi,ALIGN_LEFT, background_buffer);
		draw_bitmap_to_buffer(puzzle->button_2->image_highlighted,puzzle->button_2->xi,puzzle->button_2->yi,ALIGN_LEFT, background_buffer);
		draw_bitmap_to_buffer(puzzle->button_3->image_normal,puzzle->button_3->xi,puzzle->button_3->yi,ALIGN_LEFT, background_buffer);
		draw_bitmap_to_buffer(puzzle->button_4->image_normal,puzzle->button_4->xi,puzzle->button_4->yi,ALIGN_LEFT, background_buffer);

	} else if (puzzle->mouse_on_b3) {
		draw_bitmap_to_buffer(puzzle->button_1->image_normal,puzzle->button_1->xi,puzzle->button_1->yi,ALIGN_LEFT, background_buffer);
		draw_bitmap_to_buffer(puzzle->button_2->image_normal,puzzle->button_2->xi,puzzle->button_2->yi,ALIGN_LEFT, background_buffer);
		draw_bitmap_to_buffer(puzzle->button_3->image_highlighted,puzzle->button_3->xi,puzzle->button_3->yi,ALIGN_LEFT, background_buffer);
		draw_bitmap_to_buffer(puzzle->button_4->image_normal,puzzle->button_4->xi,puzzle->button_4->yi,ALIGN_LEFT, background_buffer);

	} else if (puzzle->mouse_on_b4) {
		draw_bitmap_to_buffer(puzzle->button_1->image_normal,puzzle->button_1->xi,puzzle->button_1->yi,ALIGN_LEFT, background_buffer);
		draw_bitmap_to_buffer(puzzle->button_2->image_normal,puzzle->button_2->xi,puzzle->button_2->yi,ALIGN_LEFT, background_buffer);
		draw_bitmap_to_buffer(puzzle->button_3->image_normal,puzzle->button_3->xi,puzzle->button_3->yi,ALIGN_LEFT, background_buffer);
		draw_bitmap_to_buffer(puzzle->button_4->image_highlighted,puzzle->button_4->xi,puzzle->button_4->yi,ALIGN_LEFT, background_buffer);
	}
	else {

		draw_bitmap_to_buffer(puzzle->button_1->image_normal,puzzle->button_1->xi,puzzle->button_1->yi,ALIGN_LEFT, background_buffer);
		draw_bitmap_to_buffer(puzzle->button_2->image_normal,puzzle->button_2->xi,puzzle->button_2->yi,ALIGN_LEFT, background_buffer);
		draw_bitmap_to_buffer(puzzle->button_3->image_normal,puzzle->button_3->xi,puzzle->button_3->yi,ALIGN_LEFT, background_buffer);
		draw_bitmap_to_buffer(puzzle->button_4->image_normal,puzzle->button_4->xi,puzzle->button_4->yi,ALIGN_LEFT, background_buffer);
	}

}


void delete_numbers_puzzle(NumbersPuzzle* puzzle) {

	deleteBitmap(puzzle->test);
	delete_button(puzzle->button_1);
	delete_button(puzzle->button_2);
	delete_button(puzzle->button_3);
	delete_button(puzzle->button_4);
	deleteBitmap(puzzle->led);
	deleteBitmap(puzzle->hint);
	free(puzzle);

}

//PUZZLE5

Puzzle5* new_puzzle5_puzzle() {

	Puzzle5* puzzle5_puzzle = (Puzzle5*) malloc(sizeof(Puzzle5));

	puzzle5_puzzle->completed = 0;

	// random generator to decide which button takes which color - need to do

	puzzle5_puzzle->draw = 1; //CHANGE THIS AFTER
	puzzle5_puzzle->test = loadBitmap("/home/lcom/svn/project/src/images/puzzle.bmp"); //CHANGE THIS AS WELL

	return puzzle5_puzzle;

}

int update_puzzle5_puzzle(Puzzle5* puzzle) {
	puzzle->completed = 1;
	//update accordingly

	return 0;

}

void draw_puzzle5_puzzle(Puzzle5* puzzle) {

	puzzle->draw = 0;

	draw_bitmap_to_buffer(puzzle->test, 341, 334, ALIGN_LEFT, background_buffer);

}

void delete_puzzle5_puzzle(Puzzle5* puzzle) {

	deleteBitmap(puzzle->test);
	free(puzzle);

}

//PUZZLE6

Puzzle6* new_puzzle6_puzzle() {

	Puzzle6* puzzle6_puzzle = (Puzzle6*) malloc(sizeof(Puzzle6));

	puzzle6_puzzle->completed = 0;



	puzzle6_puzzle->draw = 1;
	puzzle6_puzzle->test = loadBitmap("/home/lcom/svn/project/src/images/puzzle.bmp");

	return puzzle6_puzzle;

}

int update_puzzle6_puzzle(Puzzle6* puzzle) {
	puzzle->completed = 1;
	//update accordingly

	return 0;

}

void draw_puzzle6_puzzle(Puzzle6* puzzle) {

	puzzle->draw = 0;

	draw_bitmap_to_buffer(puzzle->test, 683, 334, ALIGN_LEFT, background_buffer);


}

void delete_puzzle6_puzzle(Puzzle6* puzzle) {

	deleteBitmap(puzzle->test);
	free(puzzle);

}
