#include "Bitmap.h"
#include "Button.h"

/** @defgroup GameOver GameOver
 * @{
 * Funcoes responsaveis pela manutencao do estado de fim de jogo
 */

///Representa o estado GameOver
typedef struct {

	Button* play_button;
	Button* exit_button;

	Bitmap* background;
	Bitmap* clear_screen;

	Bitmap* victory_screen;
	Bitmap* defeat_screen;

	Bitmap * numbers[10];

	int screen_cleared;

	int mouse_on_play_button;
	int mouse_on_exit_button;

	int main_menu_chosen;

	int draw;
	int done;


}GameOverState;

/**
 * @brief Cria um novo estado GameOver
 * @return Apontador para esse novo estado
 */

GameOverState* new_game_over_state_victory();

/**
 * @brief Atualiza o buffer gráfico
 * @param game_over_state Apontador para o estado responsavel pelas modificacoes
 */

void draw_game_over_state_victory(GameOverState* game_over_state);

/**
 * @brief Atualiza o estado GameOver
 *
 * @param game_over_state Apontador para o estado a atualizar
 * @return 1 caso seja necessário atualizar o buffer grafico, 0 caso contrario
 */

int update_game_over_state_victory(GameOverState* game_over_state);

/**
 * @brief Liberta a memoria ocupada pelo estado GameOver
 * @param game_over_state Apontador para o estado a apagar
 */

void delete_game_over_state_victory(GameOverState* game_over_state);

/**
 * @brief Cria um novo estado GameOver (Defeat)
 * @return Apontador para esse novo estado
 */

GameOverState* new_game_over_state_defeat();

/**
 * @brief Atualiza o buffer gráfico (Defeat)
 * @param game_over_state Apontador para o estado responsavel pelas modificacoes
 */

void draw_game_over_state_defeat(GameOverState* game_over_state);

/**
 * @brief Atualiza o estado GameOver (Defeat)
 *
 * @param game_over_state Apontador para o estado a atualizar
 * @return 1 caso seja necessário atualizar o buffer grafico, 0 caso contrario
 */

int update_game_over_state_defeat(GameOverState* game_over_state);

/**
 * @brief Liberta a memoria ocupada pelo estado GameOver (Defeat)
 * @param game_over_state Apontador para o estado a apagar
 */

void delete_game_over_state_defeat(GameOverState* game_over_state);

