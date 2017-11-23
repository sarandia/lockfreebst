/* based on Tarjan's Top-down RB Tree Algorithm */
#include <queue>

namespace lock_free_rbtree {

enum color_t {red, black};
template <typename KeyType, typename ValueType>
class TreeNode {
  
  friend RBTree;
  
  public:
  bool IsExternal();
  
  private:
  KeyType key;
  color_t color;
  TreeNode *left;
  TreeNode *right;
};

template <typename KeyType, typename ValueType>
class ExternalTreeNode : public TreeNode<KeyType, ValueType> {
  
  friend RBTree;
  
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
  
  TreeNode * Search(KeyType key);
  
  void Insert(KeyType key, ValueType value);
  
  void Remove(KeyType key);
  
  private:
  TreeNode *root_;
  
  void fix_insert(TreeNode *x, TreeNode *y);
  void rotateLeftChildLeft(TreeNode *parent);
  void rotateLeftChildRight(TreeNode *parent);
  void rotateRightChildLeft(TreeNode *parent);
  void rotateRightChildRight(TreeNode *parent);
};

template <typename KeyType, typename ValueType>
bool TreeNode::IsExternal() {
  return false;
}

template <typename KeyType, typename ValueType>
bool ExternalTreeNode::IsExternal() {
  return true;
}

template <typename KeyType, typename ValueType>
RBTree::RBTree() {
  root_ = NULL;
}

template <typename KeyType, typename ValueType>
RBTree::~RBTree() {
// TODO
}

template <typename KeyType, typename ValueType>
TreeNode *RBTree::Search(KeyType key) {
// TODO
}

template <typename KeyType, typename ValueType>
void RBTree::Insert(KeyType key, ValueType value) {
  // 1. make the top-down invariant true initially
  TreeNode *x = root_;
  if (x->color == red) x->color = black;
  if (x->left != nullptr && x->left->color == red \
      && x->right != nullptr && x->right->color == red) {
    x->left->color = black;
    x->right->color = black;
  }
  // 2. walking down from the current node
  TreeNode *y = x;
  TreeNode *prev;
  int successiveBlk = 0;
  while (!y->IsExternal()) {
    // check if node is black with 2 red chilren
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
      TreeNode *z = y;
      z->color = red;
      z->left->color = black;
      z->right->color = black;
      // fix color problems
      fix_insert(x, z);
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
    }

  } // while
  // node y is external; must perform bottom-up insertion
  if (y->key == key) return;
  TreeNode *new_int = new TreeNode();
  ExternalTreeNode *left_child = new ExternalTreeNode();
  ExternalTreeNode *right_child = new ExternalTreeNode();
  if (y->key < key) {

  }

}

template <typename KeyType, typename ValueType>
void fix_insert(TreeNode *x, TreeNode *y) {

}

template <typename KeyType, typename ValueType>
void RBTree::Remove(KeyType key) {
  std::queue<TreeNode *> q;

  q.push_back(q);


}

template <typename KeyType, typename ValueType>
void RBTree::rotateLeftChildLeft(TreeNode *parent) {
  TreeNode *temp = parent->left->right->left;
  TreeNode *newLeft = parent->left->right;
  parent->left->right->left =  parent->left;
  parent->left->right = temp;
  parent->left = newLeft;
}

template <typename KeyType, typename ValueType>
void RBTree::rotateLeftChildRight(TreeNode *parent) {
  TreeNode *newLeft = parent->left->left;
  TreeNode *temp = parent->left->left->right;
  parent->left->left->right = parent->left;
  parent->left->left = temp;
  parent->left = newLeft;
}

template <typename KeyType, typename ValueType>
void RBTree::rotateRightChildLeft(TreeNode *parent) {
  TreeNode *newRight = parent->right->right;
  TreeNode *temp = parent->right->right->left;
  parent->right->right->left = parent->right;
  parent->right->right = temp;
  parent->right = newRight;
}

template <typename KeyType, typename ValueType>
void RBTree::rotateRightChildRight(TreeNode *parent) {
  TreeNode *newRight = parent->right->left;
  TreeNode *temp = parent->right->left->right;
  parent->right->left->right = parent->right;
  parent->right->left = temp;
  parent->right = newRight;
}

}