#include <minix/syslib.h>

#include <minix/drivers.h>
#include <minix/driver.h>

#include <minix/sysutil.h>

#include <minix/ipc.h>

#include "test3.h"

#include "i8042.h"

#include "i8254.h"

unsigned long int g_scancode, sys_inb_calls = 0;

static long unsigned int counter = 0; //global counter

int hook_id = 0x00;

int kbd_hook_id = KBD_IRQ;

int kbd_test_scan(unsigned short assembly) {

        int ipc_status, irq_set = BIT(kbd_hook_id), r;

        message msg;

        unsigned long int scancode,scancode_big1;

        long int test_scancode;

        //1.Subscribe to keyboard interrupts and check if it failed

        if(kbd_subscribe_int() != KBD_IRQ)

        {

                return 1;

        }

        //2.Handler should print message

        while(scancode != ESC_KEY_BREAKCODE) {

                if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {

                        printf("driver_receive failed with: %d", r);

                        continue;

                }

                if (is_ipc_notify(ipc_status)) {

                        switch (_ENDPOINT_P(msg.m_source)) {

                        case HARDWARE:

                                if (msg.NOTIFY_ARG & irq_set) {

                                        if (assembly != 0) {

                                                kbd_scan_assembly();

                                                test_scancode = g_scancode;

                                        }

                                        else

                                                test_scancode=kbd_int_handler();

                                        if(test_scancode == -1)

                                        {

                                                printf("Interrupt handler was not successful");

                                                return 1;

                                        }

                                        scancode = (unsigned long int) test_scancode;

                                        scancode_big1=scancode;

                                        if(scancode_big1 == MAKECODE_LONG)

                                        {

                                                if (assembly != 0) {

                                                        kbd_scan_assembly();

                                                        test_scancode = g_scancode;

                                                }

                                                else

                                                        test_scancode=kbd_int_handler();

                                                if(test_scancode == -1)

                                                {

                                                        printf("Interrupt handler was not successful");

                                                        return 1;

                                                }

                                                scancode = (unsigned long int) test_scancode;

                                                if(BIT(7) & scancode) {

                                                        printf("Breakcode : 0x");

                                                        printf("%x",scancode_big1);

                                                        printf(" 0x");

                                                printf("%x\n",scancode);

                                                }

                                                else {

                                                        printf("Makecode : 0x");

                                                        printf("%x",scancode_big1);

                                                        printf(" 0x");

                                                        printf("%x\n",scancode);

                                                }

                                        }

                                        else

                                        {

                                                if(BIT(7) & scancode) {

                                                        printf("Breakcode : 0x");

                                                        printf("%x\n",scancode);

                                                }

                                                else {

                                                        printf("Makecode : 0x");

                                                        printf("%x\n",scancode);

                                                }

                                        }

                                }

                                break;

                        default:

                                break;

                        }

                } else { }

        }

        //3.Unsubscribe to keyboard interrupts

        if(kbd_unsubscribe_int()!= 0) {

                return 1;

        }

        printf("Interrupt handler successful\n");

        if (assembly == 0)

                printf("Sys_inb calls made: %u\n", sys_inb_calls);

        return 0;

}

int kbd_test_poll() {

        unsigned long int scancode, scancode_big, previous_cmd;
        unsigned char status;

        while(scancode != ESC_KEY_BREAKCODE) {

                kbd_get_status(&status);

                sys_inb_calls++;

                if(status & KBD_STAT_OBF) {

                        if((status & (KBD_STAT_PARITY | KBD_STAT_TIMEOUT )) == 0) {

                                sys_inb(OUT_BUFFER, &scancode);

                                sys_inb_calls++;

                                if(scancode == MAKECODE_LONG)

                                {

                                        scancode_big = scancode;

                                        sys_inb(OUT_BUFFER, &scancode);

                                        sys_inb_calls++;

                                        if(BIT(7) & scancode){

                                                                                        printf("Breakcode : 0x");

                                                                                        printf("%x",scancode_big);

                                                                                        printf(" 0x");

                                                                                printf("%x\n",scancode);

                                                                                }

                                                                                else {

                                                                                        printf("Makecode : 0x");

                                                                                        printf("%x",scancode_big);

                                                                                        printf(" 0x");

                                                                                printf("%x\n",scancode);

                                                                                }

                                }

                                else {

                                        if(BIT(7) & scancode) {

                                                printf("Breakcode : 0x");

                                                printf("%x\n",scancode);

                                }

                                else {

                                        printf("Makecode : 0x");

                                        printf("%x\n",scancode);

                                }

                        }

                }

                        else

                                return 1;

        }

                tickdelay(micros_to_ticks(DELAY_US));

        }

        //waits until the input buffer isn't full to write the command (may enter an infinite loop?)

        while((status & KBD_STAT_IBF) != 0) {

                kbd_get_status(&status);

        }

        previous_cmd = kbd_write_cmd_test(READ_CMD, 0);

        kbd_write_cmd_test(WRITE_CMD, (KBC_ENB_KBD | KBC_ENB_MOUSE | previous_cmd));

        printf("Poll successful\n");

        printf("Sys_inb calls made: %u\n", sys_inb_calls);

        return 0;

}

int kbd_test_timed_scan(unsigned short n) {

                int ipc_status, irq_kbd_set = BIT(KBD_IRQ), irq_timer0_set = BIT(TIMER0_IRQ), r, seconds_passed = 0;

                message msg;

                unsigned long int scancode, test_scancode, scancode_big1;

                //1.Subscribe to keyboard and timer interrupts and check if it failed

                if (kbd_subscribe_int() != KBD_IRQ) {

                        return 1;

                }

                if (timer_subscribe_int() != TIMER0_IRQ) {

                        return 1;

                }

                //2. Handlers check time and print scancodes

                while ((scancode != ESC_KEY_BREAKCODE) && (seconds_passed <= n )) { // n++ should solve the problem of finishing 1 second before the desired time

                        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {

                                printf("driver_receive failed with: %d", r);

                                continue;

                        }

                        if (is_ipc_notify(ipc_status)) {

                                switch (_ENDPOINT_P(msg.m_source)) {

                                case HARDWARE:

                                        if (msg.NOTIFY_ARG & irq_kbd_set) {

                                                test_scancode=kbd_int_handler();

                                                if(test_scancode == -1)

                                                {

                                                        printf("Interrupt handler was not successful");

                                                        return 1;

                                                }

                                                scancode = (unsigned long int) test_scancode;

                                                scancode_big1=scancode;

                                                if(scancode_big1 == MAKECODE_LONG) //not sure if it is working correctly, when the arrow keys are pressed 2 makecodes and 2 breakcodes are printed

                                                {

                                                        test_scancode=kbd_int_handler();

                                                        if(test_scancode == -1)

                                                        {

                                                                printf("Interrupt handler was not successful");

                                                                return 1;

                                                        }

                                                        scancode = (unsigned long int) test_scancode;

                                                        sys_inb_calls++;

                                                        if(BIT(7) & scancode){

                                                                                                        printf("Breakcode : 0x");

                                                                                                        printf("%x",scancode_big1);

                                                                                                        printf(" 0x");

                                                                                                printf("%x\n",scancode);

                                                                                                }

                                                                                                else {

                                                                                                        printf("Makecode : 0x");

                                                                                                        printf("%x",scancode_big1);

                                                                                                        printf(" 0x");

                                                                                                printf("%x\n",scancode);

                                                                                                }

                                                }

                                                else        if(BIT(7) & scancode){

                                                        printf("Breakcode : 0x");

                                                printf("%x\n",scancode);

                                                }

                                                else {

                                                        printf("Makecode : 0x");

                                                printf("%x\n",scancode);

                                                }

                                        }

                                        if (msg.NOTIFY_ARG & irq_timer0_set) {

                                                if(counter == 0)

                                                        seconds_passed = 0;

                                                if(counter % 60 == 0) {


                                                        seconds_passed++;

                                                }

                                                timer_int_handler();

                                        }

                                        break;

                                default:

                                        break;

                                }

                        } else { }

                }

                //3.Unsubscribe to keyboard and timer interrupts

                if(timer_unsubscribe_int()!=OK) {

                        return 1;

                }

                if(kbd_unsubscribe_int()!= OK) {

                        return 1;

                }

                printf("Interrupt handler successful\n");

                printf("Sys_inb calls made: %u\n", sys_inb_calls);

                return 0;

return 0;

}

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

                if(status & KBD_STAT_OBF)

                {

                        counter = 0; // resets the global counter, since the kbd is not idle anymore

                        if((status & (KBD_STAT_PARITY | KBD_STAT_TIMEOUT ))==0)

                        {

                                sys_inb(OUT_BUFFER, &scancode);

                                sys_inb_calls++;

                                count++;

                                return scancode;

                        }

                        else return -1;

                }

                tickdelay(micros_to_ticks(DELAY_US));

        }

        return 0;

}

 unsigned long int kbd_write_cmd_noargs(unsigned long int cmd) {

         unsigned long int return_value;
         unsigned char status;

         unsigned int count=0;

         while(count < 30) {

                 kbd_get_status(&status);

                 if((status & KBD_STAT_IBF) == 0) {

                         sys_outb(KBD_STAT_REG, cmd);

                         sys_inb(OUT_BUFFER, &return_value);

                         count++;

                         return return_value;

                 } else {

                         return 1;

                 }

                 tickdelay(micros_to_ticks(DELAY_US));

         }

         return 1;

 }

 unsigned long int kbd_write_cmd_args(unsigned long int cmd, unsigned long int arg) {

         unsigned char status;

         unsigned int count=0;

         while(count < 12) {

                 kbd_get_status(&status);

                 if((status & KBD_STAT_IBF) == 0) {

                         sys_outb(KBD_STAT_REG, cmd);

                         sys_outb(OUT_BUFFER, arg); //leaves the last byte of the cmd on the buffer, so it prints it out, need to fix this

                         count++;

                         return 0; //none of the commands we use that require arguments return any values

                 } else {

                         return 1;

                 }

                 tickdelay(micros_to_ticks(DELAY_US));

         }

         return 1;

 }

unsigned long int kbd_read_cmd() {

         unsigned long int return_value;
         unsigned char status;
         unsigned int count=0;

         while(count < 12) {

                 kbd_get_status(&status);

                 if(status & KBD_STAT_OBF) {

                         if((status & (KBD_STAT_PARITY | KBD_STAT_TIMEOUT )) == 0) {

                                 sys_inb(OUT_BUFFER, &return_value);

                                 count++;

                                 return return_value;

                         }

                         else {

                                 return 1;

                         }

                 }

                 tickdelay(micros_to_ticks(DELAY_US));

         }

         return 1;

 }

unsigned long int kbd_write_cmd_test(unsigned long int cmd, unsigned long int arg) { //if arg = 0, then that particular command doesn't require arguments

        //doesn't have any failsafe against unrecognized commands

        unsigned long int result;

        if (arg == 0) {

                result = kbd_write_cmd_noargs(cmd);

                if (result == 1) {

                        printf("Unable to write command\n");

                        return result;

                }

                else if (cmd == 0x20) {

                        //printf("Command byte in memory: 0x%x\n", result);

                        return result;

                }

        } else {

                result = kbd_write_cmd_args(cmd, arg);

                if (result == 1) {

                        printf("Unable to write command\n");
                        return result;
                }

        }

        return result;

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

int kbd_test_status(void) {

        unsigned char status;

        printf("Status register info:\n");

        if (kbd_get_status(&status) == 0) {

                if ((status & KBD_STAT_PARITY) )

                        printf("Parity error\n");

                if ((status & KBD_STAT_TIMEOUT) )

                        printf("Timeout error\n");

                if ((status & KDB_STAT_AUX) )

                        printf("Mouse data enabled\n");

                else

                        printf("Mouse data disabled\n");

                if ((status & KDB_STAT_INH) )

                        printf("Keyboard not inhibited\n");

                else

                        printf("Keyboard inhibited\n");

                if ((status & KBD_STAT_A2) )

                        printf("A2 input: command byte\n");

                else

                        printf("A2 input: data byte\n");

                if ((status & KBD_STAT_SYS) )

                        printf("System initialized\n");

                else

                        printf("System on power-on reset\n");

                if ((status & KBD_STAT_IBF) )

                        printf("Input buffer full\n");

                if ((status & KBD_STAT_OBF) )

                        printf("Output buffer full\n");

                return 0;

        }

        else {

                return -1;

        }

}

// TIMER 0 STUFF

int timer_subscribe_int(void ) {

        unsigned long int hook_temp = hook_id;

        if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id) == OK)

        {

                        if (sys_irqenable(&hook_id) == OK)

                        {

                                return hook_temp;

                        }

        }

        printf("Invalid subscription");

        return 1;



}

int timer_unsubscribe_int() {

        if (sys_irqdisable(&hook_id) == OK)

        {

                if (sys_irqrmpolicy(&hook_id)== OK)

                {

                        return 0;

                }

        }

        printf("Invalid unsubscription");

        return 1;



}

void timer_int_handler() {

        counter++;

}

