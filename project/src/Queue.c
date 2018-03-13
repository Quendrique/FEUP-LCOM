#include "Queue.h"

queue* new_queue() {
	queue * new_queue = (queue*) malloc(sizeof(queue));
	new_queue->first = NULL;
	new_queue->last = NULL;
	new_queue->size = 0;
	return new_queue;

}

unsigned int queue_size(queue* q) {
	return q->size;
}

bool is_queue_empty(queue* q) {
	if (queue_size(q) == 0)
		return true;
	else
		return false;
}

bool queue_push(queue* q, void* elem) {

	queue_node* node = (queue_node*) malloc(sizeof(queue_node));

	if (node == NULL)
		return false;

	node->elem = elem;
	node->next = NULL;

	if (is_queue_empty(q)) {
		q->first = node;
		q->last = node;
	} else {
		q->last->next = node;
		q->last = node;
	}

	q->size++;
	return true;

}

void* queue_pop(queue* q) {

	if (is_queue_empty(q))
		return NULL;

	queue_node* node;

	node = q->first;
	void * elem = node->elem;
	free(q->first);
	q->first = node->next;
	q->size--;

	return elem;

}

void* queue_top(queue* q) {
	return q->first->elem;
}

void queue_delete(queue* q) {

	queue_node* node;
	unsigned int i = 0;

	for (;i < queue_size(q); i++) {

		node = q->first->next;
		free(q->first);
		q->first = node;
		q->size--;

	}

}
