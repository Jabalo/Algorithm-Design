#include <iostream>
#include <vector> 
#include <cstdlib>  

struct aWord {
	string str;
	int occurence;
	vector<int> pgNums;
};

struct SkipListNode {
	aWord worddata;
	int height;
	vector<SkipListNode *> next; //Vector of pointers to next nodes.
};

SkipListNode * head = new SkipListNode;
head->worddata = NULL;
head->height = RAND_MAX;
head->next = NULL;






	
