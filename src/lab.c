#include <stdlib.h>
#include "lab.h"

list_t *list_init(void (*destroy_data)(void *), int (*compare_to)(const void *, const void *)) {
    // Allocate memory for the list 
    list_t *new_list = (list_t *)malloc(sizeof(list_t));
    if (new_list == NULL) {
        return NULL; 
    }
    // Allocate memory for the sentinel node
    new_list->head = (node_t *)malloc(sizeof(node_t));
    if (new_list->head == NULL) {
        free(new_list);
        return NULL;
    }
    
    // Sentinel node points to itself when the list is empty
    new_list->head->next = new_list->head;
    new_list->head->prev = new_list->head;

    new_list->destroy_data = destroy_data;
    new_list->compare_to = compare_to;
    new_list->size = 0;

    return new_list;
}

void list_destroy(list_t **list) {
    // Check if the list is empty
    if (list == NULL) {
        return;
    }

    node_t *current = (*list)->head->next;
    while (current != (*list)->head) {
        node_t *next = current->next;
        if ((*list)->destroy_data != NULL) {
            (*list)->destroy_data(current->data);
        }
        free(current);
        current = next;
    }

    free((*list)->head);
    free(*list);
    *list = NULL;
}

list_t *list_add(list_t *list, void *data) {
    // Check if the list is empty
    if (list == NULL) {
        return NULL;
    }

    node_t *new_node = (node_t *)malloc(sizeof(node_t));
    if (new_node == NULL) {
        return NULL;
    }

    new_node->data = data;
    new_node->next = list->head->next; // Points to the current first node
    new_node->prev = list->head; // Points to the sentinel node

    // Update surrounding nodes
    list->head->next->prev = new_node; // Previous first node now points to new node
    list->head->next = new_node; // Sentinel node points to new node as the first node

    list->size++; 
    return list;
}

void *list_remove_index(list_t *list, size_t index) {
    // Check if the list is empty and if the index is valid
    if (list == NULL || index >= list->size) {
        return NULL;
    }
    node_t *current = list->head->next;
    for (size_t i = 0; i < index; i++) {
        current = current->next;
    }

    // Unlink the node from the list
    current->prev->next = current->next;
    current->next->prev = current->prev;

    // Extract data and free the node
    void *data = current->data;
    free(current);

    list->size--; 
    return data;
}

int list_indexof(list_t *list, void *data) {
    // Check if the list is empty and if the data is NULL
    if (list == NULL || data == NULL) {
        return -1;
    }

    node_t *current = list->head->next;
    int index = 0;

    while (current != list->head) {
        if (list->compare_to(current->data, data) == 0) {
            return index;
        }
        current = current->next;
        index++;
    }

    return -1; 
}