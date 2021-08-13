#define _CRT_SECURE_NO_WARNINGS

// --- headers ---
#include "Queue.h"

// --- functions ---
/*create a node of a packet queue, allocates memory to it,
initialize its packet information and set its next value to NULL */
S_pkt* create_pkt_node(packet* pkt) {
	S_pkt* new_node = NULL;
	assert(new_node = (S_pkt*)malloc(sizeof(S_pkt)));
	new_node->pkt = pkt;
	new_node->next = NULL;
	return new_node;

}

//----------------------------------------------------------
/*enqueues packets to a manager queue.
check if packet is legitimate using checksum_check
if packet isn't legitimate free the packet->end
if it is legitimate create a node and check the priority of the packet
enqueue the node to the end of the queue->end*/
void enqueue_pkt(S_Out_Qs_mgr* QM_ptr, packet* pkt) { // section f subsection 1
	Bool res = checksum_check(pkt);
	S_pkt* new_node = NULL, * temp = NULL;

	switch (res)
	{
	// packet is valid
	case True:
		printf("packet is valid inserting to queue\n");
		// create a node
		new_node = create_pkt_node(pkt);
		// check the priority of the packet
		switch (pkt->Prio)
		{
		// priority is 0
		case 0:
			// if queue is empty
			if (!QM_ptr->tail_p0) {
				// only one packet in queue so it's both head and tail
				QM_ptr->head_p0 = QM_ptr->tail_p0 = new_node;
			}
			// queue is not empty
			else {
				// new node becomes the next of tail
				QM_ptr->tail_p0->next = new_node;
				// new node is set as tail
				QM_ptr->tail_p0 = new_node;
			}
			break;
		// priority is 1 - process is same as case 0
		case 1:
			if (!QM_ptr->tail_p1) {
				QM_ptr->head_p1 = QM_ptr->tail_p1 = new_node;
			}
			else {
				QM_ptr->tail_p1->next = new_node;
				QM_ptr->tail_p1 = new_node;
			}
			break;
		}
		printf("packet enqueued successfully\n\n");
		break;

	//checksum failed
	case False:
		printf("packet invalid - discarding packet\n");
		// free packet
		free(pkt->Data);
		free(pkt);
		pkt = NULL;
		printf("packet discarded\n\n");
	}
}

//----------------------------------------------------------
/*dequeues packets from a given manager queue.
check priority of wanted queue
if queue is empty->end
otherwise store address of QM_ptr head
if queue has one packet->set the tail to null
next of head becomes the head
return the packet of wanted queue*/
packet* dequeue_pkt(S_Out_Qs_mgr* QM_ptr, char priority) { // section f subsection 2
	S_pkt* S_temp = NULL;
	packet* removed = NULL;
	// check priority
	switch (priority)
	{
	// priority is 0
	case 0: 
		// queue is empty - return null
		if (!QM_ptr->head_p0) {
			printf("queue p0 is empty\n");
			return NULL;
		}
		// create temp node
		S_temp = QM_ptr->head_p0;
		// case of only one packet in queue
		if (S_temp == QM_ptr->tail_p0) { QM_ptr->tail_p0 = NULL; } 
		// next in queue becomes the head
		QM_ptr->head_p0 = QM_ptr->head_p0->next;
		// get the packet of the original head node
		removed = S_temp->pkt;
		// free the original head node
		free(S_temp);
		// return the dequeued packet
		return removed;

	// priority is 1 - same process as case 0
	case 1:
		if (!QM_ptr->head_p1) { 
			printf("queue p1 is empty\n");
			return NULL;
		}
		S_temp = QM_ptr->head_p1;
		if (S_temp == QM_ptr->tail_p1) { QM_ptr->tail_p1 = NULL; }
		QM_ptr->head_p1 = QM_ptr->head_p1->next;
		removed = S_temp->pkt;
		free(S_temp);
		return removed;
	}
}

//----------------------------------------------------------
/*sort a manager queue using a packet time as the key
creates a new queue, initialize its components to null
moves over nodes from original queue to new queue
dequeues a node from original queue
compares its time with the times of head and tail of new queue
new time < head time -> node becomes new head
new time > tail time -> node become new tail
otherwise loop over new queue and find it's correct place in the queue
copy new queue members to original queue and free the new queue*/
void sort_queue(S_Out_Qs_mgr* QM_ptr) {
	S_Out_Qs_mgr* sorted_mgr = NULL;
	S_pkt* node = NULL, * temp = NULL;
	assert(sorted_mgr = (S_Out_Qs_mgr*)malloc(sizeof(S_Out_Qs_mgr)));
	// initialize new queue's members as null
	sorted_mgr->head_p0 = NULL;
	sorted_mgr->head_p1 = NULL;
	sorted_mgr->tail_p0 = NULL;
	sorted_mgr->tail_p1 = NULL;
	// start with priority 0
	while (QM_ptr->head_p0) {
		// dequeue a node from original queue
		node = create_pkt_node(dequeue_pkt(QM_ptr, 0));
		// if queue is empty
		if (!sorted_mgr->tail_p0) {
			// only one packet in queue so it's both head and tail
			sorted_mgr->head_p0 = sorted_mgr->tail_p0 = node;
		}
		// queue is not empty
		else {
			// new packet time is earlier than head packet time
			if (sorted_mgr->head_p0->pkt->Time > node->pkt->Time) {
				node->next = sorted_mgr->head_p0;
				sorted_mgr->head_p0 = node;
			}
			// new packet time is later than tail packet time
			else if (sorted_mgr->tail_p0->pkt->Time < node->pkt->Time) {
				sorted_mgr->tail_p0->next = node;
				sorted_mgr->tail_p0 = node;
			}
			// new packet time is later than head's but earlier than tail's packet times
			else {
				// set a temp pointer for head
				temp = sorted_mgr->head_p0;
				// go over queue until desired place found
				while (temp->next != NULL && temp->next->pkt->Time < node->pkt->Time) { 
					temp = temp->next;
				}
				//place new node in desired place
				node->next = temp->next;
				temp->next = node;
			}
		}
		// reset variables to prevent connection between the 2 queues
		node = NULL;
		temp = NULL;
	}
	printf("p0 queue is now sorted by time\n");
	//repeat same process for p1 queue
	while (QM_ptr->head_p1) {
		node = create_pkt_node(dequeue_pkt(QM_ptr, 1));
		if (!sorted_mgr->tail_p1) { 
			sorted_mgr->head_p1 = sorted_mgr->tail_p1 = node; 
		}
		else { 
			if (sorted_mgr->head_p1->pkt->Time > node->pkt->Time) {
				node->next = sorted_mgr->head_p1;
				sorted_mgr->head_p1 = node;
			}
			else if (sorted_mgr->tail_p1->pkt->Time < node->pkt->Time) {
				sorted_mgr->tail_p1->next = node;
				sorted_mgr->tail_p1 = node;
			}
			else {
				temp = sorted_mgr->head_p1;
				while (temp->next != NULL && temp->next->pkt->Time < node->pkt->Time) {
					temp = temp->next;
				}
				node->next = temp->next;
				temp->next = node;
			}
		}
		node = NULL;
		temp = NULL;
	}
	printf("p1 queue is now sorted by time\n");
	// move over members of sorted mgr to original mgr
	QM_ptr->head_p0 = sorted_mgr->head_p0;
	QM_ptr->head_p1 = sorted_mgr->head_p1;
	QM_ptr->tail_p0 = sorted_mgr->tail_p0;
	QM_ptr->tail_p1 = sorted_mgr->tail_p1;
	// free the sorted mgr
	free(sorted_mgr);
	sorted_mgr = NULL;
}

//----------------------------------------------------------
/*moves over packets from PORTx.in to the exit ports queue managers
goes over PORTx.in file until it ends
creates a packet and searches it's correct route
if a route was found enqueue the packet to the correct manager
otherwise free the packet*/
void port_to_queue(FILE* fp, S_node* tree, S_Out_Qs_mgr** out) {
	packet* pkt = NULL;
	S_node* dir_route = NULL;
	// go over the file
	while (!feof(fp)) {
		// create a packet
		pkt = create_packet(fp);
		// search for the desired route of the packet's da
		dir_route = search_route(tree, pkt->Da);
		// route was found
		if (dir_route) {
			//enqueue packet to the correct output-port's manager
			printf("route found - attempting to enqueue to port %d\n", (dir_route->output_port));
			enqueue_pkt(out[(dir_route->output_port) - 1], pkt);
		}
		//route was not found
		else {
			printf("route was not found - packet discarded\n\n");
			// free packet and set it to NULL
			free(pkt);
			pkt = NULL;
		}
	}
}

//----------------------------------------------------------
/*moves over packts from a single port mgr to its correct PORTx.out file
goes over the p0 queue until it's empty
dequeue a packet and write it to the PORTx.out file
frees the data array in the packet and set it to null
free the packet itself set it to null
repeat the process for the p1 queue*/
void queue_to_port(FILE* fp, S_Out_Qs_mgr* out_i) {
	packet* pkt;
	// loop over p0 queue
	while (out_i->head_p0) {
		// dequeue a packet
		pkt = dequeue_pkt(out_i, 0);
		//write it to the file
		packet_write(fp, pkt);
		//free packet and set it to null
		free(pkt->Data);
		pkt->Data = NULL;
		free(pkt);
		pkt = NULL;
	}
	// repeat the same process for p1 queue
	while (out_i->head_p1) {
		pkt = dequeue_pkt(out_i, 1);
		packet_write(fp, pkt);
		free(pkt->Data);
		free(pkt);
		pkt = NULL;
	}
}
