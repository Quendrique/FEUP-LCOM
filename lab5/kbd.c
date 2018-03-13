#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/driver.h>
#include <minix/sysutil.h>
#include <minix/ipc.h>
#include "kbd.h"
#include "i8042.h"

int kbd_hook_id = KBD_IRQ;

int kbd_subscribe_int(void) {

        int hook_tmp = kbd_hook_id;

        if (sys_irqsetpolicy(KBD_IRQ, (IRQ_REENABLE | IRQ_EXCLUSIVE), &kbd_hook_id) == OK) {

                        if (sys_irqenable(&kbd_hook_id) == OK) {

                                return hook_tmp;

                        }

        }

        printf("Invalid subscription");

        return 1;



}

int kbd_unsubscribe_int(void) {

        if (sys_irqdisable(&kbd_hook_id) == OK) {

                if (sys_irqrmpolicy(&kbd_hook_id) == OK) {

                        return 0;

                }

        }

        printf("Invalid unsubscription");

        return 1;



}

long int kbd_int_handler(void) {

        unsigned long int scancode;
        unsigned char status;

        unsigned int count=0;

        while(count < 12)

        {

                kbd_get_status(&status);

                if(status & KBD_STAT_OBF) {

                	if((status & (KBD_STAT_PARITY | KBD_STAT_TIMEOUT ))==0)

                    	{

                        	sys_inb(OUT_BUFFER, &scancode);
                            count++;
                            return scancode;

                        }

                        else
                        	return -1;

                }

                tickdelay(micros_to_ticks(DELAY_US));

        }

        return 0;

}

int kbd_get_status(unsigned char *st) {

        unsigned long int tmpStatus;

        if(sys_inb(KBD_STAT_REG, &tmpStatus) == OK) {

                *st= (unsigned char) tmpStatus;

                return 0;

        }

        else {

                printf("Failed to obtain status\n");

                return -1;

        }

}
