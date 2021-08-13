#pragma once

// --- headers ---
#include <stdio.h>

// --- macro ---
#define ADDED printf("root was added\n")

// --- structures ---
	/*a binary search tree that acts as router for packets
	the tree states which da goes to which output port*/
typedef struct route_node {
	char da;
	char output_port;
	struct route_node* left;
	struct route_node* right;
} S_node;

// --- functions ---
S_node* create_node(char da, char output_port);
S_node* add_route(S_node* root, char da, char output_port);
S_node* search_route(const S_node* root, char da);
S_node* min_node(const S_node* root);
S_node* delete_route(S_node* root, char da);
void print_routing_table(const S_node* root);
S_node* build_route_table(FILE* fp, S_node* root);
void delete_tree(S_node* root);
