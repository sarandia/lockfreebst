#include <iostream>
#include "tree.h"

using namespace lock_free_rbtree;
using namespace std;

int main() {
	RBTree<int, int> *t = new RBTree<int, int>();
  	
	cout << "Successfully created rb tree object" << endl;
	t->Insert(5,5);
	cout << "Inserted 5" << endl;
	t->Remove(5);
  	return 0;
}
