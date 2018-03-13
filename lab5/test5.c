#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/driver.h>
#include "test5.h"
#include "kbd.h"
#include "vbe.h"
#include "video_gr.h"
#include "video_test.h"
#include "lmlib.h"
#include "i8042.h"
#include "i8254.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <machine/int86.h>
#include <unistd.h>

#define VIDEO_MODE_105		0x105
int hook_id = 0x00;
static long unsigned int counter = 0;

int video_test_init(unsigned short mode, unsigned short delay) {
	
	if (vg_init(mode) != NULL)
		sleep(delay);
	else
		return 1;
	if (vg_exit() == 1)
		return 1;
	else
		return 0;
	
}


int video_test_square(unsigned short x, unsigned short y, unsigned short size, unsigned long color) {

    int ipc_status, irq_set = BIT(KBD_IRQ), r;
    message msg;
    unsigned long int scancode;
    long int test_scancode;

    //Maps VRAM in the process's address space
	if (vg_init(VIDEO_MODE_105) == NULL) {
		return 1;
	}

	//Draws square
	if (vg_draw_square(x, y, size, color) == 1) {
		vg_exit();
		return 1;
	}
	
	video_dump_fb(); //NEED TO CHECK IF THIS IS WHERE IT'S SUPPOSED TO GO

    //Subscribes to keyboard interrupts and check if it failed

    if(kbd_subscribe_int() != KBD_IRQ) {
            return 1;
    }

    //Waits for the esc key to be released

    while(scancode != ESC_KEY_BREAKCODE) {

            if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
            	printf("driver_receive failed with: %d", r);
                continue;
            }

            if (is_ipc_notify(ipc_status)) {

                    switch (_ENDPOINT_P(msg.m_source)) {

                    case HARDWARE:

                            if (msg.NOTIFY_ARG & irq_set) {



                            	test_scancode=kbd_int_handler();

                                if(test_scancode == -1) {

                                	printf("Interrupt handler was not successful");
                                    return 1;
                                 }

                                 scancode = (unsigned long int) test_scancode;
                                 }


                            break;

                    default:

                            break;

                    }
            }
    }

    //Unsubscribes to keyboard interrupts

    if(kbd_unsubscribe_int()!= 0) {
            return 1;
    }

    //Exits video mode
	if (vg_exit() == 1)
		return 1;
	else
		return 0;

}

int video_test_line(unsigned short xi, unsigned short yi, 
		           unsigned short xf, unsigned short yf, unsigned long color) {
    int ipc_status, irq_set = BIT(KBD_IRQ), r;
    message msg;
    unsigned long int scancode;
    long int test_scancode;

    float delta_x,delta_y,declive,o_or;
    unsigned short  x_new;
	int y_new;

    //Maps VRAM in the process's address space
	if (vg_init(VIDEO_MODE_105) == NULL) {
		return 1;
	}

      delta_x = xf-xi;
      delta_y = yf-yi;

      if(delta_x != 0) // verificar se delta_x = 0, por causa de linhas verticais e pontos
      {

      declive = delta_y/delta_x;
      x_new = xi;
      o_or = yf - declive*xf;

      while (x_new < xf) //declives positivos
      {
    	  y_new = declive * x_new + o_or;

    	  if (vg_set_pixel_corner(x_new, y_new, color) == 1) {
    	  		vg_exit();
    	  		return 1;
    	  	}
    	  x_new++;

      }
      while (x_new > xf)  //declives negativos
           {
         	  y_new = declive * x_new + o_or;

         	  if (vg_set_pixel_corner(x_new, y_new, color) == 1) {
         	  		vg_exit();
         	  		return 1;
         	  	}
         	  x_new--;

           }

      }else {

    	  if((delta_x == 0) && (delta_y== 0)) //PONTOS
    	  {
    		  if (vg_set_pixel_corner(xi, yi, color) == 1) {
    		           	  		vg_exit();
    		           	  		return 1;
    		           	  	}
    	  }

    	  if( delta_y > 0)
    	  {
    		  y_new = yi;
			  while (y_new <= yf)
    		  {
    			  if (vg_set_pixel_corner(xi, y_new, color) == 1) {
    			      	  		vg_exit();
    			      	  		return 1;
    			      	  	}
    			      	  y_new++;
    		  }
    	  }

    	  if( delta_y < 0)
    	     	  {
    	     		  y_new = yi;
    	 			  while (y_new <= yf)
    	     		  {
    	     			  if (vg_set_pixel_corner(xi, y_new, color) == 1) {
    	     			      	  		vg_exit();
    	     			      	  		return 1;
    	     			      	  	}
    	     			      	  y_new++;
    	     		  }
    	     	  }


      }


    video_dump_fb(); //NEED TO CHECK IF THIS IS WHERE IT'S SUPPOSED TO GO


    //Subscribes to keyboard interrupts and check if it failed

    if(kbd_subscribe_int() != KBD_IRQ) {
            return 1;
    }

    //Waits for the esc key to be released

    while(scancode != ESC_KEY_BREAKCODE) {

            if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
            	printf("driver_receive failed with: %d", r);
                continue;
            }

            if (is_ipc_notify(ipc_status)) {

                    switch (_ENDPOINT_P(msg.m_source)) {

                    case HARDWARE:

                            if (msg.NOTIFY_ARG & irq_set) {



                            	test_scancode=kbd_int_handler();

                                if(test_scancode == -1) {

                                	printf("Interrupt handler was not successful");
                                    return 1;
                                 }

                                 scancode = (unsigned long int) test_scancode;
                                 }


                            break;

                    default:

                            break;

                    }
            }
    }

    //Unsubscribes to keyboard interrupts

    if(kbd_unsubscribe_int()!= 0) {
            return 1;
    }

    //Exits video mode
	if (vg_exit() == 1)
		return 1;
	else
		return 0;

	
}

	
int test_xpm(char *xpm[], unsigned short xi, unsigned short yi) {
	
	int ipc_status, irq_set = BIT(KBD_IRQ), r;
    message msg;
    unsigned long int scancode;
    long int test_scancode;

    //Maps VRAM in the process's address space

	if (vg_init(VIDEO_MODE_105) == NULL) {
		return 1;
	}

	//Draws the sprite on screen
	if (vg_draw_sprite(xpm, xi, yi) == 1)
		return 1;

	video_dump_fb(); //NEED TO CHECK IF THIS IS WHERE IT'S SUPPOSED TO GO

    //Subscribes to keyboard interrupts and check if it failed

    if(kbd_subscribe_int() != KBD_IRQ) {
            return 1;
    }

    //Waits for the esc key to be released

    while(scancode != ESC_KEY_BREAKCODE) {

            if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
            	printf("driver_receive failed with: %d", r);
                continue;
            }

            if (is_ipc_notify(ipc_status)) {

                    switch (_ENDPOINT_P(msg.m_source)) {

                    case HARDWARE:

                            if (msg.NOTIFY_ARG & irq_set) {



                            	test_scancode=kbd_int_handler();

                                if(test_scancode == -1) {

                                	printf("Interrupt handler was not successful");
                                    return 1;
                                 }

                                 scancode = (unsigned long int) test_scancode;
                                 }


                            break;

                    default:

                            break;

                    }
            }
    }

    //Unsubscribes to keyboard interrupts

    if(kbd_unsubscribe_int()!= 0) {
            return 1;
    }

    //Exits video mode
	if (vg_exit() == 1)
		return 1;
	else
		return 0;


}

////////////////////////////AUXILIAR STUFF/////////////////////////////////




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

////////////////////////////AUXILIAR STUFF/////////////////////////////////







int test_move(char *xpm[], unsigned short xi, unsigned short yi, unsigned short xf, unsigned short yf, short s, unsigned short f) {

	int ipc_status, irq_set = BIT(KBD_IRQ), r,irq_timer0_set = BIT(TIMER0_IRQ);
	message msg;
	unsigned long int scancode;
	long int test_scancode;

	//Maps VRAM in the process's address space
	if (vg_init(VIDEO_MODE_105) == NULL) {
		return 1;
	}


	//Subscribes to keyboard interrupts and check if it failed

	if(kbd_subscribe_int() != KBD_IRQ) {
		return 1;
	}


	if (timer_subscribe_int() != TIMER0_IRQ) {

		return 1;

	}

// VERIFICAR INPUT PARA LIMITAR MOVIMENTO A UM DOS EIXOS
	if (xi!=xf && yi!= yf)
	{
		return 1;
	}


	int width,height;
	//char * map = read_xpm (xpm,&width,&height);
	//Waits for the esc key to be released
	//vg_draw_sprite(xpm,xi,yi);

				short x = xi;
				short y = yi;
				short delta_x = xf-xi;
				short delta_y = yf-yi;
	while(scancode != ESC_KEY_BREAKCODE) {





		if(s > 0)
		{
			if( delta_x == 0)
			{
				if(delta_y < 0) // movimento vertical para cima
				{
					if(counter % (60 / f) == 0)
					{

					if (y > yf)
					{



						vg_fill_black(xpm,x,y);
						vg_draw_sprite(xpm, x, y);
						video_dump_fb();

						y -= s;


					}

					}
				}
				else // movimento vertical para baixo
				{
					if(counter % ( 60 / f) == 0)
					{

						if(y < yf)
						{

							vg_fill_black(xpm,x,y);
							vg_draw_sprite(xpm,x,y);
							video_dump_fb();

							y += s;



						}
					}
				}

			}
			else
			{

				if(delta_x > 0) // movimento horizontal para a direita
				{
					if (x < xf)
					{

						vg_fill_black(xpm,x,y);

						vg_draw_sprite(xpm,x,y);
						video_dump_fb();
						x += s;


					}
				}
				else // movimento horizontal para a esquerda
				{
					if(x > xf)
					{

						vg_fill_black(xpm,x,y);
						vg_draw_sprite(xpm,x,y);
						video_dump_fb();
						x -= s;


					}
				}



			}


		}
		else // se speed for menor que 0
		{

		}



		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}

		if (is_ipc_notify(ipc_status)) {

			switch (_ENDPOINT_P(msg.m_source)) {

			case HARDWARE:

				if (msg.NOTIFY_ARG & irq_set) {



					test_scancode=kbd_int_handler();

					if(test_scancode == -1) {

						printf("Interrupt handler was not successful");
						return 1;
					}

					scancode = (unsigned long int) test_scancode;
				}
				if (msg.NOTIFY_ARG & irq_timer0_set) {

					timer_int_handler();

				}

				break;

			default:

				break;

			}
		}
	}

	if(timer_unsubscribe_int()!=OK) {

		return 1;

	}
	//Unsubscribes to keyboard interrupts

	if(kbd_unsubscribe_int()!= 0) {
		return 1;
	}

	//Exits video mode
	if (vg_exit() == 1)
		return 1;
	else
		return 0;
	return 0;

}	

int test_controller() {


	unsigned VbeVersion, VramSize;

	//Initialize the structure with the correct signature

	vbe_controller_info_t vci_p = { .VbeSignature = "VBE2"};

	//Call function Ox4F00 - return vbe controller info and fill the structure with the necessary data

	vbe_get_controller_info(&vci_p);

	//Collect the parameters to print out

	VbeVersion = vci_p.VBeVersion;
	VramSize = vci_p.TotalMemory;

	//Convert the vbe version from bcd to decimal

	unsigned int VbeMajorVersion, VbeMinorVersion;
	VbeMajorVersion = VbeVersion>>(8);
	VbeMajorVersion = VbeMajorVersion & 0x00FF;
	VbeMinorVersion = VbeVersion & 0x00FF;

	//Getting video mode info

	short * VideoModes = vci_p.Reserved;


	//Print out the information requested

	printf("%d.%d\n", VbeMajorVersion, VbeMinorVersion);


	while(*VideoModes != -1) {

		printf("0x%X:", *VideoModes);
		VideoModes++; //need to check this

	}

	printf("\n");
	printf("%d", VramSize*64); //multiply by 64, given that the information is stored in 64kB units


	return 0;
	
}	
	
