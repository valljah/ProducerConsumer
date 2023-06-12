//
// Created by Valentin Härter on 12.06.23.
//

#include <stdlib.h>
#include "linkedlist.h"

node_t *create_node(int data, node_t *next) {
    node_t *new_node = malloc(sizeof(int));
    new_node->data = data;
    new_node->next = next;
    return new_node;
}

node_t *insert_front(node_t *anchor, int data) {
    if (anchor == NULL) {
        return create_node(data, NULL);
    }
    return create_node(data, anchor);
}

int pop_back(node_t *node) {
    if (node == NULL) return -1;
    int node_value;

    if (node->next == NULL) {
        node_value = node->data;
        free(node);
        return node_value;
    }
    node_t *current_node = node;
    while (current_node->next->next != NULL)
        current_node = current_node->next;

    node_value = current_node->next->data;
    free(current_node->next);
    current_node->next = NULL;
    return node_value;
}

void delete_list(node_t *anchor) {
    if (anchor == NULL) return;
    node_t *current_node, *temp;
    current_node = anchor;
    while (current_node->next != NULL) {
        temp = current_node;
        current_node = current_node->next;
        free(temp);
    }
    free(current_node);
}
