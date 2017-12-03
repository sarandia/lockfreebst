#include <iostream>
#include "tree.h"
#include "treeprinter.h"
#include <ctime>
using namespace lock_free_rbtree;
using namespace std;

int main() {
	RBTree<int, int> *t = new RBTree<int, int>();
	int start = 1, finish = 10;
  	
	cout << "Successfully created rb tree object" << endl;
	clock_t code_start, code_end;
    code_start = clock();

	for (int i = start; i <= finish; i++) {
		t->Insert(i,i);
		//t->print_tree();
		//print2D(t->GetRoot());
		cout << "Inserted " << i << endl;
		/*if (!t->checkBlackDepth()) {
			cout << "ERROR: checkBlackDepth() failed!" << endl;
			exit(1);
		}*/
	}
	code_end = clock();
    double msecs = ((double) (code_end - code_start)) * 1000 / CLOCKS_PER_SEC;
    cout << "Execution took " << msecs << " miliseconds." << endl;

	//print2D(t->GetRoot());
	for (int i = start; i <= finish; i++) {
		TreeNode<int, int> *found = t->Search(i);
		if (found == NULL) {
			cout << "ERROR: Key " << i << " was inserted but not found!!!" << endl;
		}
	}
	t->print_tree();
  	return 0;
}
