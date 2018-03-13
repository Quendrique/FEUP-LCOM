#pragma once

#include "Timer.h"
#include "Puzzle.h"
#include "Bitmap.h"
#include "Graphics.h"
#include "Button.h"

typedef enum {
	WON, LOST
} ActionGameplay;

/** @defgroup Gameplay Gameplay
 * @{
 * Funcoes responsaveis pela manutencao do estado de jogo
 */

///Representa o estado de jogo
typedef struct {

	WirePuzzle* puzzle_1;
	ButtonPuzzle* puzzle_2;
	WordPuzzle* puzzle_3;
	NumbersPuzzle* puzzle_4;
	Puzzle5* puzzle_5;
	Puzzle6* puzzle_6;
	//missing 2 puzzles


	int puzzle_1_done;
	int puzzle_2_done;
	int puzzle_3_done;
	int puzzle_4_done;
	int puzzle_5_done;
	int puzzle_6_done;

	int strike_num;
	//Bitmap * strikes[4];

	int all_puzzles;

	Timer* timer;
	//Keyboard* kbd;

	//PLAYER 2

	/* BITMAP MANUALS
	 *
	 * */

	Bitmap* background;
	Bitmap* clear_screen;

	int screen_cleared;

	int page_viewed;
	Bitmap* pages_available[3];

	Button* next_page;
	Button* previous_page;

	int mouse_on_next_page;
	int mouse_on_previous_page;

	int draw;
	int done;
	ActionGameplay action;

	//TEMP

	Bitmap* test_background;


} GameplayState;

/**
 * @brief Cria um novo estado de jogo (Player 1)
 * @return Apontador para o novo estado de jogo criado
 */

GameplayState* new_gameplay_state_player1();

/**
 * @brief Atualiza o buffer grafico (Player 1)
 * @param gameplay_state Apontador do estado de jogo responsavel pelas mudancas
 */

void draw_gameplay_state_player1(GameplayState* gameplay_state);

/**
 * @brief Atualiza o estado de jogo (Player 1)
 *
 * @param gameplay_state Apontador do estado de jogo a atualizar
 * @return 1 caso seja necessário atualizar o buffer grafico, 0 caso contrario
 */

int update_gameplay_state_player1(GameplayState* gameplay_state);

/**
 * @brief Liberta memoria ocupada pelo estado de jogo (Player 1)
 * @param gameplay_state Apontador do estado de jogo a eliminar
 */

void delete_gameplay_state_player1(GameplayState* gameplay_state);

/**
 * @brief Cria um novo estado de jogo (Player 2)
 * @return Apontador para o novo estado de jogo criado
 */


GameplayState* new_gameplay_state_player2();

/**
 * @brief Atualiza o buffer grafico (Player 2)
 * @param gameplay_state Apontador do estado de jogo responsavel pelas mudancas
 */

void draw_gameplay_state_player2(GameplayState* gameplay_state);

/**
 * @brief Atualiza o estado de jogo (Player 2)
 *
 * @param gameplay_state Apontador do estado de jogo a atualizar
 * @return 1 caso seja necessário atualizar o buffer grafico, 0 caso contrario
 */

int update_gameplay_state_player2(GameplayState* gameplay_state);

/**
 * @brief Liberta memoria ocupada pelo estado de jogo (Player 2)
 * @param gameplay_state Apontador do estado de jogo a eliminar
 */

void delete_gameplay_state_player2(GameplayState* gameplay_state);





