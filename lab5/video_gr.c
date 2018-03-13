#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>

#include "vbe.h"
#include "video_gr.h"
#include "lmlib.h"
#include "read_xpm.c"

/* Constants for VBE 0x105 mode */

/* The physical address may vary from VM to VM.
 * At one time it was 0xD0000000
 *  #define VRAM_PHYS_ADDR    0xD0000000 
 * Currently on lab B107 is 0xF0000000
 * Better run my version of lab5 as follows:
 *     service run `pwd`/lab5 -args "mode 0x105"
 */



/* Private global variables */

static char *video_mem;		/* Process (virtual) address to which VRAM is mapped */

static unsigned h_res;		/* Horizontal screen resolution in pixels */
static unsigned v_res;		/* Vertical screen resolution in pixels */
static unsigned bits_per_pixel; /* Number of VRAM bits per pixel */

int vg_exit() {

	struct reg86u reg86;

	reg86.u.b.intno = 0x10; /* BIOS video services */

	reg86.u.b.ah = 0x00;    /* Set Video Mode function */
	reg86.u.b.al = 0x03;    /* 80x25 text mode*/

	if( sys_int86(&reg86) != OK ) {
		printf("\tvg_exit(): sys_int86() failed \n");
		return 1;
	} else
		return 0;
}

void *vg_init(unsigned short mode) {

	int r;
	vbe_mode_info_t vmi_p;
	struct mem_range mr;
	unsigned int vram_base;  // VRAM's physical address
	unsigned int vram_size;  // VRAM's size, but you can use
	                          //  the frame-buffer size, instead
	//void *video_mem;         // frame-buffer VM address

	vbe_set_mode(mode); //setting the mode

	vbe_get_mode_info(mode, &vmi_p); //getting mode info

	h_res = vmi_p.XResolution;
	v_res = vmi_p.YResolution;
	bits_per_pixel = vmi_p.BitsPerPixel;

	vram_base = vmi_p.PhysBasePtr; //assigning the vram's physical address to local variable
	vram_size = (h_res * v_res * bits_per_pixel) / 8; //obtaining the vram's size in bytes

	// Allow memory mapping

	mr.mr_base = (phys_bytes) vram_base;
	mr.mr_limit = mr.mr_base + vram_size;

	if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
	   panic("sys_privctl (ADD_MEM) failed: %d\n", r);

	// Map memory //

	video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);

	if(video_mem == MAP_FAILED)
	   panic("couldn't map video memory");

	if(video_mem == NULL) {
		printf("Failed to map memory\n");
		return NULL;
	}

	return video_mem;


}

int vg_set_pixel(unsigned short x, unsigned short y, unsigned short color) { //in relation to a referential with origin in the center of the screen

	char *p = video_mem;
	p += (((1024/2)  + (768/2) * h_res) * bits_per_pixel) / 8; //center of the screen

	if (x > (1024/2) || y > (768/2)) { //since this function is only set to work with mode 0x105h, these are the only two conditions the coordinates must fulfill in order to be valid (them not being negative values is already covered by the unsigned's)
		printf("Invalid coordinates\n");
		return 1;
	}

	p += ((x  + y * h_res) * bits_per_pixel)/8;

	*p = color;
	return 0;

}

int vg_set_pixel_corner(unsigned short x, unsigned short y, unsigned short color) { //in relation to a referential with origin in the center of the screen

	char *p = video_mem;
	//p += ((0  + 0 * h_res) * bits_per_pixel) / 8;

	if (x > 1024 || y > 768) { //since this function is only set to work with mode 0x105h, these are the only two conditions the coordinates must fulfill in order to be valid (them not being negative values is already covered by the unsigned's)
		printf("Invalid coordinates\n");
		return 1;
	}

	p += ((x  + y * h_res) * bits_per_pixel)/8;

	*p = color;
	return 0;

}

int vg_draw_square(unsigned int x, unsigned int y, unsigned short size, unsigned int color) {

	unsigned int i, j;
	unsigned short x_new, y_new;

	if (size > (h_res/2) || size > (v_res/2)) {
		printf("Invalid size\n");
		return 1;
	}

	// calculate the coordinates of the left-most pixel in the upper left corner of the square (in relation to the center of the screen)
	// THE SQUARE CAN ONLY BE DRAWN IN THE LOWER RIGHT CORNER OF THE SCREEN

	x_new = x - (size/2);
	y_new = y - (size/2);

	for (i=0; i < size; i++) { //vertical lines

		for (j = 0; j < size; j++) { //horizontal lines

			if (vg_set_pixel(x_new+j, y_new+i, color) == 1) {
				return 1;
			}
		}
	}

	return 0;

}

int vg_draw_sprite(char *xpm[], unsigned short xi, unsigned short yi) {

	unsigned int i, j;
	int w, h;
	char* colors = read_xpm(xpm, &w, &h);
	if (colors == NULL)
		return 1;

	for (i=0; i < h; i++) { //vertical lines

		for (j = 0; j < w; j++) { //horizontal lines

			if (vg_set_pixel_corner(xi+j, yi+i, *colors) == 1) {
				return 1;
			}
			colors++;

		}
	}

	return 0;

}

int vg_fill_black(char *xpm[], unsigned short xi, unsigned short yi) {

	unsigned int i, j;
	int w, h;
	read_xpm(xpm, &w, &h);

	for (i=0; i < h; i++) { //vertical lines

		for (j = 0; j < w; j++) { //horizontal lines

			if (vg_set_pixel_corner(xi+j, yi+i, 0) == 1) {
				return 1;
			}
		}
	}

	return 0;

}
