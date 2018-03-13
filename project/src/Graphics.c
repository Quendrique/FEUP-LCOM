#include "Graphics.h"
#include "Aux.h"

#define PB2BASE(x) (((x) >> 4) & 0x0F000)
#define PB2OFF(x) ((x) & 0x0FFFF)

/* VBE FUNCTIONS AND OTHER CONSTANTS*/

#define BIOS_VC_INT				0x10
#define LINEAR_FRAME_BUFFER		BIT(14)
#define SET_VBE_MODE			0x4F02
#define RETURN_VBE_MODE_INFO	0x4F01
#define RETURN_VBE_CONTROLLER	0x4F00

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

unsigned getHorResolution()
{
	return h_res;
}
unsigned getVerResolution()
{
	return v_res;
}

char *  getGraphicsBuffer()
{
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

	if (x > 1024 || y > 768) {
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

	x_new = x - (size/2);
	y_new = y - (size/2);

	for (i=0; i < size; i++) { //vertical lines

		for (j = 0; j < size; j++) { //horizontal lines

			if (vg_set_pixel_corner(x_new+j, y_new+i, color) == 1) {
				return 1;
			}
		}
	}

	return 0;

}

//BUFFER RELATED FUNCTIONS

void initialize_buffers() {

	background_buffer = (char*) malloc((h_res * v_res * bits_per_pixel) / 8);
	mouse_buffer = (char*) malloc((h_res * v_res * bits_per_pixel) / 8);

}

void delete_buffers() {

	free(background_buffer);
	free(mouse_buffer);

}

void background_to_mouse_buffer() {
	memcpy(mouse_buffer, background_buffer, (h_res * v_res * bits_per_pixel) / 8);
}

void mouse_buffer_to_video_mem() {
	memcpy(video_mem, mouse_buffer, (h_res * v_res * bits_per_pixel) / 8);
}

//END OF BUFFER RELATED FUNCTIONS

int vbe_get_mode_info(unsigned short mode, vbe_mode_info_t *vmi_p) {

	struct reg86u reg86;
	mmap_t mmap;

	if (lm_init() == NULL) {
		printf("\tvbe_get_mode_info(): lm_init() failed \n");
		return 1;
	}

	if (lm_alloc(sizeof(vbe_mode_info_t), &mmap) == NULL) {
		printf("\tvbe_get_mode_info(): lm_alloc() failed \n");
		return 1;
	}

	reg86.u.b.intno = BIOS_VC_INT;
	reg86.u.w.ax = RETURN_VBE_MODE_INFO;
	reg86.u.w.cx = mode;
	reg86.u.w.es = PB2BASE(mmap.phys);
	reg86.u.w.di = PB2OFF(mmap.phys);

	if( sys_int86(&reg86) != OK ) {
		printf("\tvbe_get_mode_info(): sys_int86() failed \n");
		lm_free(&mmap);
		return 1;
	}

	*vmi_p = *(vbe_mode_info_t*) mmap.virtual;
	lm_free(&mmap);
	return 0;




}

int vbe_set_mode(unsigned short mode) {

	struct reg86u reg86;

	reg86.u.b.intno = BIOS_VC_INT; //BIOS video card interrupt service

	reg86.u.w.ax = SET_VBE_MODE;
	reg86.u.w.bx = LINEAR_FRAME_BUFFER | mode;

	if( sys_int86(&reg86) != OK ) {
		printf("\tvbe_set_mode(): sys_int86() failed \n");
		return 1;
	} else
		return 0;

}

int vbe_get_controller_info(vbe_controller_info_t *vci_p) {

	struct reg86u reg86;
	mmap_t mmap;

	if (lm_init() == NULL) {
		printf("\tvbe_get_mode_info(): lm_init() failed \n");
		return 1;
	}

	if (lm_alloc(sizeof(vbe_mode_info_t), &mmap) == NULL) {
		printf("\tvbe_get_mode_info(): lm_alloc() failed \n");
		return 1;
	}

	reg86.u.b.intno = BIOS_VC_INT;
	reg86.u.w.ax = RETURN_VBE_CONTROLLER;
	reg86.u.w.es = PB2BASE(mmap.phys);
	reg86.u.w.di = PB2OFF(mmap.phys);

	if( sys_int86(&reg86) != OK ) {
		printf("\tvbe_get_controller_info(): sys_int86() failed \n");
		lm_free(&mmap);
		return 1;
	}

	*vci_p = *(vbe_controller_info_t*) mmap.virtual;
	lm_free(&mmap);
	return 0;



}


int draw(Bitmap * bmp) {

	drawBitmap (bmp, 0 ,0 ,ALIGN_LEFT);
}
