/* based on Tarjan's Top-down RB Tree Algorithm */

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
      TreeNode *new_int = new TreeNode();
      new_int->
    }
    prev = y;
    if (key < y->key) {
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

  }

}

template <typename KeyType, typename ValueType>
void RBTree::Remove(KeyType key) {
  // TODO
}

template <typename KeyType, typename ValueType>
void rotateLeftChildLeft(TreeNode *parent);
void rotateLeftChildRight(TreeNode *parent);
void rotateRightChildLeft(TreeNode *parent);
void rotateRightChildRight(TreeNode *parent);
