#include <iostream>
#include <set>
#include <iterator>
#include "tree.h"
#include "treeprinter.h"
#include <pthread.h>

using namespace lock_free_rbtree;
using namespace std;

int insert_start1 = 1;
int insert_end1 = 3;

int delete_start1 = 1;
int delete_end1 = 1;

int insert_start2 = 5;
int insert_end2 = 7;

int delete_start2 = 3;
int delete_end2 = 3;

RBTree<int, int> *t = new RBTree<int, int>();

void *threadfunc1(void *tid) {
	set<int> insert_set;
    set<int> delete_set;

    for (int i = insert_start1; i <= insert_end1; i++) {
        insert_set.insert(i);
    }

    for (int i = delete_start1; i <= delete_end1; i++) {
        delete_set.insert(i);
    }

    for (auto itr = insert_set.begin(); itr != insert_set.end(); itr++) {
        t->Insert(*itr, *itr);
        //t->print_tree();
        cout << "Inserted " << *itr << endl;
        //print2D(t->GetRoot());
        //for (int j = 0; j <= 1000000; j++);
    }

    /*for (auto itr = delete_set.begin(); itr != delete_set.end(); itr++) {
        t->Remove(*itr);
        //t->print_tree();

        cout << "Removed " << *itr << endl;
        //cout << "***********************************************************" << endl;
        //print2D(t->GetRoot());
    }*/
    return NULL;
}

void *threadfunc2(void *tid) {
	set<int> insert_set;
    set<int> delete_set;

    for (int i = insert_start2; i <= insert_end2; i++) {
        insert_set.insert(i);
    }

    for (int i = delete_start2; i <= delete_end2; i++) {
        delete_set.insert(i);
    }

    for (auto itr = insert_set.begin(); itr != insert_set.end(); itr++) {
        t->Insert(*itr, *itr);
        //t->print_tree();
        cout << "Inserted " << *itr << endl;
    }

    /*for (auto itr = delete_set.begin(); itr != delete_set.end(); itr++) {
        t->Remove(*itr);
        //t->print_tree();

        cout << "Removed " << *itr << endl;
        //cout << "***********************************************************" << endl;
        //print2D(t->GetRoot());
    }*/
    return NULL;
}

int main() {
    //t->Insert(1000000, 1000000);

    set<int> remain_set;
    set<int> delete_set;

    for (int i = insert_start1; i <= insert_end1; i++) {
    	remain_set.insert(i);
    }

    for (int i = insert_start2; i <= insert_end2; i++) {
    	remain_set.insert(i);
    }

    for (int i = delete_start1; i <= delete_end1; i++) {
    	delete_set.insert(i);
    	remain_set.erase(i);
    }

    for (int i = delete_start2; i <= delete_end2; i++) {
    	delete_set.insert(i);
    	remain_set.erase(i);
    }

    pthread_t t1, t2;
    pthread_create(&t1, NULL, threadfunc1, (void *) NULL);
    pthread_create(&t2, NULL, threadfunc2, (void *) NULL);
    
    void *status;
	pthread_join(t1, &status);
	pthread_join(t2, &status);
    print2D(t->GetRoot());
    t->checkBlackDepth();

    for (auto itr = remain_set.begin(); itr != remain_set.end(); itr++) {
        TreeNode<int, int> *found = t->Search(*itr);
        if (found == NULL) {
            cout << "ERROR: Key " << *itr << " was inserted but not found!!!" << endl;
        }
    }

    /*for (auto itr = delete_set.begin(); itr != delete_set.end(); itr++) {
        TreeNode<int, int> *found = t->Search(*itr);
        if (found != NULL) {
            cout << "ERROR: Key " << *itr << " was deleted but found!!!" << endl;
        }
    }*/
    t->print_tree();
    return 0;
}