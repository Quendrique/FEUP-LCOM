#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

/** @defgroup Queue Queue
 * @{
 * Funcoes e estruturas de dados relativas a queue
 */

typedef struct queue_node queue_node;

///Representa um elemento de uma queue
struct queue_node {
	void * elem;
	queue_node* next;
};

///Representa uma queue
typedef struct {
	queue_node* first;
	queue_node* last;
	unsigned int size;
} queue;


/**
 * @brief Cria uma nova queue
 *
 * @return Apontador para a queue criada
 *
 */

queue* new_queue();

/**
 * @brief Determina o tamanho de uma queue
 *
 * @param q Queue em questao
 * @return Tamanho da queue
 */

unsigned int queue_size(queue* q);

/**
 * @brief Determina se a queue esta vazia ou nao
 *
 * @param q Queue em questao
 * @return true se vazia, false caso contrario
 */

bool is_queue_empty(queue* q);

/**
 * @brief Remove o primeiro elemento da queue
 *
 * @param q Queue em questao
 * @return Apontador para o elemento retirado
 */

void* queue_pop(queue* q);

/**
 * @brief Insere um elemento da queue
 *
 * @param q Queue em questao
 * @param elem elemento a inserir
 * @return true se a operacao tiver sucesso, false caso contrario
 */

bool queue_push(queue* q, void* elem);

/**
 * @brief Permite consultar o primeiro elemento da queue
 *
 * @param q Queue em questao
 * @return Apontador para o elemento consultado
 */

void* queue_top(queue* q);

/**
 * @brief Liberta o espaco de memoria ocupado pela queue
 *
 * @param q Queue em questao
 */

void queue_delete(queue* q);
