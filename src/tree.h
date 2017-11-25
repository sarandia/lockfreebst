/* based on Tarjan's Top-down RB Tree Algorithm */
#include <vector>
#include <iterator>

namespace lock_free_rbtree {

enum color_t {red, black};

template <typename KeyType, typename ValueType>
class RBTree;

template <typename KeyType, typename ValueType>
class TreeNode {
  
  friend class RBTree<KeyType, ValueType>;
  
  public:
  TreeNode(bool ext) {
    isExternal_ = ext;
  }

  bool IsExternal();
  ValueType GetValue();
  bool ReplaceChild(TreeNode<KeyType, ValueType> *oldchld, TreeNode<KeyType, ValueType> *newchld) {
    if (left == oldchld) {
      left = newchld;
      return true;
    }
    if (right == oldchld) {
      right = newchld;
      return true;
    }
    return false;
  }
  
  private:
  KeyType key;
  color_t color;
  TreeNode *left;
  TreeNode *right;
  bool isExternal_;
  ValueType value;
};

template <typename KeyType, typename ValueType>
class RBTree {
  
  public:
	RBTree();
  ~RBTree();
  
  typedef TreeNode<KeyType, ValueType> treenode_t;

  treenode_t * Search(KeyType key);
  
  void Insert(KeyType key, ValueType value);
  
  void Remove(KeyType key);
  
  private:
  treenode_t *root_;
  void fix_insert(std::vector<treenode_t*> &v);
  bool has_red_child_or_grandchild(treenode_t *cur);
  bool has_red_child(treenode_t *cur);
  void fix_delete(std::vector<treenode_t *> &v);
  void swapNodes(treenode_t *node1, treenode_t *node2);
  void rotateLeft(treenode_t *parent);
  void rotateRight(treenode_t *parent);
};

template <typename KeyType, typename ValueType>
bool TreeNode<KeyType, ValueType>::IsExternal() {
  return isExternal_;
}

template <typename KeyType, typename ValueType>
RBTree<KeyType, ValueType>::RBTree() {
  root_ = NULL;
}

template <typename KeyType, typename ValueType>
RBTree<KeyType, ValueType>::~RBTree() {
// TODO
}

template <typename KeyType, typename ValueType>
TreeNode<KeyType, ValueType> *RBTree<KeyType, ValueType>::Search(KeyType key) {
// TODO
}

template <typename KeyType, typename ValueType>
void RBTree<KeyType, ValueType>::Insert(KeyType key, ValueType value) {
// 1. make the top-down invariant true initially
  treenode_t *x = root_;
  if (x->color == red) x->color = black;
  if (x->left != NULL && x->left->color == red \
      && x->right != NULL && x->right->color == red) {
    x->left->color = black;
    x->right->color = black;
  }
  // 2. walking down from the current node
  treenode_t *y = x;
  treenode_t *prev;
  std::vector<treenode_t *> q;
  int successiveBlk = 0;
  while (!y->IsExternal()) {
    // check if node is black with 2 red chilren
    q.push_back(y);
    if (y->color == black) {
      if (y->left == red && y->right == red) {
        successiveBlk ++;
      }
      else {
        successiveBlk = 0;
      }
    }
    else {
      if (prev->color == red) {
        successiveBlk = 0;
      }
    }
    // c. color z red and its two children black, then proceed as bottom-up
    if (successiveBlk == 4) {
      treenode_t *z = y;
      z->color = red;
      z->left->color = black;
      z->right->color = black;
      // fix color problems
      fix_insert(q);
      // replace the current node x by the child of z along the access path
      if (key < z->key) {
        x = z->left;
        y = x;
      }
      else {
        x = z->right;
        y = x;
      }
      successiveBlk = 0;
      q.clear();
      // repeat the general step
      continue;
    }
    prev = y;
    if (key <= y->key) {
      y = y->left;
    }
    else if (key > y->key) { // to be re-visited 
      y = y->right;
    }
    // b. a black node with a black child is reached
    if (prev->color == black && y->color == black) {
      // repeat current node x by y
      x = y;
      q.clear();
    }

  } // while
  // node y is external; must perform bottom-up insertion
  if (y->key == key) return;
  treenode_t *new_int = new treenode_t(false);
  treenode_t *new_item = new treenode_t(true);
  new_item->key = key;
  new_item->value = value;
  if (y->key <= key) {
    new_int->key = y->key;
    new_int->left = y;
    new_int->right = new_item;
  }
  else {
    new_int->key = key;
    new_int->left = new_item;
    new_int->right = y;
  }
  new_int->color = red;
  if (new_int->key <= prev->key) {
    prev->left = new_int;
  }
  else {
    prev->right = new_int;
  }
  q.pop(); // pop off y
  q.push_back(new_int);
  fix_insert(q);
 
}

template <typename KeyType, typename ValueType>
void RBTree<KeyType, ValueType>::fix_insert(std::vector<treenode_t *> &q) {
  bool is_violation = true;
  while (is_violation) {
    is_violation = false;
    treenode_t *cur = q.back();
    if (cur->color == red) {
      treenode_t *parent = *(q.end() - 1);
      treenode_t *sibling;
      if (cur->key <= parent->key) {
        sibling = parent->right;
      }
      else {
        sibling = parent->left;
      }
      if (parent == q[0]) break;
      if (parent->color == red && sibling->color == red) {
        is_violation = true;
        treenode_t *grandparent = *(q.end() - 2);
        grandparent->color = red;
        parent->color = black;
        sibling->color = black;
        // we then go up by 1. The current node is now the parent
        q.pop();
      }
    }
  }

  // if reached the root of the tree, fix the last color violation
  if (q.size() <=2) {
    treenode_t *cur = q[1];
    treenode_t *window_root = q[0];
    treenode_t *sibling;
    int direction = 0;
    if (window_root->left == q[1]) {
      sibling = window_root->right;
    }
    else {
      sibling = window_root->left;
      direction = 1;
    }
    // 3-c, both window_root and cur are red, and all children of cur are black
    // also, the sibling needs to be black
    if (window_root->color == red && sibling->color == black && cur->color == red && \
      cur->left->color == black && cur->right->color == black) {
      // we color root to be black
      window_root->color = black;
    }
    // 3-d and e, cur is red and has a red child
    if (window_root->color == black && cur->color == red && (cur->left->color == red \
      || cur->right->color == red)) {
      // re-color the tree, then rotate
      cur->color = black;
      if (direction == 0) {
        rotateRight(window_root);
      }
      else {
        rotateLeft(window_root);
      }
    }
  }

}

template <typename KeyType, typename ValueType>
bool RBTree<KeyType, ValueType>::has_red_child_or_grandchild(treenode_t *cur) {
  if (cur->left != NULL) {
    if (cur->left->color == red) {
      return true;
    }
    if (has_red_child(cur->left)) {
      return true;
    }
  }
  if (cur->right != NULL) {
    if (cur->right->color == red) {
      return true;
    }
    if (has_red_child(cur->right)) {
      return true;
    }
  }

  return false;
}

template <typename KeyType, typename ValueType>
bool RBTree<KeyType, ValueType>::has_red_child(treenode_t *cur) {
  if (cur->left != NULL && !cur->left.IsExternal() && cur->left->color == red) {
    return true;
  }
  if (cur->right != NULL && !cur->right.IsExternal() && cur->right->color == red) {
    return true;
  }
  return false;
}

template <typename KeyType, typename ValueType>
void RBTree<KeyType, ValueType>::fix_delete(std::vector<treenode_t *> &v) {
  auto r_itr = v.rbegin();

  treenode_t *cur = NULL;
  treenode_t *par = NULL;
  treenode_t *sibling = NULL;

  if (cur->IsExternal()) {

    cur = *r_itr;
    r_itr++;

    if (r_itr != v.rend()) {
      par = *r_itr;
    } else {
      root_ = NULL;
      delete cur;
      return;
    }
    
    if (par->left == cur) {
      sibling = par->right;
    } else {
      sibling = par->left;
    }

    swapNodes(par, sibling);

    delete cur;
    delete sibling;
  }

  while (r_itr != v.rend()) {

    cur = *r_itr;
    r_itr++;
    
    if (r_itr != v.rend()) {
      par = *r_itr;
    } else {
      if (!cur->IsExternal()) {
        if (cur->color = red) {
          cur->color = black;
        }
      }
      return;
    }
    
    if (par->left == cur) {
      sibling = par->right;
    } else {
      sibling = par->left;
    }

    if (par->color == black && sibling->color == black && !sibling->has_red_child()) {
      sibling->color = red;
      continue;
    } else {
      break;
    }
  }

  if (sibling->color == red) {
    par->color = red;
    sibling->color = black;
    if (sibling == par->right) {
      rotateLeft(par);
      cur = par->left->left;
    } else {
      rotateRight(par);
      cur = par->right->right;
    }
  }

  if (par->color == red & &sibling->color == black && !sibling->has_red_child()) {
    par->color = black;
    sibling->color = red;
    return;
  }

  if (sibling == par->right) {
    if (sibling->color == black && sibling->right->color == red) {
      sibling->color = par->color;
      sibling->right->color = black;
      par->color = black;
      rotateLeft(par);
      return;
    }
    if (sibling->color == black && sibling->left->color == red) {
      sibling->left->color = par->color;
      par->color = black;
      rotateRight(sibling);
      rotateLeft(par);
      return;
    }
  } else {
    if (sibling->color == black && sibling->left->color == red) {
      sibling->color = par->color;
      sibling->left->color = black;
      par->color = black;
      rotateRight(par);
      return;
    }
    if (sibling->color == black && sibling->right->color == red) {
      sibling->right->color = par->color;
      par->color = black;
      rotateLeft(sibling);
      rotateRight(par);
      return;
    }
  }
}

template <typename KeyType, typename ValueType>
void RBTree<KeyType, ValueType>::Remove(KeyType key) {
  std::vector<treenode_t *> v;

  treenode_t *curNode = root_;
  int black_count = 0;

  while (true) {

    if (curNode->IsExternal()) {
      v.push_back(curNode);
      fix_delete(v);
      return;
    } else {
      if (curNode->color != black || has_red_child_or_grandchild(curNode)) {
        if (!v.empty()) {
          treenode_t *tempNode = NULL;
          tempNode->v.back();
          v.clear();
          v.push_back(tempNode);
        }
        
        black_count = 0;
      } else {
        black_count++;
      }

      if (black_count == 3) {
        treenode_t *par = *(v.rbegin());
        if (curNode == par->left) {
          par->right->color = red;
        } else {
          par->left->color = red;
        }

        fix_delete(v);
        v.clear();
        v.push_back(par);

        black_count = 0;
      }

      v.push_back(curNode);
      
      if (key <= curNode->key) {
        curNode = curNode->left;
      } else {
        curNode = curNode->right;
      }
    }
  }
}

template <typename KeyType, typename ValueType>
void RBTree<KeyType, ValueType>::swapNodes(treenode_t *node1, treenode_t *node2) {
  KeyType tmpKey = node1->key;
  color_t tmpColor = node1->color;
  treenode_t tmpLeft = node1->left;
  treenode_t tmpRight = node1->right;

  node1->key = node2->key;
  node1->color = node2->color;
  node1->left = node2->left;
  node1->right = node2->right;

  node2->key = tmpKey;
  node2->color = tmpColor;
  node2->left = tmpLeft;
  node2->right = tmpRight;
}

template <typename KeyType, typename ValueType>
void RBTree<KeyType, ValueType>::rotateLeft(treenode_t *parent) {

  treenode_t *tempRight = parent->right;
  parent->right = parent;
  swapNodes(parent, tempRight);

  parent = tempRight;
  
  treenode_t *newPar = parent->right;
  treenode_t *temp = parent->right->left;
  parent->right->left = parent;
  parent->right = temp;
}

template <typename KeyType, typename ValueType>
void RBTree<KeyType, ValueType>::rotateRight(treenode_t *parent) {

  treenode_t *tempLeft = parent->left;
  parent->left = parent;
  swapNodes(parent, tempLeft);

  parent = tempLeft;

  treenode_t *temp = parent->left->right;
  parent->left->right = parent;
  parent->left = temp;
}

}