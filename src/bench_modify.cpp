#include <iostream>
#include <set>
#include <iterator>
#include "tree.h"
#include "treeprinter.h"
#include <pthread.h>
#include <ctime>

#define NUM_THREADS 4
#define NUM_NODES 100

using namespace lock_free_rbtree;
using namespace std;

int insert_start[NUM_THREADS];
int insert_end[NUM_THREADS];

int delete_start[NUM_THREADS];
int delete_end[NUM_THREADS];

RBTree<int, int> *t = new RBTree<int, int>();

void *threadfunc(void *tid) {
    int myid = (long) tid;
    printf("Starting thread %d\n", myid);

	set<int> insert_set;
    set<int> delete_set;

    for (int i = insert_start[myid]; i <= insert_end[myid]; i++) {
        insert_set.insert(i);
    }

    for (int i = delete_start[myid]; i <= delete_end[myid]; i++) {
        delete_set.insert(i);
    }

    for (auto itr = insert_set.begin(); itr != insert_set.end(); itr++) {
        t->Insert(*itr, *itr);
        //t->print_tree();
        //cout << "Inserted " << *itr << endl;
        //print2D(t->GetRoot());
        //for (int j = 0; j <= 1000000; j++);
    }

    //for (auto itr = delete_set.begin(); itr != delete_set.end(); itr++) {
    //    t->Remove(*itr);
        //t->print_tree();

        //cout << "Removed " << *itr << endl;
        //cout << "***********************************************************" << endl;
        //print2D(t->GetRoot());
    //}
    return NULL;
}

int main() {
    t->Insert(0,0);
    set<int> remain_set;
    set<int> delete_set;

    for (int i = 0; i < NUM_THREADS; i++) {
        insert_start[i] = 1 + i * NUM_NODES/NUM_THREADS;
        delete_start[i] = 1 + i * NUM_NODES/NUM_THREADS;
        insert_end[i] = (i+1) * NUM_NODES/NUM_THREADS;
        delete_end[i] = (i+1) * NUM_NODES/NUM_THREADS;
    }

    struct timespec code_start, code_end;
    clock_gettime(CLOCK_MONOTONIC, &code_start);
    //code_start = clock();

    pthread_t tids[NUM_THREADS];

    for (int tid = 0; tid < NUM_THREADS; tid++) {
        pthread_create(&tids[tid], NULL, threadfunc, (void *) tid);
    }
    for (int tid = 0; tid < NUM_THREADS; tid++) {
        pthread_join(tids[tid], NULL);
    }

    //code_end = clock();
    //double msecs = ((double) (code_end - code_start)) * 1000 / CLOCKS_PER_SEC;
    clock_gettime(CLOCK_MONOTONIC, &code_end);
    double elapsed = (code_end.tv_sec - code_start.tv_sec);
    elapsed += (code_end.tv_nsec - code_start.tv_nsec) / 1000000000.0;
    cout << "Execution took " << elapsed << " seconds." << endl;

    //print2D(t->GetRoot());
    t->checkBlackDepth();

    for (int i = 1; i < NUM_NODES; i++) {
        TreeNode<int, int> *found = t->Search(i);
        if (found == NULL) {
            cout << "ERROR: Key " << i << " was inserted but not found!!!" << endl;
        }
    }
    t->print_tree();
    return 0;
}
