#include <iostream>
#include "tree.h"
#include "treeprinter.h"
using namespace lock_free_rbtree;
using namespace std;

int main() {
	RBTree<int, int> *t = new RBTree<int, int>();
	int start = 1, finish = 10000;
  	
	cout << "Successfully created rb tree object" << endl;
	for (int i = start; i <= finish; i++) {
		t->Insert(i,i);
		//t->print_tree();
		print2D(t->GetRoot());
		cout << "Inserted " << i << endl;
		if (!t->checkBlackDepth()) {
			cout << "ERROR: checkBlackDepth() failed!" << endl;
			exit(1);
		}
	}
	for (int i = start; i <= finish; i++) {
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
