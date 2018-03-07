#include <iostream>
#include <fstream>
#include <sstream>
#include <string>       // provides string class
#include <cctype>       // provides isalpha() and tolower()
#include <vector>


using namespace std;

//A STRUCT THAT WE MADE!
//---------------------------------------------------------
//We made a struct called "aWord" that contained a string (the word), the number of occurences and another vector that contained the page numbers. 
struct aWord {
	string str;
	int occurence;
	vector<int> pgNums;
};

//We initiate these globally.
vector<aWord> array;
vector<aWord>::iterator it;
int lineNumber=0;

//---------------------------------------------------------

// The following function should be moved into your skiplist.cc file when you
// write that.  It implements the random bit generation.
#include <ctime>                // for time()
#include <cstdlib>              // for rand(), srand(), and RAND_MAX

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
    } else {
      *it = tolower(*it);
    }
  }
}

//THE NEW FUNCTIONS THAT WE MADE!
//-----------------------------------------------------
//Gets the page number which is the line number divided by 40, because theres 40 lines in a page. By dividing we make sure the result doesn't round up. 
int getPageNumber() {
	int result = lineNumber/40;
	result++;
	return result;
}

//Creates a new "aWord" struct, assigns the string word, sets occurence to 1 (because we have now seen the word once), and page number (which is calculated using getPageNumber(). Returns a new "aWord" struct.
aWord createNew(string word) {
	aWord  *aword = new aWord;
	aword->str = word;
	aword->occurence = 1;
	aword->pgNums.push_back(getPageNumber());
	return *aword;
}

//Inserts a string word into a certain position (int) into the array. First creates a new aWord, assigns it the str word, and inserts into our array.
void insert(string word, int position) {
	
	aWord aword = createNew(word);
	array.insert(array.begin()+position,aword);

	return;	
}

//Returns a boolean. We use the compare function to see if the two strings s and array[i].str are equal, if so return true, otherwise return false.
bool contains(string s) {
	for (unsigned int i=0; i< array.size(); i++) {
    if(s.compare(array[i].str) == 0){
		  return true;
    }
	}
		return false;	
}

//Returns a boolean. This one is a contains function for the page number to see if the page number has already been recorded in the page number array. If it is found, return true, otherwise return false. 
bool contains(int pg, aWord entry){
  for(unsigned int i = 0; i< entry.pgNums.size(); i++){
    if(pg == entry.pgNums[i])
      return true;
  }
  return false;
}

//First we use find to see if word is in the array using the contains(string s) function that we made. If it does contain the word we add 1 to number of occurences for that word. Then we use contains(int pg, aWord entry) to see if the page number was already recorded, if not we use push_back to insert the page number into the pgNum array.
//If the word was not found in the array, we want to create a new aWord and insert it into the array and make sure its in alphabetical order. We make sure its in alphabetical order using the compare function. 
void find(string word) {
//Updates pre-existing in array.
  if (contains(word)) {
    for(unsigned int i = 0; i<array.size();i++){
      if(word.compare(array[i].str)==0){
      array[i].occurence++; 
      if(!contains(getPageNumber(), array[i])){
        array[i].pgNums.push_back(getPageNumber());
        break;
      }
    }
    }
  }
//Inserts new word in a organized way when array size is greater than 2.
  if (!contains(word)) {
    for(unsigned int i=1; i<array.size(); i++){ 
      if(word.compare(array[0].str)<0){
        it = array.begin();
        array.insert(it, 1, createNew(word));
        break;
      }
      if(word.compare(array[i-1].str)>0){
        if(word.compare(array[i].str)<0){
          insert(word,i);
          break;
        }
      }
    }
//Inserts words in an organized way when array size is less than 2.
    if(!contains(word)){
      if(!array.empty() && word.compare(array[0].str)<0){
        it = array.begin();
        array.insert(it, 1, createNew(word));
      }
      else{
      array.push_back(createNew(word));
   }
}
}
}

//This prints the entire array into the output file. We have the larger for-loop to print out each aWord and each part of it, and the for-loop inside is for the pgNum since pgNum is a array as well. 
 void printInOrder(ostream & output) {
  for (int i=0; i<array.size(); i++) {
    output << array[i].str<<"(" << array[i].occurence<<")";
    for (int j=0; j<array[i].pgNums.size(); j++) {
      output << array[i].pgNums[j]<<" ";
    }   
    output<<" "<<endl;
  } 
}

	

//-------------------------------------------------------------
//PART OF MAIN WAS CHANGED AS WELL. (LOOK BELOW)

void print(ostream & output, string & word) {
  output << word << endl;
}

int main(int argc, char *argv[]) {
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

//Here we added find(word) so it can put in each word into the array as an aWord. We also added the printInOrder function to print out the array that we created. find(word) opereated on each word, while printInOrder(fout) starts after the two while loops fill the array. We commented out print(fout,word) since we have our new printInOrder function and do not need it anymore.
  string line, word;
  while( !fin.eof() ) {
    lineNumber++;
    getline(fin,line);
    lowercaseWords(line);
    istringstream iss(line, istringstream::in);
    while( iss >> word ) {
	find(word);
      //print(fout, word);
    }
  }
  printInOrder(fout);
}
