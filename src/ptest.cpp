#include <iostream>
#include <set>
#include <iterator>
#include "tree.h"
#include "treeprinter.h"
#include <pthread.h>
#include <ctime>

using namespace lock_free_rbtree;
using namespace std;

int insert_start1 = 1;
int insert_end1 = 50000;

int delete_start1 = 1;
int delete_end1 = 1;

int insert_start2 = 50001;
int insert_end2 = 100000;

int delete_start2 = 3;
int delete_end2 = 3;

int insert_start3 = 100001;
int insert_end3 = 150000;

int delete_start3 = 3;
int delete_end3 = 3;

int insert_start4 = 150001;
int insert_end4 = 200000;

int delete_start4 = 3;
int delete_end4 = 3;

int inserts[2] = {1, 100001};
int deletes[2] = {100000, 200000};

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
        //cout << "Inserted " << *itr << endl;
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
        //cout << "Inserted " << *itr << endl;
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

void *threadfunc3(void *tid) {
    set<int> insert_set;
    set<int> delete_set;

    for (int i = insert_start3; i <= insert_end3; i++) {
        insert_set.insert(i);
    }

    for (int i = delete_start3; i <= delete_end3; i++) {
        delete_set.insert(i);
    }

    for (auto itr = insert_set.begin(); itr != insert_set.end(); itr++) {
        t->Insert(*itr, *itr);
        //t->print_tree();
        //cout << "Inserted " << *itr << endl;
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

void *threadfunc4(void *tid) {
    set<int> insert_set;
    set<int> delete_set;

    for (int i = insert_start4; i <= insert_end3; i++) {
        insert_set.insert(i);
    }

    for (int i = delete_start4; i <= delete_end3; i++) {
        delete_set.insert(i);
    }

    for (auto itr = insert_set.begin(); itr != insert_set.end(); itr++) {
        t->Insert(*itr, *itr);
        //t->print_tree();
        //cout << "Inserted " << *itr << endl;
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
    set<int> remain_set;
    set<int> delete_set;

    for (int i = insert_start1; i <= insert_end1; i++) {
    	remain_set.insert(i);
    }

    for (int i = insert_start2; i <= insert_end2; i++) {
    	remain_set.insert(i);
    }

    for (int i = insert_start3; i <= insert_end3; i++) {
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

    for (int i = delete_start3; i <= delete_end3; i++) {
        delete_set.insert(i);
        remain_set.erase(i);
    }

    struct timespec code_start, code_end;
    clock_gettime(CLOCK_MONOTONIC, &code_start);
    //code_start = clock();

    pthread_t t1, t2, t3, t4;
    pthread_create(&t1, NULL, threadfunc1, (void *) NULL);
    pthread_create(&t2, NULL, threadfunc2, (void *) NULL);
    pthread_create(&t3, NULL, threadfunc3, (void *) NULL);
    pthread_create(&t4, NULL, threadfunc4, (void *) NULL);
    
    void *status;
	pthread_join(t1, &status);
	pthread_join(t2, &status);
    pthread_join(t3, &status);
    pthread_join(t4, &status);

    //code_end = clock();
    //double msecs = ((double) (code_end - code_start)) * 1000 / CLOCKS_PER_SEC;
    clock_gettime(CLOCK_MONOTONIC, &code_end);
    double elapsed = (code_end.tv_sec - code_start.tv_sec);
    elapsed += (code_end.tv_nsec - code_start.tv_nsec) / 1000000000.0;
    cout << "Execution took " << elapsed << " seconds." << endl;

    //print2D(t->GetRoot());
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