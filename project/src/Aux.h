#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/driver.h>
#include <minix/sysutil.h>
#include <minix/ipc.h>

#define SCREEN_X_RESOLUTION		1024
#define SCREEN_Y_RESOLUTION		768

#define DELAY_US		20000

#define BIT(n) (0x01<<(n))
