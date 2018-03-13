#include "i8042.h"

.global _kbd_scan_assembly, _g_scancode

.data
_g_scancode: .long 0

.text

_kbd_scan_assembly:

	inb $0x64, %al //al has the status byte
	test $KBD_STAT_OBF, %al
	cmp $KBD_STAT_OBF, %al //checks if out buffer not full
	jne obf
	movl $1, %eax //returns 1 if it detects an error/out buffer not full
	jmp end

	obf:
	inb $0x64, %al
	test $KBD_STAT_PARITY, %al //checks if there's a parity error
	cmp $KBD_STAT_PARITY, %al
	jne no_par_error
	movl $1, %eax //returns 1 if it detects an error/out buffer not full
	jmp end

	no_par_error:
	inb $0x64, %al
	test $KBD_STAT_TIMEOUT, %al //checks if there's a timeout error
	cmp $KBD_STAT_TIMEOUT, %al
	jne no_time_error
	movl $1, %eax //returns 1 if it detects an error/out buffer not full
	jmp end

	no_time_error:
	inb $0x60, %al
	test $0x0000FF, %eax //eax contains the scancode
	mov %eax, _g_scancode //saves the scancode in the global variable

	mov $2147483, %ecx //delay
	cycle:
	loop cycle

	end:
	ret
