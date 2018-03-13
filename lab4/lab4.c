#include <limits.h>
#include <string.h>
#include <errno.h>
#include "test4.h"

static int proc_args(int argc, char **argv);
static unsigned long parse_ulong(char *str, int base);
static void print_usage(char **argv);

int main(int argc, char **argv)
{
	sef_startup();

	if (argc == 1) {						/* Prints usage of the program if no arguments are passed */
		print_usage(argv);
		return 0;
	}
	else return proc_args(argc, argv);
}

static void print_usage(char **argv)
{
	printf("Usage: one of the following:\n"
			"\t service run %s -args \"packet <decimal no. - no. of packets to receive>\"\n"
			"\t service run %s -args \"async <decimal no. - no. of seconds w/o packets before exiting>\"\n"
			"\t service run %s -args \"remote <decimal no. - period ; decimal no. - no. of packets to receive>\"\n"
			"\t service run %s -args \"gesture <decimal no. - length of the movement>\"\n",
			argv[0], argv[0], argv[0], argv[0]);
}

static int proc_args(int argc, char **argv)
{
	unsigned long cnt, idle_time, period, length;
	if (strncmp(argv[1], "packet", strlen("packet")) == 0) {
		if (argc != 3) {
			printf("packet: wrong number of arguments for mouse_test_packet()\n");
			return 1;
		}
		cnt = parse_ulong(argv[2], 10);						/* Parses string to unsigned long */
		if (cnt == ULONG_MAX)
			return 1;
		printf("mouse::mouse_test_packet(%lu)\n", cnt);
		return mouse_test_packet(cnt);
	}

	else if (strncmp(argv[1], "async", strlen("async")) == 0) {
		if (argc != 3) {
				printf("async: wrong number of arguments for mouse_test_async()\n");
				return 1;
		}
		idle_time = parse_ulong(argv[2], 10);						/* Parses string to unsigned long */
		if (idle_time == ULONG_MAX)
			return 1;
		printf("mouse::mouse_test_async(%lu)\n", idle_time);
		return mouse_test_async(idle_time);

	}

	else if (strncmp(argv[1], "remote", strlen("remote")) == 0) {
		if (argc != 4) {
			printf("remote: wrong number of arguments for mouse_test_remote()\n");
			return 1;
		}
		period = parse_ulong(argv[2], 10);					/* Parses string to unsigned long */
		cnt = parse_ulong(argv[3], 10);
		if (period == ULONG_MAX || cnt == ULONG_MAX)
			return 1;
		printf("mouse::mouse_test_remote(%lu, %lu)\n", period, cnt);
		return mouse_test_remote(period, cnt);
	}

	else if (strncmp(argv[1], "gesture", strlen("gesture")) == 0) {
		if (argc != 3) {
			printf("gesture: wrong number of arguments for mouse_test_gesture()\n");
			return 1;
		}
		length = parse_ulong(argv[2], 10);					/* Parses string to unsigned long */
		if (length == ULONG_MAX)
			return 1;
		printf("mouse::mouse_test_gesture(%lu)\n", length);
		return mouse_test_gesture(length);;
	}

	else {
		printf("mouse: %s - not a valid function\n", argv[1]);
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
