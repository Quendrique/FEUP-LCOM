#include "Button.h"
#include "Bitmap.h"

Button *new_button (int xi, int xf, int yi, int yf)
{
	Button *button = (Button *) malloc (sizeof (Button));
	button->xi = xi;
	button->xf = xf;
	button->yi = yi;
	button->yf = yf;


	return button;

}

void draw_button_highlighted(Button* button) {

	//draw_bitmap_to_buffer(button,button->xi,button->yi,ALIGN_LEFT);

}

void delete_button (Button* button)
{
	free(button);
}
