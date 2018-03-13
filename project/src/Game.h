#include "Bitmap.h"

typedef enum {
	MAIN_MENU_STATE, GAMEPLAY_STATE, GAME_OVER_STATE
} State;

/** @defgroup Game Game
 * @{
 * Funcoes responsaveis pela principal maquina de estados do jogo
 */

/// Represents a Game structure
typedef struct {
	int IRQ_SET_KBD, IRQ_SET_MOUSE, IRQ_SET_TIMER, IRQ_SET_SERIAL, IRQ_SET_RTC;
	int done, draw;
	unsigned long int kbd_scancode;

	int frame_passed;

	int player_selected;

	int victory; // 1 - won, 2 - lost

	State current_state;
	void* state;

} Game;

/**
 * @brief Cria e inicializa um game
 * @return Apontador para o game criado
 */

Game* start_game();

/**
 * @brief Atualiza o jogo
 */

void update_game();

/**
 * @brief De acordo com o estado atual, atualiza o buffer gráfico
 */

void draw_game();

/**
 * @brief Liberta a memoria ocupada pelos elementos do game e do estado atual
 */

void delete_game();

/**
 * @brief Modifica o estado do jogo de acordo com os events ocorridos
 * @param game Apontador para o game criado
 * @param new_state Estado para o qual devera mudar
 */

void change_state(Game* game, State new_state);

/**
 * @brief Atualiza o estado atual
 * @param game Apontador para o game criado
 */

void update_current_state(Game* game);

/**
 * @brief Liberta a memoria ocupada pela estrutura pertencente ao estado atual
 * @param game Apontador para o game criado
 */

void delete_current_state(Game* game);
