#pragma once
// --- macros ---
#define UN (unsigned char)

// --- headers ---
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


// --- structures ---
//section a - the data structure of a packet
	 /*a data packet storing some information.
	 all elements are considered as chars - one byte size elements
	 Data is a pointer to an array since its size is not fixed*/
typedef struct Packet { 
	unsigned int Time;
	char Da, Sa, Prio, Data_Length, Checksum;
	unsigned char* Data;

} packet;

// --- variables ---
typedef enum Boolean { False, True } Bool;

// --- functions ---
packet* create_packet(FILE* fp);
void packet_read(FILE* fp, packet* pkt);
void packet_write(FILE* fp, const packet* pkt);
Bool checksum_check(const packet* pkt);