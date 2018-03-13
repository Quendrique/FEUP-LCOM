#pragma once
#include "Aux.h"

/** @defgroup RTC RTC
 * @{
 * Funcoes responsaveis pelo uso do RTC
 */

unsigned long* seconds;
unsigned long* minutes;
unsigned long* hours;
unsigned long* day;
unsigned long* month;
unsigned long* year;

#define RTC_IRQ		8

#define RTC_ADDRESS_REG		0x70
#define RTC_DATA_REG		0x71

#define RTC_REGISTER_A		0x0A
#define RTC_REGISTER_B		0x0B
#define RTC_REGISTER_C		0x0C
#define RTC_REGISTER_D		0x0D

#define RTC_REG_B_AIE		BIT(5)
#define RTC_REG_B_PIE		BIT(6)
#define RTC_REG_B_UIE		BIT(4)

#define RTC_REG_C_PF		BIT(6)
#define RTC_REG_C_AF		BIT(5)
#define RTC_REG_C_UF		BIT(4)

#define RTC_ADDRESS_SECONDS		0
#define RTC_ADDRESS_MINUTES		2
#define RTC_ADDRESS_HOURS		4
#define RTC_ADDRESS_DAY			7
#define RTC_ADDRESS_MONTH		8
#define RTC_ADDRESS_YEAR		9

#define RTC_UPDATE_IN_PROGRESS	BIT(7);

/**
 * @brief Subscreve as interrupcoes do RTC
 *
 * @return 0 se a operacao tiver sucesso, 1 caso contrario
 */

int rtc_subscribe_int();

/**
 * @brief Cancela as subscricoes as interrupcoes do RTCS
 *
 * @return 0 se a operacao tiver sucesso, 1 caso contrario
 */

int rtc_unsubscribe_int();

/**
 * @brief Lida com as interrupcoes do RTC
 *
 * @return 0 se a operacao tiver sucesso, 1 caso contrario
 */

int rtc_int_handler();

/**
 * @brief Recorre a polling para obter a data e hora atual
 *
 * @return 0 se a operacao tiver sucesso, 1 caso contrario
 */

int rtc_get_time_and_date();

/**
 * @brief Desativa as interrupcoes do RTC
 *
 * @return 0 se a operacao tiver sucesso, 1 caso contrario
 */

int rtc_disable_int();

/**
 * @brief Ativa as interrupcoes do RTC
 *
 * @return 0 se a operacao tiver sucesso, 1 caso contrario
 */

int rtc_enable_int();

/**
 * @brief Inicializa as variaveis globais responsaveis por guardar a data e hora
 *
 * @return 0 se a operacao tiver sucesso, 1 caso contrario
 */

int rtc_initialize_global_variables();

/**
 * @brief Apaga as variaveis globais
 *
 * @return 0 se a operacao tiver sucesso, 1 caso contrario
 */

void rtc_delete_global_variables();
