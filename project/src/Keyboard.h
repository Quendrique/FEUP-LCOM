#include "i8042.h"
#include "i8254.h"
#include "Bitmap.h"

/** @defgroup Keyboard Keyboard
 * @{
 * Estrutura e funcoes responsaveis pela implementacao do teclado
 */

///Representa a estrutura do teclado
typedef struct
{
	short x_draw_t;
	short y_draw_t;

	short x_draw;
	short y_draw;
	Bitmap * keys[26];
	Bitmap * textbox;

	unsigned int count;
	unsigned long int text[300]; //CALCULAR COM O TAMANHO DO BMP DAS LETRAS O NUMERO DE CARACTERES POSSIVEL

	int string_received;
	int string_received_count;
	unsigned char* string_to_read;
	unsigned long string_to_display[300];
	int string_received_is_completed;


}Keyboard;

/**
 * @brief Inicializa a estrutura do teclado
 *
 * @return Apontador para a estrutura criada
 */

Keyboard * new_kbd();
Keyboard * kbd;

/**
 * @brief Atualiza o buffer grafico (inclui apenas o background da text box)
 *
 * @param kbd Teclado responsavel pela atualizacao grafica
 */

void draw_kbd(Keyboard * kbd);

/**
 * @brief Atualiza o buffer grafico (inclui apenas o texto da text box)
 *
 */

void manamalloc();

/**
 * @brief Atualiza a estrutura do teclado
 *
 * @param scancode Scancode recebido durante a atualizacao do Game
 */

int update_kbd(unsigned long int scancode);

/**
 * @brief Subscreve as interrupcoes do teclado
 *
 * @return 1 se falhar, hook_id se tiver sucesso
 */

int kbd_subscribe_int();

/**
 * @brief Retira a subscricao as interrupcoes do teclado
 *
 * @return 1 se falhar, 0 se tiver sucesso
 */

int kbd_unsubscribe_int();

/**
 * @brief Handler das interrupcoes do teclado
 *
 * @return 1 se falhar, o scancode recebido se tiver sucesso
 */

long int kbd_int_handler();

/**
 * @brief Disponibiliza o estado do teclado
 *
 * @param st Array no qual guardar a informacao
 * @return 1 se falhar, 0 se tiver sucesso
 */

int kbd_get_status(unsigned char *st);
