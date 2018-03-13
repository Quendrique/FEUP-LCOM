#pragma once

#include <minix/syslib.h>
#include <minix/drivers.h>
#include "Aux.h"
#include "Queue.h"
#include "MainMenu.h"

#include "Gameplay.h"

/** @defgroup SerialPort SerialPort
 * @{
 * Funcoes responsaveis pelo funcionamento da implementacao da serial port
 */

queue* serial_transmitter_queue;
queue* serial_receiver_queue;

#define DELAY_US_SERIAL		200

#define SERIAL_IRQ_COM1		4
#define SERIAL_IRQ_COM2		3

#define COM1_BASE_ADDRESS	0x3F8
#define COM2_BASE_ADDRESS	0x2F8

#define UART_THR_OFFSET		0	//transmitter holding buffer
#define UART_RBR_OFFSET		0x00	//receiver buffer
#define UART_DLL_OFFSET		0x00	//divisor latch low byte
#define UART_IER_OFFSET		0x01	//interrupt enable register
#define UART_DLH_OFFSET		0x01	//divisor latch high byte
#define UART_IIR_OFFSET		0x02	//interrupt identification register
#define UART_FCR_OFFSET		0x02	//fifo control register
#define UART_LCR_OFFSET		0x03	//line control register
#define UART_LSR_OFFSET		0x05	//line status register

#define IER_ENABLE_RECEIVED_DATA		BIT(0)
#define IER_ENABLE_TRANSMITTER_EMPTY	BIT(1)
#define IER_ENABLE_RECEIVER_LINE_STATUS	BIT(2)

#define IIR_TRANSMITTER_EMPTY	BIT(1)
#define IIR_RECEIVED_DATA		BIT(2)
#define IIR_INTERRUPT_PENDING	BIT(0)

#define FCR_ENABLE			BIT(0)
#define FCR_CLEAR_RECEIVE	BIT(1)
#define FCR_CLEAR_TRANSMIT	BIT(2)
#define FCR_TRIGGER_LEVEL_8	BIT(7)
#define FCR_CLEAR_TRANSMIT	BIT(2)
#define FCR_CLEAR_RECEIVE	BIT(1)

#define LCR_WORD_LENGTH_8	BIT(0) | BIT(1)
#define LCR_PARITY_ODD		BIT(3)
#define LCR_LOAD_DL			BIT(7)

#define LSR_DATA_READY			BIT(0)
#define LSR_TRANSMITTER_EMPTY 	BIT(5)
#define LSR_ERROR_FIFO			BIT(7)
#define LSR_FRAMING_ERROR		BIT(3)
#define LSR_PARITY_ERROR		BIT(2)
#define LSR_OVERRUN_ERROR		BIT(1)

#define MAX_BITRATE			115200

/**
 * @brief Subscreve as interrupcoes da serial port
 *
 * @return 0 se a operacao tive sucesso, 1 caso contrario
 */

int serial_subscribe_int();

/**
 * @brief Cancela as subscricoes as interrupcoes da serial port
 *
 * @return 0 se a operacao tive sucesso, 1 caso contrario
 */

int serial_unsubscribe_int();

/**
 * @brief Configura a serial port
 *
 * @return 0 se a operacao tive sucesso, 1 caso contrario
 */

int serial_set_conf();

/**
 * @brief Lida com as interrupcoes da serial port
 *
 * @return 0 se a operacao tive sucesso, 1 caso contrario
 */

int serial_int_handler();

/**
 * @brief Envia uma string para a queue
 *
 * @param string String a ser enviada
 * @return 0 se a operacao tive sucesso, 1 caso contrario
 */

int serial_transmitter_string_to_queue(unsigned char* string);

/**
 * @brief Recebe uma string da queue
 *
 * @param string Estrutura para guardar a string recebida
 * @return 0 se a operacao tive sucesso, 1 caso contrario
 */

int serial_receiver_string_from_queue(unsigned char** string);

/**
 * @brief Limpa a queue de transmissao da UART
 *
 * @return 0 se a operacao tive sucesso, 1 caso contrario
 */

int serial_transmitter_send_string();

/**
 * @brief Limpa a queue de rececao da UART
 *
 * @return 0 se a operacao tive sucesso, 1 caso contrario
 */

int serial_receiver_receive_string();

/**
 * @brief Fornece o line status register
 *
 * @return Line status register
 */

unsigned long serial_get_lsr();

/**
 * @brief Inicializa a serial port
 *
 * @return 0 se a operacao tive sucesso, 1 caso contrario
 */

int serial_initialize();



