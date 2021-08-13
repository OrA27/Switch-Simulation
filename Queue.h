#pragma once
// --- macros ---
#define ROUTE 0

// --- headers ---
#include "Packet.h"
#include "BST.h"

// --- structures ---
	/*a node int the packet queue
	contains a packet and a pointer to the next node*/
typedef struct pkt_node {
	packet* pkt;
	struct pkt_node* next;
} S_pkt;

//----------------------------------------------------------
/*the queue manager
contains the heads and tails of two queues
the queues are distinguished by their priority
priority 0 is greater than priority 1*/
typedef struct Out_Qs_mgr {
	struct pkt_node* head_p1;
	struct pkt_node* tail_p1;
	struct pkt_node* head_p0;
	struct pkt_node* tail_p0;
} S_Out_Qs_mgr;


// --- functions ---

S_pkt* create_pkt_node(packet* pkt);
void enqueue_pkt(S_Out_Qs_mgr* QM_ptr, packet* pkt);
packet* dequeue_pkt(S_Out_Qs_mgr* QM_ptr, char priority);
void sort_queue(S_Out_Qs_mgr* QM_ptr);
void port_to_queue(FILE* fp, S_node* tree, S_Out_Qs_mgr** out);
void queue_to_port(FILE* fp, S_Out_Qs_mgr* out_i);