/*
	This is a circular doubly linked list.
	The very first element in the list is the sentry 
	element and does not count as a element of the list.
 	New: the first element also contains the length of the list
	Made by Anders Busch (2015)
*/
#include <stdlib.h>
#include "dlinkedlist.h"

/*
 * Allocates memory to the list and setups the sentry element
 * returns NULL if unsuccessful
 */
linked_list *initialize_list() {
	linked_list *new_list;
	int *initial_length;
	new_list = malloc( sizeof(linked_list) );

	if (new_list == NULL) {
		DL_OUT_OF_MEM_MSG;
		return NULL;	
	}
	initial_length = calloc(1, sizeof(int));

	new_list->next = new_list;
	new_list->previous = new_list;
	new_list->data = (void *) initial_length;

	return new_list;
}

/* 
 * Appending a new element to the list.
 * Returns DL_SUCCESS_SIGNAL on successful appending,
 * DL_ERROR_SIGNAL otherwise.
 */
int append_element(linked_list *list, void *element) {
	if (list == NULL || element == NULL) {
		DL_INVALID_ARGS_MSG;
		return DL_ERROR_SIGNAL;
	}

	linked_list *new_list_element;

	new_list_element = malloc( sizeof(linked_list) );
	
	if (new_list_element == NULL) {
		DL_OUT_OF_MEM_MSG;
		return DL_ERROR_SIGNAL;
	}

	new_list_element->data = element;

	*((int*) list->data) += 1;

	// list->previous is the (old) last element in the list

	new_list_element->previous = list->previous;
	new_list_element->next = list;

	list->previous->next = new_list_element;
	list->previous = new_list_element;

	return DL_SUCCESS_SIGNAL;
}

/*
 * Prepends a new element to the list. 
 * Returns DL_SUCCESS_SIGNAL on successful pre-pending,
 * DL_ERROR_SIGNAL otherwise.
 */
int prepend_element(linked_list *list, void *element) {
	if(list == NULL || element == NULL){
		DL_INVALID_ARGS_MSG;
		return DL_ERROR_SIGNAL;
	}

	linked_list *new_list_element;

	new_list_element = malloc(sizeof(linked_list));
	
	if(new_list_element == NULL){
		DL_OUT_OF_MEM_MSG;
		return DL_ERROR_SIGNAL;
	}

	new_list_element->data = element;

	*((int*) list->data) += 1;

	// list->next is the old first element in the list

	new_list_element->next = list->next;
	new_list_element->previous = list;

	list->next->previous = new_list_element;
	list->next = new_list_element;

	return DL_SUCCESS_SIGNAL;
}

/*
 * Returns DL_SUCCESS_SIGNAL if element was removed, returns
 * DL_ERROR_SIGNAL otherwise
 */
int delete_element(linked_list *list, void *element) {
	if (list == NULL || element == NULL) {
		DL_INVALID_ARGS_MSG;
		return DL_ERROR_SIGNAL;
	}

	linked_list *iterator;

	iterator = list->next;

	while (iterator != list) {
		if (iterator->data == element) {
			iterator->previous->next = iterator->next;
			iterator->next->previous = iterator->previous;
			free(iterator);
			*((int*) list->data) -= 1;
			return DL_SUCCESS_SIGNAL;
		}
		iterator = iterator->next;
	}
	return DL_ERROR_SIGNAL;
}

/* 
 * Retrieves the first element in the list, or NULL if no first element is
 * found
 */
void *get_first_element(linked_list *list) {
	if (list == NULL) {
		DL_INVALID_ARGS_MSG;
		return NULL;	
	} else if (list->next == list) {
		DL_OUT_OF_BOUND_MSG;
		return NULL;
	}
	return list->next->data;
}


/*
 * Retrieves the last element in the list, or NULL if no last element is found
 */
void *get_last_element(linked_list *list) {
	if (list == NULL) {
		DL_INVALID_ARGS_MSG;
		return NULL;	
	} else if (list->previous == list) {
		DL_OUT_OF_BOUND_MSG;
		return NULL;
	}
	return list->previous->data;
}

/*
 * Retrieves the k'th element in the list, or NULL and an error message if
 * index is out of bounce 
 */
void *get_element(linked_list *list, int k) {
	if (list == NULL) {
		DL_INVALID_ARGS_MSG;
		return NULL;	
	}

	linked_list *iterator;
	int i;

	if ( k >= 0 && k < get_length(list) ) {
		iterator = list->next;

		for (i = 0; i < k; ++i) {
			iterator = iterator->next;
		}

		return iterator->data;
	}

	DL_OUT_OF_BOUND_MSG;
	return NULL;
}

/*
 * Gets the index of the element,
 * returns -1 if not found
 */
int get_index_of_element(linked_list *list, void *data) {
	if (list == NULL || data == NULL) {
		DL_INVALID_ARGS_MSG;
		return DL_ERROR_SIGNAL;
	}
	int i;
	linked_list *iterator;
	iterator = list->next;

	for (i = 0; list != iterator; i++) {
		if (data == iterator->data) {
			return i;
		}
		iterator = iterator->next;
	}

	return DL_ERROR_SIGNAL;
}

/*
 * Gets a count of the elements in the linked list or DL_ERROR_SIGNAL if error
 */
int get_length(linked_list *list) {
	if (list == NULL) {
		DL_INVALID_ARGS_MSG;
		return DL_ERROR_SIGNAL;
	}

/*	linked_list *iterator;
	int counter;

	counter = 0;
	iterator = list->next;

	while (iterator != list) {
		counter++;
		iterator = iterator->next;
	}

	return counter; */

	return *((int*) list->data);
}

/* 
 * Makes a string containing all pointers to the element data 
 * in the linked list. This hopefully gives some insight into 
 * the contents of the linked list. 
 */
void list_dump(const linked_list *list) {
	if (list == NULL) {
		DL_INVALID_ARGS_MSG;
		return;	
	}

	linked_list *iterator;
	
	iterator = list->next;
	
	printf("[");
	while(list != iterator) {
		if (iterator->next == list) {
			printf("%p",iterator->data);
		} else {
			printf("%p,",iterator->data);
		}
		iterator = iterator->next;
	}
	printf("]\n");
}

/*
 * Deallocates all the nodes in the whole list
 */
void terminate_list(linked_list **list) {
	if (list == NULL || *list == NULL) {
		DL_INVALID_ARGS_MSG;
		return;	
	}

	linked_list *iterator;
	linked_list *placeholder;

	iterator = (*list)->next;
	while (iterator != (*list)) {
		placeholder = iterator->next;
		free(iterator);
		iterator = placeholder;
	}
	free(*list);
	*list = NULL;
}
