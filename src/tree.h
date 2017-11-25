/* based on Tarjan's Top-down RB Tree Algorithm */
#include <vector>

namespace lock_free_rbtree {

enum color_t {red, black};

template <typename KeyType, typename ValueType>
class RBTree;

template <typename KeyType, typename ValueType>
class TreeNode {
  
  friend class RBTree<KeyType, ValueType>;
  
  public:
  bool IsExternal();
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
};

template <typename KeyType, typename ValueType>
class ExternalTreeNode : public TreeNode<KeyType, ValueType> {
  
  friend class RBTree<KeyType, ValueType>;
  
  public:
  bool IsExternal();
  ValueType GetValue();
  
  private:
  ValueType value;
};

template <typename KeyType, typename ValueType>
class RBTree {
  
  public:
	RBTree();
  ~RBTree();
  
  typedef TreeNode<KeyType, ValueType> treenode_t;
  typedef ExternalTreeNode<KeyType, ValueType> exttreenode_t;

  treenode_t * Search(KeyType key);
  
  void Insert(KeyType key, ValueType value);
  
  void Remove(KeyType key);
  
  private:
  treenode_t *root_;
  void fix_insert(std::vector<treenode_t*> &v);
  bool has_red_child_or_grandchild(treenode_t *cur);
  void fix_delete(std::vector<treenode_t *> &v);
  void swapNodes(treenode_t *node1, treenode_t *node2);
  void rotateLeft(treenode_t *parent);
  void rotateRight(treenode_t *parent);
};

template <typename KeyType, typename ValueType>
bool TreeNode<KeyType, ValueType>::IsExternal() {
  return false;
}

template <typename KeyType, typename ValueType>
bool ExternalTreeNode<KeyType, ValueType>::IsExternal() {
  return true;
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
  if (x->left != nullptr && x->left->color == red \
      && x->right != nullptr && x->right->color == red) {
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
  treenode_t *new_int = new treenode_t();
  exttreenode_t *new_item = new exttreenode_t();
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

}

template <typename KeyType, typename ValueType>
void RBTree<KeyType, ValueType>::fix_delete(std::vector<treenode_t *> &v) {
  auto r_itr = v.rbegin();

  while (r_itr != v.rend()) {
    treenode_t *cur = NULL;
    treenode_t *par = NULL;
    treenode_t *grandpar = NULL;
    treenode_t *sibling = NULL;

    cur = *r_itr;
    r_itr++;
    
    if (r_itr != v.rbegin()) {
      par = *r_itr;
    } else {
      if (cur->color = red) {
        cur->color = black;
      }
      return;
    }
    r_itr++;

    if (r_itr != v.rbegin()) {
      grandpar = *r_itr;
    } 
    
    if (par->left == cur) {
      sibling = par->right;
    } else {
      sibling = par->left;
    }

    if (cur->IsExternal()) {
      if (grandpar != NULL) {
        grandpar->ReplaceChild(par, sibling);
        delete cur;
        delete par;
      } else {
          // ASSERT(root_ == par)
          root_-> = sibling;
          if (root_ != Null && root_->color != black) {
            root_color = black;
          }
          delete cur;
          delete par;
          return;
      }
    } else {
      if (sibling )
    }
  }
  
}

template <typename KeyType, typename ValueType>
void RBTree<KeyType, ValueType>::Remove(KeyType key) {
  std::vector<treenode_t *> v;

  treenode_t *curNode = root_;
  int black_count = 0;

  while (true) {

    if (curnode->IsExternal()) {

    } else {
      if (has_red_child_or_grandchild(curnode)) {
        v.clear();
        black_count = 0;
      } else {
        black_count++;
      }

      v.push_back(curnode);

      if (black_count == 3) {

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