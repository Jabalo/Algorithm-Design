#include <iostream>
#include <vector> 
#include <cstdlib> 
#include <string>

using namespace std; 

int MAX_HEIGHT = 16;

struct aWord {
	string str;
	int occurence;
	vector<int> pgNums;
};

struct SkipListNode {
	aWord worddata;
	int height;
	SkipListNode** next; //Array of pointers to next nodes.

};



SkipListNode* head;
int lineNumber = 0;







	
