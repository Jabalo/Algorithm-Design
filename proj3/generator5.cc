#include <iostream>
#include <cstdlib> // for atoi
using namespace std;



int main(int argc, char* argv[]){
	if (argc != 2){ // remember, argv[0] is the program name
		cerr << "Wrong number of arguments!" << endl;
		return 1;
	}
	int n = atoi(argv[1]);
	

    for (int i = 1; i <= n / 2; i++) {
        std::cout << "I " << i << std::endl;
    }

    for (int i = n / 2; i >= 1; i--) {
        std::cout << "R " << i << std::endl;
    }

    return 0;
}
