#include <iostream>
#include <fstream>
#include <algorithm>
#include "codetree.h"
#include <vector>
using namespace std;

vector<TreeNode*> nodeArray; //Global Array.

//Creating a new TreeNode, and returning it.
TreeNode* createNode(int frequency, int i) {
	TreeNode * result = new TreeNode;
	result->letter = i;
	result->frequency = frequency;
	result->right = NULL;
	result->left = NULL;
	return result;
}

//Just inserting into the global array nodeArray. There is no specific order yet (not sorted). Inserting in the order that main() reads it.	
void insert(int frequency, int i){
	TreeNode* node = createNode(frequency,i);
	nodeArray.push_back(node);
}

//Finding the minimum frequency to build our Code Tree. We set the minimum to the first node in nodeArray, then go through nodeArray to find the minimum. Since makeTree calls findMin() twice, if we don't erase it we will always get the same minimum (we need the two lowest frequency nodes).
TreeNode* findMin(){
	TreeNode* minNode = nodeArray[0];
	int position = 0;
	for(int i = 0; i < nodeArray.size(); i++){
		if(minNode->frequency > nodeArray[i]->frequency){
			minNode = nodeArray[i];
			position = i;
		}
	}
	nodeArray.erase(nodeArray.begin()+position);
	return minNode;
}

//Making the Code Tree based on the Huffman Algorithm. We have the two minimums with the lowest frequency. Then add the frequencies of those to make the parent of the two nodes. This node's frequency is set to -1, since -1 is not associated with any character. Then we add this node to our global array nodeArray. We call makeTree() again to recursively do this and stop when nodeArray size is 0 or 1.
void makeTree(){
	if(nodeArray.size() == 0 || nodeArray.size() ==1){
		return;
	}
	TreeNode* combNode;
	TreeNode* minNode1 = findMin();
	TreeNode* minNode2 = findMin();
	int frequency = (minNode1->frequency) + (minNode2->frequency);
	combNode = createNode(frequency, -1);
	combNode->left = minNode1;
	combNode->right = minNode2;
	nodeArray.push_back(combNode);
	makeTree();
}
//---------------------------------------------------------


void printChar(int ch) {
	if (ch < 16) {
		std::cout << "x0" << std::hex << ch;
	}
	else if (ch < 32 || ch > 126) {
		std::cout << "x" << std::hex << ch;
	}
	else {
		std::cout << "\"" << (char)ch << "\"";
	}
}


// Code provided from Piazza.
// Uses recursion to number left branches with "0" and right branches with "1".
void printTree(TreeNode* r, std::string pre) {
	if (r == NULL) return;
	if (r->right == NULL) { //implies r->left == NULL
		printChar(r->letter);
		std::cout << std::endl;
	}
	else {
		std::cout << "." << std::endl;
		std::cout << pre << "|`1-";
		printTree(r->right, pre + "|   ");
		std::cout << pre + "|   " <<std::endl;
		std::cout << pre << "`-0-`";
		printTree(r->left, pre + "   ");
	}
}
//Prints the tree in the format of:
// char: code(in binary)
void printCode(TreeNode* r, string add) {
	r->code = add;
	if (r == NULL) return;
	if (r->left == NULL&&r->right==NULL){ //implies node is leaf
		printChar(r->letter);	//prints character and its code
		std::cout<<":"<<" ";
		std::cout<<r->code<<"";
		std::cout << std::endl;
	}
	else {
		r->left->code = r->code +"0";		//sets code of left child to its parent's plus "0"
		r->right-> code = r->code+"1";		//sets code of right child to its parent's plus "1"
		printCode(r->left,r->left->code);	//left child's code gets carried over to combine with its children's
		printCode(r->right, r->right->code);//right child's code gets carried over to combine with its children's 
	}
}
// Print the Huffman code tree and list of codes for a given text file.
void usage() {
  std::cerr << "Usage: huftree text.txt" << std::endl;
  throw std::exception();
}
int main( int argc, char *argv[] ) {
  if( argc != 2 ) {
    usage();
  }
  int freq[256] = { 0 };	// array initialized to 0
  
  std::ifstream fin( argv[1] );
  if( fin.is_open() ) {
    char ch;
    while (fin >> std::noskipws >> ch) {	// don't skip whitespace
      freq[(int)ch]++;
    }
    for(int i = 0; i < 256; i++){
	    if(freq[i]!=0){	
	    	insert(freq[i], i);		//makes an array of characters in form of node
		}
	}
    makeTree();					//combines all node in array into a tree of nodes 	
    printTree(nodeArray[0],"");	
    printCode(nodeArray[0],"");
    fin.close();
  }
  return 0;
}
