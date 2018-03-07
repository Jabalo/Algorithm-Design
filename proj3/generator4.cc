#include <iostream>
#include <cstdlib> // for atoi
using namespace std;



int main(int argc, char* argv[]){
	if (argc != 2){ // remember, argv[0] is the program name
		cerr << "Wrong number of arguments!" << endl;
		return 1;
	}
	int n = atoi(argv[1]);



	for(int i = 1; i <= n/2; i++) {
		cout << "I " << i << endl;
	}

	for(int k = 1; k <=n; k++) {
		cout << "F" << n/2 << endl;
	}

	for(int j = n/2; 0 < j; j--) {
		cout << "R" << j << endl;
	}


	return 0;
}
