#pragma once

#include "Bitmap.h"
#include "Button.h"
#include "Timer.h"

/** @defgroup MainMenu MainMenu
 * @{
 * Estrutura e funcoes responsaveis pela manutencao do menu principal
 */

typedef enum {
	PLAY_1, PLAY_2, EXIT_GAME
} Action;

///Representa o menu principal
typedef struct {

	int done;

	Bitmap * mm_background;

	//Button* play_button;
	Button* exit_button;

	Button* player1_button;
	Button* player2_button;

	//int mouse_on_play;
	int mouse_on_player1;
	int mouse_on_player2;
	int mouse_on_exit;

	int player_id_sent;

	int other_player_selected;
	int player_selected;

	int action;

} MainMenuState;

/**
 * @brief Cria um novo menu principal
 *
 * @return Apontador para o menu principal criado
 */

MainMenuState* new_main_menu_state ();

/**
 * @brief Atualiza o estado MainMenu
 *
 * @param main_menu Apontador para o estado a atualizar
 * @return 1 caso seja necessário atualizar o buffer grafico, 0 caso contrario
 */

int update_main_menu_state(MainMenuState* main_menu);

/**
 * @brief Atualiza o buffer gráfico
 * @param main_menu Apontador para o estado responsavel pelas modificacoes
 */

void draw_main_menu_state(MainMenuState* main_menu);

/**
 * @brief Liberta a memoria ocupada pelo estado MainMenu
 * @param main_menu Apontador para o estado a apagar
 */

void delete_main_menu_state(MainMenuState* main_menu);

