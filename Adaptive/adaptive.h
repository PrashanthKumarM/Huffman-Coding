#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>

#ifndef ADAPTIVEHUFFMAN
#define ADAPTIVEHUFFMAN

#define SYMBOL 2048
#define NUMBER 4096
using namespace std;

typedef struct treenode {
   unsigned char symbol; // symbol
   int weight,           // weight
       order;           // number
  treenode *parent,         // parent
       *left,           // left child
       *right;          // right child
}node;

void newNode(node **leaf, unsigned char symbol, bool is_nyt);
void joinNodes(node **tree, node *left, node *right);
void deleteTree(node **tree) ;
void findNextBlock(node **tree, int weight, int *number, int parent_number, node **position, char *l_r);
void swapNodes(node *tree, char *l_r, node *sibling, char *l_r_sibling) ;
void treeTraversal(node **tree, vector<pair<int, node*> > *queue, int deep);
void updateWeight(node **tree);
void findSymbolLeaf(node **tree, unsigned char symbol, node **position);
bool isSort(pair<int, node*> p, pair<int, node*> q);
void updateTree(node **tree, unsigned char symbol, unsigned char *dictionary, node **nyt);
void fetchSymbolCode(node **tree, unsigned char symbol, char *do_code, queue<char> *code_write);
void fetchNytCode(node **tree, char *do_code, queue<char> *code_write);
void fetchCode(unsigned char symbol, queue<char> *code_write);
void writeCodeToFile(ofstream *file, queue<char> *code_write) ;
void encode(node **tree, unsigned char symbol, unsigned char *dictionary, queue<char> *code_write, ofstream *file, node **nyt) ;
bool readCode(fstream *file, queue<char> *code_read);
void fetchSymbolFromCode(queue<char> *code_read, unsigned char *character);
void writeToOutput(ofstream *file, unsigned char symbol);
void decode(node **tree, unsigned char *dictionary, queue<char> *code_read, ifstream *file, ofstream *out_file, node **nyt, bool *oke, short offset);
bool readSymbol(ifstream *file, unsigned char *symbol);
void compress(ifstream *in, ofstream *out);
void decompress(ifstream *in, ofstream *out);

#endif