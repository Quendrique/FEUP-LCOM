#pragma once

#include "Bitmap.h"
#include "Button.h"

/** @defgroup Puzzle Puzzle
 * @{
 * Funcoes e estrutura de dados relativas aos puzzles
 */

///Representa um "wire puzzle"
typedef struct {

	int completed;
	int solution;


	Button * wire_1;
	Button * wire_2;
	Button * wire_3;

	int mouse_on_w1;
	int mouse_on_w2;
	int mouse_on_w3;


	Bitmap * led;
	Bitmap* test;

	int wrong; //if the user chooses the wrong wire, one "life" will be subtracted

	int draw;

} WirePuzzle;

/**
 * @brief Cria um novo puzzle
 *
 * @return Apontador para o puzzle criado
 */

WirePuzzle* new_wire_puzzle();

/**
 * @brief Atualiza o buffer grafico
 *
 * @param puzzle Apontador para o puzzle
 */

void draw_wire_puzzle(WirePuzzle* puzzle);

/**
 * @brief Atualiza a struct do puzzle
 *
 * @param puzzle Apontador para o puzzle
 * @return 1 se for necessario atualizar o buffer grafico, 0 caso contrario
 */

int update_wire_puzzle(WirePuzzle* puzzle);

/**
 * @brief Liberta a memoria ocupada pelo puzzle
 *
 * @param puzzle Apontador para o puzzle
 */

void delete_wire_puzzle(WirePuzzle* puzzle);


///Representa um "button puzzle"
typedef struct {
	int completed;
	//int solution;
	//ButtonColor solution;


	Button* button_1;//green
	Button* button_2;//red
	Button* button_3;//blue

	int mouse_on_b1;
	int mouse_on_b2;
	int mouse_on_b3;


	Bitmap* test;
	Bitmap * led;

	int wrong; //if the user chooses the wrong button, one "life" will be subtracted

	int draw;

} ButtonPuzzle;

/**
 * @brief Cria um novo puzzle
 *
 * @return Apontador para o puzzle criado
 */

ButtonPuzzle* new_button_puzzle();

/**
 * @brief Atualiza a struct do puzzle
 *
 * @param puzzle Apontador para o puzzle
 * @param solution Solucao a implementar
 * @return 1 se for necessario atualizar o buffer grafico, 0 caso contrario
 */

int update_button_puzzle(ButtonPuzzle* puzzle, int solution);

/**
 * @brief Atualiza o buffer grafico
 *
 * @param puzzle Apontador para o puzzle
 */

void draw_button_puzzle(ButtonPuzzle* puzzle);

/**
 * @brief Liberta a memoria ocupada pelo puzzle
 *
 * @param puzzle Apontador para o puzzle
 */

void delete_button_puzzle(ButtonPuzzle* puzzle);

///Representa um "word puzzle"
typedef struct {
	int completed;

	Button* button_1;
	Button* button_2;
	Button* button_3;
	Button* button_4;
	Button* button_5;
	Button* button_6;

	int mouse_on_b1;
	int mouse_on_b2;
	int mouse_on_b3;
	int mouse_on_b4;
	int mouse_on_b5;
	int mouse_on_b6;

	Bitmap* test;
	Bitmap * led;
	Bitmap * hint;

	int wrong; //if the user chooses the wrong button, one "life" will be subtracted

	int draw;

} WordPuzzle;

/**
 * @brief Cria um novo puzzle
 *
 * @return Apontador para o puzzle criado
 */

WordPuzzle* new_word_puzzle();

/**
 * @brief Atualiza o buffer grafico
 *
 * @param puzzle Apontador para o puzzle
 */


void draw_word_puzzle(WordPuzzle* puzzle);

/**
 * @brief Atualiza a struct do puzzle
 *
 * @param puzzle Apontador para o puzzle
 * @return 1 se for necessario atualizar o buffer grafico, 0 caso contrario
 */

int update_word_puzzle(WordPuzzle* puzzle);

/**
 * @brief Liberta a memoria ocupada pelo puzzle
 *
 * @param puzzle Apontador para o puzzle
 */

void delete_word_puzzle();

///Representa um "numbers puzzle"
typedef struct {
	int completed;


	Button* button_1;
	Button* button_2;
	Button* button_3;
	Button* button_4;


	//solution can be implied on the status of the next variables ?
	int mouse_on_b1;
	int mouse_on_b2;
	int mouse_on_b3;
	int mouse_on_b4;

	Bitmap * led;
	Bitmap* test;
	Bitmap * hint;

	int wrong; //if the user chooses the wrong button, one "life" will be subtracted

	int draw;

} NumbersPuzzle;

/**
 * @brief Cria um novo puzzle
 *
 * @return Apontador para o puzzle criado
 */

NumbersPuzzle* new_numbers_puzzle();

/**
 * @brief Atualiza a struct do puzzle
 *
 * @param puzzle Apontador para o puzzle
 * @return 1 se for necessario atualizar o buffer grafico, 0 caso contrario
 */

int update_numbers_puzzle(NumbersPuzzle* puzzle);

/**
 * @brief Atualiza o buffer grafico
 *
 * @param puzzle Apontador para o puzzle
 */

void draw_numbers_puzzle(NumbersPuzzle* puzzle);

/**
 * @brief Liberta a memoria ocupada pelo puzzle
 *
 * @param puzzle Apontador para o puzzle
 */

void delete_numbers_puzzle(NumbersPuzzle* puzzle);

typedef struct {

	Bitmap* test;

	int completed;
	int draw;
	int wrong;

} Puzzle5;

Puzzle5* new_puzzle5_puzzle();
int update_puzzle5_puzzle();
void draw_puzzle5_puzzle();
void delete_puzzle5_puzzle();

typedef struct {

	Bitmap* test;

	int completed;
	int draw;
	int wrong;

} Puzzle6;

Puzzle6* new_puzzle6_puzzle();
int update_puzzle6_puzzle();
void draw_puzzle6_puzzle();
void delete_puzzle6_puzzle();
