#include <math.h>
#include <stdlib.h>
#include "node_array_list.h"
#include "../misc/util.h"

#define INITIAL_ARRAY_SIZE 256
#define GROWTH_RATE 1.5


typedef struct NodeArrayList {
    Node** nodes;
    int current_size;
    int current_used;
} NodeArrayList;


NodeArrayList* create_node_array_list() {
    NodeArrayList* result = safe_malloc(sizeof(NodeArrayList));
    result->nodes = safe_malloc(INITIAL_ARRAY_SIZE * sizeof(Node*));
    result->current_size = INITIAL_ARRAY_SIZE;
    result->current_used = 0;
    return result;
}


void free_node_array_list(NodeArrayList* list) {
    free(list->nodes);
    free(list);
}


size_t get_node_array_list_allocated_bytes(NodeArrayList* list) {
    return (list->current_size * sizeof(Node*)) + sizeof(NodeArrayList);
}


void insert_node(NodeArrayList* list, Node* node) {
    if (list->current_used == list->current_size) {
        list->current_size = floor(list->current_size * GROWTH_RATE);
        list->nodes = safe_realloc(list->nodes, list->current_size * sizeof(Node*));
    }
    list->nodes[list->current_used++] = node;
}


/*
 * Since the list is sorted by id and all id's are associated with a node (there are no 'holes'),
 * we can calculate the index of a node in the array list by it's id.
 */
int get_node_index(NodeArrayList* list, Node* node, int id_plus) {
    return list->current_used - get_node_id(node, id_plus);
}


void notify_swap(NodeArrayList* list, Node* n, Node* m, int id_plus) {
    int n_index = get_node_index(list, n, id_plus);
    int m_index = get_node_index(list, m, id_plus);
    list->nodes[n_index] = m;
    list->nodes[m_index] = n;
}


/*
 * Starting from the index of the node in the list, traverse the list backwards
 * until we find the last node that has the same weight as the given node.
 * As the list is sorted by id, this node is the leader. In theory
 * this approach has a worst case time complexity of O(n).
 * e.g. if all leaf nodes have a weight of 1, and we wish to find the node with the highest id
 * of weight 1 and we are unlucky enough to have to start from the lowest id node with weight 1,
 * we'd have to check all leaf nodes or around n/2 nodes. In practice however, this rarely happens.
 */
Node* find_leader(NodeArrayList* list, Node* node, int id_plus) {
    int node_index = get_node_index(list, node, id_plus);
    int i = node_index - 1;
    while (i >= 0 && has_same_weight(list->nodes[i], node)) {
        i--;
    }
    return list->nodes[i+1];
}
