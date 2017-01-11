#include "adaptive.h"

void newNode(node **leaf, unsigned char symbol, bool is_nyt) {
		node *temp =  (node*) malloc(sizeof(node));
		if (is_nyt) {
			temp->symbol = 0x00;
			temp->weight = 0;
			
		} else {
			temp->symbol = symbol;
			temp->weight = 1;
			
		}
		temp->parent = NULL;
		temp->left = NULL;
		temp->right = NULL;
			
		*leaf = temp;
		
		return;
	}
	
	void joinNodes(node **tree, node *left, node *right) {
		node *temp = (node*) malloc(sizeof(node));
		temp->weight = left->weight + right->weight;
		temp->left = left;
		temp->right = right;
		temp->left->parent = temp;
		temp->right->parent = temp;
		temp->symbol = 0x00;
		temp->parent = NULL;
		*tree = temp;
		
		return;
	}
	
	void deleteTree(node **tree) {
		if ((*tree)->left != NULL) {
			deleteTree(&(*tree)->left);
		}
		
		if ((*tree)->right != NULL) {
			deleteTree(&(*tree)->right);
		}
		
		if ((*tree) != NULL) {
			if ((*tree)->parent != NULL && (*tree)->left == NULL) {
				(*tree)->parent->left = NULL;
				free(*tree);
				
			} else if ((*tree)->parent != NULL && (*tree)->right == NULL) {
				(*tree)->parent->right = NULL;
				free(*tree);
				
			} else if ((*tree)->right == NULL && (*tree)->left == NULL){
				free(*tree);
				(*tree) = NULL;
				
			}
			
		}
		
		return;
	}
	
	void findNextBlock(node **tree, int weight, int *number, int parent_number, node **position, char *l_r) {
		if ((*tree)->weight == weight && (*tree)->order > *number && (*tree)->order != parent_number) {
			*position = (*tree)->parent;
			*number = (*tree)->order;
			if ((*tree)->parent->left->order == (*tree)->order) {
				strcpy(l_r, "l");
			} else {
				strcpy(l_r, "r");
			}
		}
		
		if ((*tree)->left != NULL) {
			findNextBlock(&(*tree)->left, weight, &*number, parent_number, &*position, l_r);
		}
		
		if ((*tree)->right != NULL) {
			findNextBlock(&(*tree)->right, weight, &*number, parent_number, &*position, l_r);
		}
			
		return;
	}
	
	void swapNodes(node *tree, char *l_r, node *sibling, char *l_r_sibling) {	
		if (strcmp(l_r, "l") == 0 && strcmp(l_r_sibling, "l") == 0) {
			node *temp = tree->left;
			tree->left = sibling->left;
			sibling->left = temp;
			
			tree->left->parent = tree;
			sibling->left->parent = sibling;
			
		} else if (strcmp(l_r, "l") == 0) {
			node *temp = tree->left;
			tree->left = sibling->right;
			sibling->right = temp;
			
			tree->left->parent = tree;
			sibling->right->parent = sibling;
			
		} else if (strcmp(l_r_sibling, "l") == 0) {
			node *temp = tree->right;
			tree->right = sibling->left;
			sibling->left = temp;
			
			tree->right->parent = tree;
			sibling->left->parent = sibling;
			
		} else {
			node *temp = tree->right;
			tree->right = sibling->right;
			sibling->right = temp;
			
			tree->right->parent = tree;
			sibling->right->parent = sibling;
			
		}
	
		return;
	}
	
	void treeTraversal(node **tree, vector<pair<int, node*> > *queue, int deep) {
		(*queue).push_back(make_pair(deep, *tree));
		
		if ((*tree)->right != NULL) {
			treeTraversal(&(*tree)->right, &*queue, deep+1);
		}
		
		if ((*tree)->left != NULL) {
			treeTraversal(&(*tree)->left, &*queue, deep+1);
		}
		
		return;
	}
	
	void updateWeight(node **tree) {
		if ((*tree)->left != NULL && (*tree)->right != NULL) {
			(*tree)->weight = (*tree)->left->weight + (*tree)->right->weight;
			
		} else {
			(*tree)->weight++;
			
		}
		return;
	}
	
	void findSymbolLeaf(node **tree, unsigned char symbol, node **position) {
		if ((*tree)->left != NULL) {
			findSymbolLeaf(&(*tree)->left, symbol, &*position);
		}
		
		if ((*tree)->symbol == symbol && (*tree)->left == NULL && (*tree)->right == NULL && (*tree)->weight != 0) {
			*position = *tree;
		}
		
		if ((*tree)->right != NULL) {
			findSymbolLeaf(&(*tree)->right, symbol, &*position);
		}
		
		return;
	}

	bool isSort(pair<int, node*> p, pair<int, node*> q) {
		return p.first < q.first;
	}
	
	void updateTree(node **tree, unsigned char symbol, unsigned char *dictionary, node **nyt) {
		node *temp;
		
		vector<pair<int, node*> > queue;
		
		if (dictionary[(int) symbol] == 0x01) {
			findSymbolLeaf(&*tree, symbol, &temp);
			
			node *inner_temp = NULL;
			char l_r[2];
			if (temp->parent->left->order == temp->order) {
				strcpy(l_r, "l");
				
			} else {
				strcpy(l_r, "r");
				
			}
			
			char l_r_sibling[2];
			int number = temp->order;
			findNextBlock(&*tree, temp->weight, &number, temp->parent->order, &inner_temp, l_r_sibling);
			if (inner_temp != NULL) {
				swapNodes(temp->parent, l_r, inner_temp, l_r_sibling);
			}
				
		// NYT
		} else {
			node *new_nyt;
			newNode(&new_nyt, 0x00, true);
			node *new_node;
			newNode(&new_node, symbol, false);
	
			node *old_nyt = NULL;
			joinNodes(&old_nyt, new_nyt, new_node);
			
			if (*tree == NULL) {
				*tree = old_nyt;
				*nyt = (*tree)->left;
				
			} else {
				old_nyt->parent = (*nyt)->parent;
				(*nyt)->parent->left = old_nyt;
				*nyt = old_nyt->left;
				
			}
			
			// goto old nyt
			temp = old_nyt;
			
			// give number
			treeTraversal(&*tree, &queue, 0);
			sort(queue.begin(), queue.end(), isSort);
			
			int num = NUMBER;
			for (int i=0; i<queue.size(); i++) {
				queue.at(i).second->order = --num;
			}
			
			queue.clear();
			
			dictionary[(int) symbol] = 0x01;
	
		}
		
		// increment weight
		updateWeight(&temp);
		
		while(temp->parent != NULL) {
			// go to parent node
			temp = temp->parent;
			
			// if not root
			if (temp->parent != NULL)
			{
				node *inner_temp = NULL;
				
				char l_r[2];
				if (temp->parent->left->order == temp->order) {
					strcpy(l_r, "l");
					
				} else {
					strcpy(l_r, "r");
					
				}
				
				char l_r_sibling[2];
				int number = temp->order;
				findNextBlock(&*tree, temp->weight, &number, temp->parent->order, &inner_temp, l_r_sibling);
				if (inner_temp != NULL) {
					swapNodes(temp->parent, l_r, inner_temp, l_r_sibling);
				}
				
			}
			updateWeight(&temp);
			
			treeTraversal(&*tree, &queue, 0);
			sort(queue.begin(), queue.end(), isSort);
			
			int num = NUMBER;
			for (int i=0; i<queue.size(); i++) {
				queue.at(i).second->order = --num;
			}
			
			queue.clear();
		}
		
		*tree = temp;
		
		return;
	}
	
	/**
	 * ENCODE
	 */
	
	void fetchSymbolCode(node **tree, unsigned char symbol, char *do_code, queue<char> *code) {
		char temp[strlen(do_code)+1];
		if ((*tree)->symbol == symbol && (*tree)->left == NULL && (*tree)->right == NULL && (*tree)->weight != 0) {
			for (int i=0; i<strlen(do_code); i++) {
				(*code).push(do_code[i]);
			}
			
			return;
		}
		
		strcpy(temp, do_code);
		if ((*tree)->left != NULL) {
			fetchSymbolCode(&(*tree)->left, symbol, strcat(temp, "0"), &*code);
		}
		
		strcpy(temp, do_code);
		if ((*tree)->right != NULL) {
			fetchSymbolCode(&(*tree)->right, symbol, strcat(temp, "1"), &*code);
		}
			
		return;
	}
	
	void fetchNytCode(node **tree, char *do_code, queue<char> *code) {
		char temp[strlen(do_code)+1];
		if ((*tree)->weight == 0 && (*tree)->left == NULL && (*tree)->right == NULL) {
			for (int i=0; i<strlen(do_code); i++) {
				(*code).push(do_code[i]);
			}
			
			return;
		}
		cout << "\rCompressing...";
		
		strcpy(temp, do_code);
		if ((*tree)->left != NULL) {
			fetchNytCode(&(*tree)->left, strcat(temp, "0"), &*code);
		}
		
		strcpy(temp, do_code);
		if ((*tree)->right != NULL) {
			fetchNytCode(&(*tree)->right, strcat(temp, "1"), &*code);
		}
			
		return;
	}
	
	void fetchCode(unsigned char symbol, queue<char> *code) {
		for (int i=0; i<8; i++) {
			if ((symbol & 0x80) == 0x80) {
				(*code).push('1');
				
			} else {
				(*code).push('0');
				
			}
			symbol <<= 1;
		}
		
		return;
	}
				
	void writeCodeToFile(ofstream *file, queue<char> *code) {

		unsigned char temp;
		temp = temp & 0x00;
		for (int i=0; i<8; i++) {
			if ((*code).front() == '1') {
				temp ^= 0x01;
			}
			
			if (i != 7) {
				temp <<= 1;
			}
			
			(*code).pop();
		}
		*file << temp;
		return;
	}

	void encode(node **tree, unsigned char symbol, unsigned char *dictionary, queue<char> *code, ofstream *outStream, node **nyt) {

		if (dictionary[(int) symbol] == 0x01) {
			char do_code[1];
			do_code[0] = '\0';
			
			fetchSymbolCode(&*tree, symbol, do_code, &*code);
			
		} else {
			char do_code[1];
			do_code[0] = '\0';
			
			if (*tree != NULL){
				fetchNytCode(&*tree, do_code, &*code);
			}
			fetchCode(symbol, &*code);
		}
		
		// call updateTree procedure
		updateTree(&*tree, symbol, dictionary, &*nyt);
		
		// write to file
		while ((*code).size() >= 8) {
			writeCodeToFile(&*outStream, &*code);\
		}
		
		return;
	}
	
	/**
	 * DECODE
	 */
	
	bool readCode(ifstream *inputFile, queue<char> *code) {
		char temp;
		
		temp &= 0x00;
		
		if ((*inputFile).get(temp)) {
			for (int i=0; i<8; i++) {
				if ((temp & 0x80) == 0x80) {
					(*code).push('1');
					
				} else {
					(*code).push('0');
					
				}
				temp <<= 1;
			}
			
			return true;
			
		} else {
			return false;
			
		}
	}
	
	void fetchSymbolFromCode(queue<char> *code, unsigned char *symbol) {
		unsigned char temp;
		temp &= 0x00;
		for (int i=0; i<8; i++) {
			if ((*code).front() == '1') {
				temp ^= 0x01;
			}
			
			if (i != 7) {
				temp <<= 1;
			}
			
			(*code).pop();
		}
		symbol[0] = temp;
		
		return;
	}
	
	void writeToOutput(ofstream *file, unsigned char symbol) {
		*file << symbol;
		
		return;
	}
	
	void decode(node **tree, unsigned char *dictionary, queue<char> *code, ifstream *instream, ofstream *outstream, node **nyt, bool *oke, short offset) {
		// 4 byte
		while ((*code).size() < 32 && *oke) {
			*oke = readCode(&*instream, &*code);
		}
		
		node *temp = *tree;
		
		if (temp == NULL) {
			unsigned char symbol[1];
			fetchSymbolFromCode(&*code, symbol);
			writeToOutput(&*outstream, symbol[0]);
			
			// call update procedure
			updateTree(&*tree, symbol[0], dictionary, &*nyt);
			
		} else {
			while (temp->left != NULL && temp->right != NULL && (*code).size() > offset) {
				if ((*code).front() == '0') {
					temp = temp->left;
					(*code).pop();
					
				} else {
					temp = temp->right;
					(*code).pop();
					
				}
				
				// 4 byte
				while ((*code).size() < 32 && *oke) {
					*oke = readCode(&*instream, &*code);
				}
				
			}
			
			if ((*code).size() == offset && temp->left != NULL && temp->right != NULL) {
				return;
			}
			
			if (temp->weight == 0) {
				unsigned char symbol[1];
				fetchSymbolFromCode(&*code, symbol);
				writeToOutput(&*outstream, symbol[0]);
				
				// call update tree procedure
				updateTree(&*tree, symbol[0], dictionary, &*nyt);
				
			} else {
				writeToOutput(&*outstream, temp->symbol);
				
				// call update tree procedure
				updateTree(&*tree, temp->symbol, dictionary, &*nyt);
				
			}
		}
		
		return;
	}
	
	bool readSymbol(ifstream *inputFile, unsigned char *symbol) {
		char temp;
		if ((*inputFile).get(temp)) {
			symbol[0] = temp;
			return true;
			
		} else {
			return false;
			
		}
	}
	
	/**
	 * COMPRESS
	 */
	
	void compress(ifstream *in, ofstream *out) {
		node *root = NULL;
		node *nyt = NULL;
		
		unsigned char dictionary[SYMBOL];
		memset(dictionary, 0, SYMBOL);
		
		queue<char> code_write;
		unsigned char symbol[1];
		
		unsigned short offset = 0;
		
		// initiate file with offset
		*out << (unsigned char) 0x00;
		
		while (readSymbol(&*in, symbol)) {
			// printf("in the compress while loop: count->%d\n", count);
			encode(&root, symbol[0], dictionary, &code_write, &*out, &nyt);
		}
		
		// write to file for offset
		if (code_write.size() > 0) {
			offset = 8 - code_write.size();
			
			while (code_write.size() < 8) {
				code_write.push('0');
			}
			
			writeCodeToFile(&*out, &code_write);
			
			(*out).seekp(0);
			
			char write_offset[1];
			write_offset[0] = (char) offset;
			
			(*out).write(write_offset, 1);
		}
		printf("Done!!!\n");
		deleteTree(&root);
		
		return;
	}
	
	/**
	 * DECOMPRESS
	 */
	
	void decompress(ifstream *in, ofstream *out) {
		node *root = NULL;
		node *nyt = NULL;
		
		unsigned char dictionary[SYMBOL];
		memset(dictionary, 0, SYMBOL);
		
		queue<char> code_read;
		
		bool oke = true;
		
		unsigned short offset = 0;
		char temp = 0x00;
		(*in).get(temp);
		offset = (unsigned short) temp;
		
		do {
			decode(&root, dictionary, &code_read, &*in, &*out, &nyt, &oke, offset);
		} while (code_read.size() > offset || oke);
		
		deleteTree(&root);
		
		return;
	}