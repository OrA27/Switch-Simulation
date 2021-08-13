#define _CRT_SECURE_NO_WARNINGS

// -- headers ---
#include <string.h>
#include "BST.h"
#include "Queue.h"



void main(int argc, char* argv[]) {
	int i =0;
	// placeholder for output-ports names
	char port_name[10] =  "port0.out";
	char port_index = NULL;
	// array of pointers to the 4 output-ports managers
	S_Out_Qs_mgr** out_mgr = NULL;
	// array for all the PORTx.in files and route file
	FILE** in_files = NULL; 
	// array of the output-port files
	FILE** out_ports = NULL;

	// memory allocation for the port managers and the in and out ports files
	assert(out_mgr = (S_Out_Qs_mgr**)malloc(4 * sizeof(S_Out_Qs_mgr*)));
	assert(in_files = (FILE**)malloc(argc * sizeof(FILE*)));
	assert(out_ports = (FILE**)malloc(4 * sizeof(FILE*)));
	for (i = 0; i < 4; i++){
		assert(out_mgr[i] = (S_Out_Qs_mgr*)malloc(sizeof(S_Out_Qs_mgr)));
		out_mgr[i]->head_p0 = NULL;
		out_mgr[i]->head_p1 = NULL;
		out_mgr[i]->tail_p0 = NULL;
		out_mgr[i]->tail_p1 = NULL;
		printf("port %d manager is up and initialized\n\n", i+1);
	}
	i = 0; // reset i
	// open the files that were given in the command line
	while (i < argc - 1) {
		in_files[i] = fopen(argv[i + 1], "r");
		i++;
	}
	printf("command line files are open\n\n");
	// create the BST that contain the routing table
	S_node* tree = NULL;
	// build the routing table
	tree = build_route_table(in_files[ROUTE], tree);
	//close the route file
	fclose(in_files[ROUTE]);
	
	/*read all the packets from portx.in
	sort them to their respective output-port manager
	close the respective PORTx.in file*/
	for (i = 1; i <= 4; i++) {
		port_to_queue(in_files[i] ,tree ,out_mgr);
		printf("data from input-port %d has been received\n\n", i);
		fclose(in_files[i]);
	}
	// sort all output-ports by time 
	for (i = 0; i < 4; i++) {
		printf("now sorting output port %d\n", i + 1);
		sort_queue(out_mgr[i]);
		printf("output-port %d is now sorted\n\n", i+1);
	}

	/*open the PORTx.out files
	write in them the packets of their respective output manager
	close the PORTx.out files
	free the respective out_mgr and set it to NULL*/
	for (i = 0; i < 4; i++) {
		port_index = '0' + i + 1; // "stringify and int into a char"
		memset(port_name + 4, port_index, 1); // set the memory byte of the number to the port index
		out_ports[i] = fopen(port_name, "w"); // create a file with the name
		queue_to_port(out_ports[i], out_mgr[i]); // write packets from queue to respective port
		fclose(out_ports[i]); //close the newly created file
		printf("output-port %d has sent all of its packets\n\n", i + 1);

		free(out_mgr[i]); // free the manager of said port
		out_mgr[i] = NULL; // set it to null
	}
	// free the all the dynamic memory-allocated variables
	delete_tree(tree);
	free(out_ports);
	free(in_files);
	free(out_mgr);
	out_ports = in_files = out_mgr = NULL;
}