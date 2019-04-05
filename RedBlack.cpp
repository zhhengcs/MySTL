#include <stdint.h>
#include <stdlib.h>
#include "RedBlack.h"
#include <iostream>
#include <random>
using std::default_random_engine;
using namespace std;

bool printSquares(uint8_t,int*);


RedBlack<int, uint8_t> rbTree(true);


int main(int argc, char const *argv[])
{
	default_random_engine e;
	
	for (int i = 1; i < 9; ++i) {
		int key = e() % 16;
		cout << key << endl;

		int* tmp = rbTree.nodeSearch(key);
		if (tmp) {
			rbTree.deleteNode(key);
		}
		rbTree.insertNode(key, &i);
	}

	cout << rbTree.nodeCount(false) << " Total nodes\n";
	cout << rbTree.nodeCount(true) << " Black nodes\n";
	cout << "Black Height: " << rbTree.blackHeight() << '\n';
	cout << "Preorder traversal" << endl;
	rbTree.iterate(printSquares);
	cout << "Post order traversal" << endl;
	rbTree.levelIterate(printSquares);
	return 0;
}

bool printSquares(uint8_t key,int* value){
	// cast the uint8_t as otherwise it is treated as a char by cout
	cout << "Key " << (int)key << " Value£º " << *value << '\n';
	return true;
}
