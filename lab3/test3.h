#ifndef __TEST3_H
#define __TEST3_H

extern unsigned long int g_scancode;

/** @defgroup test3 test3
 * @{
 *
 * Functions for testing the kbd code
 */

/** 
 * @brief To test reading of scancode via KBD interrupts 
 * 
 * Displays the scancodes received from the keyboard
 * Exits upon release of the ESC key
 * 
 * @param asm Which IH to use: if 0, in C, otherwise in assembly
 * 
 * @return Return 0 upon success and non-zero otherwise
 */
int kbd_test_scan(unsigned short assembly);

/**
 * @brief To test reading of scancodes via polling
 * 
 * Displays the scancodes received from the keyboard
 * Exits upon release of the ESC key
 * 
 * @return Return 0 upon success and non-zero otherwise
 */
int kbd_test_poll();

/** 
 * @brief To test handling of more than one interrupt
 * 
 *  Similar to kbd_test_scan() except that it 
 *  should terminate also if no scancodes are received for n seconds
 * 
 * @param n Number of seconds without scancodes before exiting
 * 
 * @return Return 0 upon success and non-zero otherwise
 */
int kbd_test_timed_scan(unsigned short n);

int kbd_subscribe_int(void);

int kbd_unsubscribe_int(void);

long int kbd_int_handler(void);

unsigned long int kbd_write_cmd_noargs(unsigned long int cmd);

unsigned long int kbd_write_cmd_args(unsigned long int cmd, unsigned long int arg);

unsigned long int kbd_read_cmd();

unsigned long int kbd_write_cmd_test(unsigned long int cmd, unsigned long int arg);

int kbd_get_status(unsigned char *st);

int kbd_test_status(void);

void kbd_scan_assembly();

void timer_int_handler();

int timer_unsubscribe_int();

int timer_subscribe_int(void);

#endif /* __TEST3_H */
