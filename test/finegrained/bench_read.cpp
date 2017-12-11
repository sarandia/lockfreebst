#include <iostream>
#include <set>
#include <iterator>
#include "tree.h"
#include "treeprinter.h"
#include <pthread.h>
#include <ctime>

#define NUM_THREADS 4
#define NUM_NODES 500000

using namespace lock_free_rbtree;
using namespace std;

int insert_start[NUM_THREADS];
int insert_end[NUM_THREADS];

int delete_start[NUM_THREADS];
int delete_end[NUM_THREADS];

set<int> insert_set[NUM_THREADS];
set<int> delete_set[NUM_THREADS];

RBTree<int, int> *t = new RBTree<int, int>();

void *threadfunc(void *tid) {
    int myid = (long) tid;
    printf("Starting thread %d\n", myid);
    for (int i = 0; i < 100; i++) {
        for (auto itr = insert_set[myid].begin(); itr != insert_set[myid].end(); itr++) {
            t->Search(*itr);
        }
    }

    return NULL;
}

int main() {
    t->Insert(0,0);

    for (int i = 0; i < NUM_THREADS; i++) {
        insert_start[i] = 1 + i * NUM_NODES/NUM_THREADS;
        delete_start[i] = 1 + i * NUM_NODES/NUM_THREADS;
        insert_end[i] = (i+1) * NUM_NODES/NUM_THREADS;
        delete_end[i] = (i+1) * NUM_NODES/NUM_THREADS;


        for (int j = insert_start[i]; j<= insert_end[i]; j++) {
            insert_set[i].insert(j);
        }

        for (int j = delete_start[i]; j <= delete_end[i]; j++) {
            delete_set[i].insert(j);
        }
    }

    for (int i = 1; i < NUM_NODES; i++) {
        t->Insert(i,i);
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
    //t->checkBlackDepth();

    /*for (int i = 1; i < NUM_NODES; i++) {
        TreeNode<int, int> *found = t->Search(i);
        if (found == NULL) {
            cout << "ERROR: Key " << i << " was inserted but not found!!!" << endl;
        }
    }*/
    t->print_tree();
    return 0;
}
