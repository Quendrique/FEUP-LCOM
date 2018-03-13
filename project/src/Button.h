#pragma once

#include "Bitmap.h"

/** @defgroup Button Button
 * @{
 * Estrutura do botao e funcoes relativas a este
 */

///Representa um botao
typedef struct {
	int xi,yi,xf,yf;
	Bitmap * image_normal;
	Bitmap * image_highlighted;

	// Bitmap* image = (...) needs to be completed

} Button;

/**
 * @brief Cria um novo botao
 *
 * @param xi Coordenada x inicial
 * @param xf Coordenada x final
 * @param yi Coordenada y inicial
 * @param yf Coordenada y final
 *
 * @return Apontador para o botao
 */

Button *new_button (int xi, int xf, int yi, int yf);

/**
 * @brief Desenha um botao
 *
 * @param button Apontador para o botao a desenhar
 */

void draw_button_highlighted(Button* button);

/**
 * @brief Liberta a memoria ocupada pelo botao
 *
 * @param button Apontador para o botao a eliminar
 */

void delete_button (Button* button);
