#include <limits.h>
#include <string.h>
#include <errno.h>
#include "test5.h"
#include "pixmap.h"
#include <minix/sysutil.h>
#include <minix/drivers.h>
#include <minix/syslib.h>

static int proc_args(int argc, char **argv);
static unsigned long parse_ulong(char *str, int base);
static void print_usage(char **argv);
static unsigned short parse_ushort(char *str, int base);

int main(int argc, char **argv)
{
	sef_startup();

	if (argc == 1) {						/* Prints usage of the program if no arguments are passed */
		print_usage(argv);
		return 0;
	}
	else
		return proc_args(argc, argv);
}

static void print_usage(char **argv)
{
	printf("Usage: one of the following:\n"
			"\t service run %s -args \"init <...>\"\n"
			"\t service run %s -args \"square <...>\"\n"
			"\t service run %s -args \"line <...>\"\n"
			"\t service run %s -args \"xpm <...>\"\n"
			"\t service run %s -args \"move <...>\"\n"
			"\t service run %s -args \"controller <no args>\"\n",
			argv[0], argv[0], argv[0], argv[0]);
}

static int proc_args(int argc, char **argv)
{
	unsigned short mode, delay, x, y, size, xi, yi, xf, yf, frame_rate;
	short speed;
	unsigned long color;
	unsigned int xpm ; //NEED TO FIX THIS

	if (strncmp(argv[1], "init", strlen("init")) == 0) {
		if (argc != 4) {
			printf("init: wrong number of arguments for video_test_init()\n");
			return 1;
		}
		mode = parse_ushort(argv[2], 10);	// parses string to unsigned short
		if (mode == USHRT_MAX)
			return 1;
		delay = parse_ushort(argv[3], 10); // parses string to unsigned short
		if (delay == USHRT_MAX)
			return 1;
		printf("video::video_test_init(%hu, %hu)\n", mode, delay);
		return video_test_init(mode, delay);
	}

	else if (strncmp(argv[1], "square", strlen("square")) == 0) {
		if (argc != 6) {
				printf("square: wrong number of arguments for video_test_square()\n");
				return 1;
		}
		x =  parse_ushort(argv[2], 10);
		if (x == USHRT_MAX)
			return 1;
		y = parse_ushort(argv[3], 10);
		if (y == USHRT_MAX)
			return 1;
		size = parse_ushort(argv[4], 10);
		if (size == USHRT_MAX)
			return 1;
		color = parse_ulong(argv[5], 10);
		if (color == ULONG_MAX)
			return 1;
		printf("video::video_test_square(%hu, %hu, %hu, %hu)", x, y, size, color);
		return video_test_square(x ,y , size, color);

	}

	else if (strncmp(argv[1], "line", strlen("line")) == 0) {
		if (argc != 7) {
			printf("line: wrong number of arguments for video_test_line()\n");
			return 1;
		}

		xi = parse_ushort(argv[2], 10); // parses string to unsigned short
		if (xi == USHRT_MAX)
			return 1;
		yi = parse_ushort(argv[3], 10);
		if (yi == USHRT_MAX)
			return 1;
		xf = parse_ushort(argv[4], 10);
		if (xf == USHRT_MAX)
			return 1;
		yf = parse_ushort(argv[5], 10);
		if (yf == USHRT_MAX)
			return 1;
		color = parse_ulong(argv[6], 10); // parses string to unsigned long
		if (color == ULONG_MAX)
			return 1;
		printf("video::video_test_line(%hu, %hu, %hu, %hu, %lu)\n", xi, yi, xf, yf, color);
		return video_test_line(xi, yi, xf, yf, color);
	}

	else if (strncmp(argv[1], "xpm", strlen("xpm")) == 0) {

		if (argc != 5) {
			printf("xpm: wrong number of arguments for video_test_xpm()\n");
			return 1;
		}

		xpm = parse_ushort(argv[2], 10); //dont know if its right
		xi = parse_ushort(argv[3], 10); //needs to be converted to unsigned short
 		yi = parse_ushort(argv[4], 10); // ""
		printf("video::video_test_xpm(%d, %hu, %hu)\n", xpm , xi, yi);
		switch(xpm)
				{
				case 1:
					return test_xpm(pic1,xi,yi);
					break;
				case 2:
					return test_xpm(pic2,xi,yi);
					break;
				case 3:
					return test_xpm(pic3,xi,yi);
					break;
				case 4:
					return test_xpm(penguin,xi,yi);
					break;
				case 5:
					return test_xpm(cross,xi,yi);

				}


	}

	else if (strncmp(argv[1], "move", strlen("move")) == 0) {

		if (argc != 9) {
			printf("move: wrong number of arguments for video_test_line()\n");
			return 1;
		}

		xpm = parse_ushort(argv[2], 10); //NEED TO FIX THIS
		xi = parse_ushort(argv[3], 10); //needs to be converted to unsigned short
		yi = parse_ushort(argv[4], 10); //""
		xf = parse_ushort(argv[5], 10);
		yi = parse_ushort(argv[6], 10);
		speed = parse_ushort(argv[7], 10); //needs to be converted to short ??
		frame_rate = parse_ushort(argv[8], 10);
		printf("video::video_test_move(%p, %hu, %hu, %hu, %hu, %d, %d)\n", xpm, xi, yi, xf, yf, speed, frame_rate);
		switch(xpm)
				{
				case 1:
					return test_move(pic1 ,xi, yi, xf, yf, speed, frame_rate);
					break;
				case 2:
					return test_move(pic2 ,xi, yi, xf, yf, speed, frame_rate);
					break;
				case 3:
					return test_move(pic3 ,xi, yi, xf, yf, speed, frame_rate);
					break;
				case 4:
					return test_move(penguin ,xi, yi, xf, yf, speed, frame_rate);
					break;
				case 5:
					return test_move(cross ,xi, yi, xf, yf, speed, frame_rate);

				}


	}

	else if (strncmp(argv[1], "controller", strlen("controller")) == 0) {
		return test_controller();
	}

	else {
		printf("video: %s - not a valid function\n", argv[1]);
		return 1;
	}
}

static unsigned long parse_ulong(char *str, int base)
{
	char *endptr;
	unsigned long val;

	/* Convert string to unsigned long */
	val = strtoul(str, &endptr, base);

	/* Check for conversion errors */
	if ((errno == ERANGE && val == ULONG_MAX) || (errno != 0 && val == 0)) {
		perror("strtoul");
		return ULONG_MAX;
	}

	if (endptr == str) {
		printf("mouse: parse_ulong: no digits were found in %s\n", str);
		return ULONG_MAX;
	}

	/* Successful conversion */
	return val;
}

static unsigned short parse_ushort(char *str, int base)
{
	char *endptr;
	unsigned short val;

	/* Convert string to unsigned short */
	val = atoi(str);

	/* Check for conversion errors */
	if ((errno == ERANGE && val == USHRT_MAX) || (errno != 0 && val == 0)) {
		perror("atoi");
		return USHRT_MAX;
	}

	if (endptr == str) {
		printf("mouse: parse_ushort: no digits were found in %s\n", str);
		return USHRT_MAX;
	}

	/* Successful conversion */
	return val;
}
