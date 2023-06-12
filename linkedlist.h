//
// Created by Valentin Härter on 12.06.23.
//

#ifndef PRODUCERCONSUMER_LINKEDLIST_H
#define PRODUCERCONSUMER_LINKEDLIST_H

typedef struct node {
    int data;
    struct node *next;
} node_t;

node_t *create_node(int data, node_t *next);
node_t *insert_front(node_t *anchor, int data);
int pop_back(node_t *node);
void delete_list(node_t *anchor);

#endif //PRODUCERCONSUMER_LINKEDLIST_H
