#ifndef PROJECT_NODE_ARRAY_LIST_H
#define PROJECT_NODE_ARRAY_LIST_H

#include "tree.h"

/*
 * A dynamic array (ArrayList) that keeps the nodes in the tree stored in sorted order
 * from largest to smallest id. This allows for a fast find_leader operation.
 */
typedef struct NodeArrayList NodeArrayList;

NodeArrayList* create_node_array_list();

void free_node_array_list(NodeArrayList* list);

size_t get_node_array_list_allocated_bytes(NodeArrayList* list);

/*
 * Node must have weight 1 and have the lowest id of all nodes currently in the array list
 * (nng should thus never be added to this list).
 *
 * Inserts the node in the first free position at the front of the list,
 * this way the list stays sorted from largest to smallest id.
 */
void insert_node(NodeArrayList* list, Node* node);

/*
 * Notify the list that nodes n and m were swapped.
 * To keep the list sorted we have to swap them in the list as well.
 */
void notify_swap(NodeArrayList* list, Node* n, Node* m, int id_plus);

/*
 * Find the node with the highest id that has the same weight as the given node.
 * Returns the given node if it is the leader.
 */
Node* find_leader(NodeArrayList* list, Node* node, int id_plus);

#endif //PROJECT_NODE_ARRAY_LIST_H
