#define _CRT_SECURE_NO_WARNINGS

// --- headers ---
#include "Packet.h"

// --- functions ---
	/*reads packet information from a file and stores it in a packet structure*/
void packet_read(FILE* fp, packet* pkt) { // section b
	unsigned char* data_arr = NULL;
	int i = 0;
	// scan information of a line up to data length
	fscanf(fp, "%u %hhu %hhu %hhu %hhu", &pkt->Time, &pkt->Da, &pkt->Sa, &pkt->Prio, &pkt->Data_Length);

	// create array with size of length
	assert(data_arr = (unsigned char*)malloc(sizeof(unsigned char) * pkt->Data_Length)); 
	// scan data information into the array
	for (; i < pkt->Data_Length; i++) { 
		fscanf(fp, "%hhu", &data_arr[i]);
	}

	// add the array into the packet
	pkt->Data = data_arr;
	
	// scan checksum as the last information,
	// space in the end is meant to "eat up" the remaining '\n'
	fscanf(fp, "%hhu ", &pkt->Checksum);
}

//----------------------------------------------------------

	/*writes information stored in a packet onto a given file*/
void packet_write(FILE* fp, const packet* pkt) { // section c
	int i = 0;
	// write up to data length
	fprintf(fp, "%d %d %d %d %d ", pkt->Time, UN pkt->Da, UN pkt->Sa, UN pkt->Prio, UN pkt->Data_Length);

	// write data to file
	for (; i < pkt->Data_Length; i++) {
		fprintf(fp, "%d ", pkt->Data[i]);
	}

	// write the remaining checksum and the '\n' 
	fprintf(fp, "%d\n", UN pkt->Checksum); // write checksum
}

//----------------------------------------------------------

	/*creates a packet with allocated memory*/
packet* create_packet(FILE* fp) {
	packet* new_packet = NULL;
	assert(new_packet = (packet*)malloc(sizeof(packet)));
	packet_read(fp, new_packet);

	return new_packet;
}

//----------------------------------------------------------

/*checks if a packet is valid by 'XOR-ing' bits of selected values
and compares it to its checksum value*/
Bool checksum_check(const packet* pkt) { // section d
	// preform bitwise XOR over the packet's information
	// return the result of comparison between checker and checksum in the form of Bool 
	int i = 0;
	char checker = pkt->Da ^ pkt->Sa;
	checker ^= pkt->Prio;
	checker ^= pkt->Data_Length;
	for (; i < pkt->Data_Length; i++) {
		checker ^= pkt->Data[i];
	}
	return checker == pkt->Checksum;
}
