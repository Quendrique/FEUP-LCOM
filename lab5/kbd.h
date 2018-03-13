#ifndef __KBD_H
#define __KBD_H

int kbd_subscribe_int(void);

int kbd_unsubscribe_int(void);

long int kbd_int_handler(void);

int kbd_get_status(unsigned char *st);

#endif
