#include <minix/syslib.h>
#include <minix/drivers.h>
#include <stdint.h>
#include <sys/types.h>
#include <machine/int86.h>

#include "vbe.h"
#include "lmlib.h"

#define BIT(n) (0x0001<<(n))

#define PB2BASE(x) (((x) >> 4) & 0x0F000)
#define PB2OFF(x) ((x) & 0x0FFFF)

/* VBE FUNCTIONS AND OTHER CONSTANTS*/

#define BIOS_VC_INT				0x10
#define LINEAR_FRAME_BUFFER		BIT(14)
#define SET_VBE_MODE			0x4F02
#define RETURN_VBE_MODE_INFO	0x4F01
#define RETURN_VBE_CONTROLLER	0x4F00


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


