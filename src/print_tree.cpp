#include <iostream>
#include "tree.h"

using namespace lock_free_rbtree;
using namespace std;

int main() {
	RBTree<int, int> *t = new RBTree<int, int>();
  	
	cout << "Successfully created rb tree object" << endl;
	for (int i = 5; i > -6; i--) {
		t->Insert(i,i);
		t->print_tree();
		cout << "Inserted " << i << endl;
	}
	for (int i = 5; i > -6; i--) {
		TreeNode<int, int> *found = t->Search(i);
		if (found == NULL) {
			cout << "ERROR: Key " << i << " was inserted but not found!!!" << endl;
		}
	}
	t->Remove(5);
	t->print_tree();
	cout << "Removed 5" << endl;
  	return 0;
}
