#define _CRT_SECURE_NO_WARNINGS

// --- headers ---
#include "Packet.h"
#include "BST.h"

// --- functions ---

/*creates a node of a BST, allocate memory to it and store several values to it
rest is set to null*/
S_node* create_node(char da, char output_port) { // aiding function for section e - subsection 1
	S_node* new_node = NULL;
	assert(new_node = (S_node*)malloc(sizeof(S_node)));
	new_node->da = da;
	new_node->output_port = output_port;
	new_node->left = NULL;
	new_node->right = NULL;

	return new_node;

}

//----------------------------------------------------------
/*adds a leaf to a route*/
S_node* add_route(S_node* root, char da, char output_port) { // section e - subsection 1
	// if no root exists add the node as the head
	if (!root) {
		root = create_node(da, output_port);
		ADDED;
	}
	
	// if da bigger go right
	else if (UN da > UN root->da) {
		if (!root->right) {
			root->right = create_node(da, output_port);
			ADDED;
		}
		else { add_route(root->right, da, output_port); }
	}
	// if da smaller go left
	else if (UN da < UN root->da) {
		if (!root->left) {
			root->left = create_node(da, output_port);
			ADDED;
		}
		else { add_route(root->left, da, output_port); }
	}

	// in case of getting a double value
	else { printf("this da already exists\n"); }
	return root;
}

//----------------------------------------------------------
/*looks for the lowest valued leaf of a given branch*/
S_node* min_node(const S_node* root) { // aiding function for delete route
	S_node* curr = root;

	// search for the lowest value of given root
	while (curr && curr->left != NULL) {
		curr = curr->left;
	}

	return curr;
}

//----------------------------------------------------------
/*deletes a leaf/branch from the tree
if a branch needs to be deleted all of its' child branches and leaves will be balanced*/ 
S_node* delete_route(S_node* root, char da) { // section e subsection 2
	 // if root is empty
	if (root == NULL) return root;

	// da smaller -> go left
	if (UN da < UN root->da) { root->left = delete_route(root->left, da); }

	// da bigger -> go right
	else if (UN da > UN root->da) { root->right = delete_route(root->right, da); }

	// da that needs to be deleted was found
	else 
	{	
		// root has 1 child
		// no left
		if (root->left == NULL)
		{
			S_node* temp = root->right;
			free(root);
			root = NULL;
			return temp;
		}
		// no right
		else if (root->right == NULL)
		{
			S_node* temp = root->left;
			free(root);
			root = NULL;
			return temp;
		}

		// root has 2 children
		S_node* temp = min_node(root->right);
		root->da = temp->da;
		root->output_port = temp->output_port;
		root->right = delete_route(root->right, temp->da);
	}

	return root;
}

//----------------------------------------------------------
//searches for a node with a specific value
S_node* search_route(const S_node* root, char da) { //section e subsection 3
	if (!root || UN root->da == UN da) { return root; } // stop condition
	if (UN da > UN root->da) { return search_route(root->right, da); } // da bigger -> go right
	if (UN da < UN root->da) { return search_route(root->left, da); } // da smaller -> go left

}

// ----------------------------------------------------------
// prints the routing table in order
void print_routing_table(const S_node* root) { // section e subsection 4
	if (root) {
		// start recursing from the left
		print_routing_table(root->left);
		// print the route
		printf("da -> %d | output -> %d\n", UN root->da, UN root->output_port);
		// keep recursing from the right
		print_routing_table(root->right);
	}
}

// ----------------------------------------------------------
//create an entire BST using data from a given file
S_node* build_route_table(FILE* fp, S_node* root) { // section e subsection 5
	char command = 0;
	int da = 0, port = 0;
	//loop over the file
	while (!feof(fp)) {
		//scan the command
		fscanf(fp, "%c", &command);
		if (command == 'a') {
			//add the relevant da and port to the tree
			fscanf(fp, "%d %d\n", &da, &port);
			root = add_route(root, da, port);
		}
		else if (command == 'd') {
			//delete the relevant node 
			fscanf(fp, "%d\n", &da);
			root = delete_route(root, da);
		}
		// reset all variables for next iteration
		da = port = command = NULL;
	}
	printf("the routing table is as shown bellow:\n");
	//print the BST that was created
	print_routing_table(root);
	return root;
}

// ----------------------------------------------------------
//deletes an entire tree
void delete_tree(S_node* root) {
	if (root) {
		// start recursing from the left
		delete_tree(root->left);
		// keep recursing from the right
		delete_tree(root->right);
		//
		free(root);
	}
}
