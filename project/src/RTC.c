#include "RTC.h"
#include <stdio.h>

int rtc_hook_id = RTC_IRQ;

unsigned char rtc_convert_bcd (unsigned char to_convert)
{
	return ((to_convert >> 4) * 10) + (to_convert & 0x0F);
}

int rtc_subscribe_int() {

	int hook_tmp = rtc_hook_id;

	//subscribing to interrupts

	if (sys_irqsetpolicy(RTC_IRQ, IRQ_REENABLE, &rtc_hook_id) == OK) {

		if (sys_irqenable(&rtc_hook_id) == OK)
			return hook_tmp;

	}

	return 1;

}

int rtc_unsubscribe_int() {


	if (sys_irqdisable(&rtc_hook_id) == OK) {

		if (sys_irqrmpolicy(&rtc_hook_id) == OK)

			return 0;

	}

	return 1;

}

int rtc_initialize_global_variables() {

	seconds = (unsigned long*) malloc(sizeof(unsigned long));

	if (seconds == NULL)
		return 1;

	minutes = (unsigned long*) malloc(sizeof(unsigned long));

	if (minutes == NULL)
		return 1;

	hours = (unsigned long*) malloc(sizeof(unsigned long));

	if (hours == NULL)
		return 1;

	day = (unsigned long*) malloc(sizeof(unsigned long));

	if (day == NULL)
		return 1;

	month = (unsigned long*) malloc(sizeof(unsigned long));

	if (month == NULL)
		return 1;

	year = (unsigned long*) malloc(sizeof(unsigned long));

	if (year == NULL)
		return 1;

	return 0;
}

void rtc_delete_global_variables() {
	free(seconds);
	free(minutes);
	free(hours);
	free(day);
	free(month);
	free(year);
}

int rtc_int_handler() {

	unsigned long regC;

	if (sys_outb(RTC_ADDRESS_REG, RTC_REGISTER_C) != OK)
		return 1;

	if (sys_inb(RTC_DATA_REG, &regC) != OK)
		return 1;


	if (regC & RTC_REG_C_UF != 0)
		rtc_get_time_and_date();

	return 0;

}

int rtc_get_time_and_date() {

	unsigned long regA, test, seconds_temp, minutes_temp, year_temp;
	seconds_temp = 0;
	year_temp = 0;
	minutes_temp = 0;

	if (rtc_disable_int() == 1)
		return 1;

	do {

		if (sys_outb(RTC_ADDRESS_REG, RTC_REGISTER_A) != OK)
			return 1;
		if (sys_inb(RTC_DATA_REG, &regA) != OK)
			return 1;

		test = regA & RTC_UPDATE_IN_PROGRESS;

	} while (test != 0);

	if (sys_outb(RTC_ADDRESS_REG, RTC_ADDRESS_SECONDS) != OK)
		return 1;

	if (sys_inb(RTC_DATA_REG, seconds) != OK)
		return 1;

	do {

		if (sys_outb(RTC_ADDRESS_REG, RTC_REGISTER_A) != OK)
			return 1;
		if (sys_inb(RTC_DATA_REG, &regA) != OK)
			return 1;
		test = regA & RTC_UPDATE_IN_PROGRESS;

	} while (test != 0);

	if (sys_outb(RTC_ADDRESS_REG, RTC_ADDRESS_MINUTES) != OK)
		return 1;

	if (sys_inb(RTC_DATA_REG, minutes) != OK)
		return 1;

	do {

		if (sys_outb(RTC_ADDRESS_REG, RTC_REGISTER_A) != OK)
			return 1;
		if (sys_inb(RTC_DATA_REG, &regA) != OK)
			return 1;
		test = regA & RTC_UPDATE_IN_PROGRESS;

	} while (test != 0);

	if (sys_outb(RTC_ADDRESS_REG, RTC_ADDRESS_HOURS) != OK)
		return 1;

	if (sys_inb(RTC_DATA_REG, hours) != OK)
		return 1;

	do {

		if (sys_outb(RTC_ADDRESS_REG, RTC_REGISTER_A) != OK)
			return 1;
		if (sys_inb(RTC_DATA_REG, &regA) != OK)
			return 1;
		test = regA & RTC_UPDATE_IN_PROGRESS;

	} while (test != 0);

	if (sys_outb(RTC_ADDRESS_REG, RTC_ADDRESS_DAY) != OK)
		return 1;

	if (sys_inb(RTC_DATA_REG, day) != OK)
		return 1;

	do {

		if (sys_outb(RTC_ADDRESS_REG, RTC_REGISTER_A) != OK)
			return 1;
		if (sys_inb(RTC_DATA_REG, &regA) != OK)
			return 1;
		test = regA & RTC_UPDATE_IN_PROGRESS;

	} while (test != 0);

	if (sys_outb(RTC_ADDRESS_REG, RTC_ADDRESS_MONTH) != OK)
		return 1;

	if (sys_inb(RTC_DATA_REG, month) != OK)
		return 1;

	do {

		if (sys_outb(RTC_ADDRESS_REG, RTC_REGISTER_A) != OK)
			return 1;
		if (sys_inb(RTC_DATA_REG, &regA) != OK)
			return 1;
		test = regA & RTC_UPDATE_IN_PROGRESS;

	} while (test != 0);

	if (sys_outb(RTC_ADDRESS_REG, RTC_ADDRESS_YEAR) != OK)
		return 1;

	if (sys_inb(RTC_DATA_REG, year) != OK)
		return 1;


	if (rtc_enable_int() == 1)
		return 1;

	*day = rtc_convert_bcd(*day);
	*month = rtc_convert_bcd(*month);
	*year = rtc_convert_bcd(*year);

	return 0;

}

int rtc_disable_int() {

	unsigned long regB;

	if (sys_outb(RTC_ADDRESS_REG, RTC_REGISTER_B) != OK)
		return 1;

	if (sys_inb(RTC_DATA_REG, &regB) != OK)
		return 1;

	//sets the bits responsible for enabling interrupts to 1

	regB |= 0x0080;

	if (sys_outb(RTC_ADDRESS_REG, RTC_REGISTER_B) != OK)
		return 1;

	if (sys_outb(RTC_DATA_REG, regB) != OK)
		return 1;

	//printf("reg b after disable: %x", regB);

	return 0;
}

int rtc_enable_int() {

	unsigned long regB;

	if (sys_outb(RTC_ADDRESS_REG, RTC_REGISTER_B) != OK)
		return 1;

	if (sys_inb(RTC_DATA_REG, &regB) != OK)
		return 1;

	//sets the bits responsible for enabling interrupts to 0

	regB &= 0xFF7F;
	regB |= 0x0070;

	if (sys_outb(RTC_ADDRESS_REG, RTC_REGISTER_B) != OK)
		return 1;

	if (sys_outb(RTC_DATA_REG, regB) != OK)
		return 1;

	return 0;
}

