#include <iostream>
#include <set>
#include <iterator>
#include "tree.h"

using namespace lock_free_rbtree;
using namespace std;

int main() {
    RBTree<int, int> *t = new RBTree<int, int>();

    cout << "Successfully created rb tree object" << endl;
    int insert_start = -5;
    int insert_end = 10;

    int delete_start = 1;
    int delete_end = 5;

    set<int> insert_set;
    set<int> delete_set;
    set<int> remain_set;

    for (int i = insert_start; i <= insert_end; i++) {
        insert_set.insert(i);
        remain_set.insert(i);
    }

    for (int i = delete_start; i <= delete_end; i++) {
        delete_set.insert(i);
        remain_set.erase(i);
    }



    for (auto itr = insert_set.begin(); itr != insert_set.end(); itr++) {
        t->Insert(*itr, *itr);
        //t->print_tree();
        cout << "Inserted " << *itr << endl;
    }

    for (auto itr = remain_set.begin(); itr != remain_set.end(); itr++) {
        TreeNode<int, int> *found = t->Search(*itr);
        if (found == NULL) {
            cout << "ERROR: Key " << *itr << " was inserted but not found!!! (Before delete)" << endl;
        }
    }

    t->print_tree();
    cout << "***********************************************************" << endl;

    for (auto itr = delete_set.begin(); itr != delete_set.end(); itr++) {
        t->Remove(*itr);
        t->print_tree();

        cout << "Removed " << *itr << endl;
        cout << "***********************************************************" << endl;
    }
    for (auto itr = remain_set.begin(); itr != remain_set.end(); itr++) {
        TreeNode<int, int> *found = t->Search(*itr);
        if (found == NULL) {
            cout << "ERROR: Key " << *itr << " was inserted but not found!!!" << endl;
        }
    }

    for (auto itr = delete_set.begin(); itr != delete_set.end(); itr++) {
        TreeNode<int, int> *found = t->Search(*itr);
        if (found != NULL) {
            cout << "ERROR: Key " << *itr << " was deleted but found!!!" << endl;
        }
    }
    t->print_tree();
    return 0;
}