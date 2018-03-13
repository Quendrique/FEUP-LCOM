#pragma once

#include "Bitmap.h"

long unsigned int timer_counter; //global timer_counter

/** @defgroup Timer Timer
 * @{
 * Funcoes e estrutura necessarias para a implementacao do timer
 */

///Representa o timer
typedef struct {

	short seconds_1;
	short seconds_2;

	short minutes_1;

	int draw;
	int done;

	//	Bitmap* test1;
	//	Bitmap* test2;
	Bitmap * dois_pontos;
	Bitmap * numbers[10];
	Bitmap * background;



} Timer;

/**
 * @brief Inicializa o contador global
 */
void initialize_timer_counter();

/**
 * @brief Cria um novo timer
 *
 * @return Apontador para o timer criado
 */

Timer* new_timer();

/**
 * @brief Atualiza a estrutura do timer e determina se e necessario atualizar o buffer grafico
 *
 * @param timer Timer a atualizar
 */

void update_timer(Timer* timer);

/**
 * @brief Atualiza o buffer grafico
 *
 * @param timer Timer a atualizar
 */

void draw_timer(Timer* timer);

/**
 * @brief Liberta a memoria ocupada pela estrutura do timer
 *
 * @param timer Timer a remover
 */

void delete_timer(Timer* timer);

/**
 * @brief Permite escolher a frequencia do timer
 *
 * @param timer Timer a modificar
 * @param freq Frequencia pretendida
 *
 * @return 0 se operacao tiver sucesso, 1 caso contrario
 */

int timer_set_frequency(unsigned char timer, unsigned long freq);

/**
 * @brief Subscreve as atualizacoes do timer
 *
 * @return 0 se tiver sucesso, 1 caso contrario
 */

int timer_subscribe_int();

/**
 * @brief Cancela a subscricao as atualizacoes do timer
 *
 * @return 0 se tiver sucesso, 1 caso contrario
 */

int timer_unsubscribe_int();

/**
 * @brief Lida com as interrupcoes do timer
 *
 * @return 0 se tiver sucesso, 1 caso contrario
 */

int timer_int_handler();


/**
 * @brief Obtem a configuracao do timer
 *
 * @param timer Timer (0,1,2) do qual se pretende obter a configuracao
 * @param st Estrutura que permite guardar a informacao
 * @return 0 se tiver sucesso, 1 caso contrario
 */


int timer_get_conf(unsigned char timer, unsigned char *st);
