#include <iostream>
#include "tree.h"

using namespace lock_free_rbtree;

void print_tree(RBTree* t) {

}

int main() {
  print_tree(new RBTree<int, int>());
  return 0;
}
