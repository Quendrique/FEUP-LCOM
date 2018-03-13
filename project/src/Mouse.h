#pragma once

#include "Button.h"
#include "Bitmap.h"
#include "Graphics.h"
#include "i8042.h"

/** @defgroup Mouse Mouse
 * @{
 * Estrutura e funcoes reponsaveis pela implementacao do rato
 */

///Representa o rato
typedef struct {
	int draw;
	short x, y;
	int x_sign, y_sign; // 1 is negative, 0 is positive

	Bitmap * cursor;

	unsigned int packet[3];
	int packet_byte1_check;
	int packet_byte2_check;
	//int packet_byte3_check; not sure if needed

	int lb, rb, mb;
	int lb_released;
} Mouse;

/**
 * @brief Inicializa a estrutura do mouse
 *
 * @return Apontador para a estrutura criada
 */

Mouse* new_mouse();

/**
 * @brief Devolve o apontador para o rato inicializado
 *
 * @return Apontador para a estrutura ja existente
 */

Mouse* get_mouse();

Mouse* mouse;

/**
 * @brief Atualiza o buffer grafico
 *
 * @param mouse Apontador para o rato
 */

void draw_mouse(Mouse* mouse);

/**
 * @brief Atualiza a struct do rato
 *
 * @param mouse Apontador para o rato
 */

void update_mouse(Mouse* mouse);

/**
 * @brief Apaga a struct do mouse
 *
 * @param mouse Apontador para o rato
 */

void delete_mouse(Mouse* mouse);

/**
 * @brief Diz-nos se o botao esquerdo do rato foi pressionado
 *
 * @param mouse Apontador para o rato
 */

int is_lb_pressed(Mouse* mouse);

/**
 * @brief Diz-nos se o botao esquerdo do rato foi liberto
 *
 * @param mouse Apontador para o rato
 */

int was_lb_released(Mouse* mouse);

/**
 * @brief Diz-nos se o rato esta dentro de um botao
 *
 * @param mouse Apontador para o rato
 * @param button Apontador para o botao
 */

int is_mouse_inside_button(Mouse* mouse, Button* button);

/**
 * @brief Subscreve as interrupcoes do rato
 *
 * @return 0 se operacao bem-sucedida, 1 caso contrario
 */

int mouse_subscribe_int();

/**
 * @brief Remove a subscricao as interrupcoes do rato
 *
 * @return 0 se operacao bem-sucedida, 1 caso contrario
 */

int mouse_unsubscribe_int();

/**
 * @brief Devolve o estado do rato
 *
 * @param st Apontador para a estrutura onde serao guardados os dados
 * @return 0 se operacao bem-sucedida, 1 caso contrario
 */

int mouse_get_status(unsigned char *st);

/**
 * @brief Permite receber um command byte do kbc
 *
 * @return O command byte lide
 */

long unsigned int mouse_read_cmd_byte();

/**
 * @brief Permite o envio de um byte para o kbc
 *
 * @param cmd Byte a enviar
 * @return 0 se operacao bem-sucedida, 1 caso contrario
 */

unsigned int mouse_write_byte(unsigned char cmd);

/**
 * @brief Permite o envio de um command byte para o kbc
 *
 * @param cmd Comando a enviar
 * @return 0 se operacao bem-sucedida, 1 caso contrario
 */

unsigned int kbc_write_cmd_byte(unsigned char cmd);

/**
 * @brief Responsavel por lidar com as interrupcoes do rato
 *
 * @return 0 se operacao bem-sucedida, 1 caso contrario
 */

unsigned int mouse_handler();

/**
 * @brief Responsavel por enviar o primeiro byte que deve suceder a qualquer byte enviado par o mouse
 *
 * @return 0 se operacao bem-sucedida, 1 caso contrario
 */

unsigned int mouse_first_cmd();

/**
 * @brief Permite o envio de um byte para o kbc
 *
 * @param cmd Comando a enviar
 * @return 0 se operacao bem-sucedida, 1 caso contrario
 */

unsigned int mouse_write_cmd(unsigned char cmd);

