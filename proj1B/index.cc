#include <iostream>
#include <fstream>
#include <sstream>
#include <string>       // provides string class
#include <cctype>       // provides isalpha() and tolower()
#include <vector>
#include <ctime>                // for time()
#include <cstdlib>              // for rand(), srand(), and RAND_MAX
#include "index.h"
using namespace std;

//Keeps track of the nodes visited.
SkipListNode* update[16];

// The following function should be moved into your skiplist.cc file when you
// write that.  It implements the random bit generation.
int randBit(void) {             // return a "random" bit
	static int bitsUpperBd=0;
	static int bits;              // store bits returned by rand()
	if( bitsUpperBd == 0 ) {      // refresh store when empty
		bitsUpperBd = RAND_MAX;
		bits = rand();
 	}
  	int b = bits & 1;
  	bits >>= 1;
  	bitsUpperBd >>= 1;
  	return b;
}

// Remove all characters except letters (A-Z,a-z) from line,
// except keep '-' or '\'' if they are between letters.
void lowercaseWords(string & line) {
	for( string::iterator it = line.begin(); it != line.end(); ++it ) {
    		if( !isalpha(*it) ) {
      			if( (*it != '-' && *it != '\'') ||
          			it == line.begin() || it+1 == line.end() ||
          			!isalpha(*(it-1)) || !isalpha(*(it+1)) ) {
       					 *it = ' ';
     			 }
   		 }
		 else {
      			*it = tolower(*it);
   		 }
 	 }
}

//THE NEW FUNCTIONS THAT WE MADE!
//-----------------------------------------------------
//We decided to initiate our head node by simply creating one in a seperate function.
void create_headNode() {
	head = new SkipListNode; //We make head (which was declared in index.h) a new SkipListNode.
	head->height = MAX_HEIGHT; //The height of the head node will have maximum height (which we declared to be 16 (number they used on project outline).
	SkipListNode ** array = new SkipListNode *[head->height]; //We initiate the next array of pointers.
	head->next = array; //We assign array we just created (next nodes) to head->next;
	for (int i=0; i< head->height; i++) { //The next array pointers are all NULL for the head node. 
   		 head->next[i] = NULL;
  	}
  	return;
}

int getPageNumber() { //Code same as proj1A.
 	int result = lineNumber/40; //There are 40 lines in a page, so we divide lineNumber by 40 to get the page number.
  	result++;
  	return result;
}

aWord createWord(string word) { //Code same as proj1A.
	aWord  *aword = new aWord; //Create a new word, assign the string word, make occurence 1 and call getPageNumber() to make an array of page numbers. We use push_back to add to this array.
  	aword->str = word;
  	aword->occurence = 1;
  	aword->pgNums.push_back(getPageNumber());
  	return *aword;
}


SkipListNode * createNode(string word) {//Just creating a regular node.
	SkipListNode * newNode = new SkipListNode; 
	newNode->worddata = createWord(word); //Assign data using createWord(word).
  	newNode->height = randBit(); //The height of the node is a random bit.
  	SkipListNode ** array = new SkipListNode *[newNode->height]; //This is the array for next pointers.
 	newNode->next = array; //We assign above array to newNode->next;
  	return newNode;
}
 
//Goes through the SkipList starting from last element in the head node.
//If the element has no pointers then goes on to the next element in the list.
//If the element has a pointer goes through the skiplist until the word is found.
//Returns NULL if the word does not exist in the skiplist.
SkipListNode * find(string word) {
	SkipListNode* node = head;
  	SkipListNode* next;
  		for(int h = MAX_HEIGHT-1; h>0;h--){
   			update[h] = node; //Saves the visited nodes starting from update[MAX_HEIGHT-1].
    			while(node->next[h]!=NULL){
     				update[h] = node;
      				next = node->next[h];
      				if(word.compare(node->worddata.str)==0) //Checks if the word matches word in entry.
        				return node;
      				else if(word.compare(next->worddata.str)>0) // Checks if the word is greater than word in entry.
     					node = next; //Looks at the word the pointer leads to.
   			 }
  		}
 		 return NULL; //Word was not found in the SkipList.
}

//Inserts the word into the SkipList if the word was not found in the SkipList.
//First we call find(word) to see if the word exists in the SkipList and keeps track of visited nodes with update[].
//If NULL create a new node x and assign update->next to x->next for h. Then we update the next pointers that change.
void insert(string word) {
	SkipListNode * entry = find(word);
  		if (entry == NULL) {
   			SkipListNode * x = createNode(word);
   			for(int h=0; h<x->height; h++) {
    				x->next[h] = update[MAX_HEIGHT-1-h]->next[h]; //Initialize x's next pointers.
    				update[MAX_HEIGHT-1-h]->next[h] = x; //Update next pointers that change.
  			 } 
  		}
 		 else{
  			 entry->worddata.occurence++;
   			 entry->worddata.pgNums.push_back(getPageNumber()); //If not NULL, add to occurences and add the page number to pgNums[].
  		}
  		return;
}

//Prints each SkipListNode in the SkipList using recursion. 
//We only look at next[0] because it contains all the data (string, occurence, page numbers).
void printInOrder(SkipListNode * node, ostream & output) {
	if (node->next[0] == NULL) { //Will stop printing when node->next does not exist.
    		return;
  	}
 	output << node->worddata.str << "(" << node->worddata.occurence <<")";
  	for (unsigned int i=0; i<node->worddata.pgNums.size(); i++) {
        	output << node->worddata.pgNums[i]<<" "<<endl;
 	}
  	printInOrder(node->next[0], output); //We use recursion here to print node->next.
  
}

//-------------------------------------------------------------
//PART OF MAIN WAS CHANGED AS WELL. (LOOK BELOW)
void print(ostream & output, string & word) {
  output << word << endl;
}
int main(int argc, char *argv[]) {
  create_headNode();
  if( argc != 3 ) {
    cerr << "Usage: " << argv[0] << " book.txt index.txt" << endl;
    exit(1);
  }
  ifstream fin;
  fin.open(argv[1]);
  if( !fin.is_open() ) {
    cerr << "Unable to open " << argv[1] << endl;
    exit(2);
  }
  ofstream fout;
  fout.open(argv[2]);
  if( !fout.is_open() ) {
    cerr << "Unable to open " << argv[2] << endl;
    exit(3);
  }
//Here we added insert(word), which calls find(word);
  string line, word;
  while( !fin.eof() ) {
    lineNumber++;
    getline(fin,line);
    lowercaseWords(line);
    istringstream iss(line, istringstream::in);
    while( iss >> word ) {
      insert(word);
    }
  }
  printInOrder(head, fout); //printInOrder will start after all the words of every line has been run through.
}
