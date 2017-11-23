/* based on Tarjan's Top-down RB Tree Algorithm */

enum color_t {red, black};
template <typename KeyType, typename ValueType>
class TreeNode {
  
  friend RBTree;
  
  public:
  bool IsExternal();
  
  private:
  KeyType key;
  
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
// TODO
}

template <typename KeyType, typename ValueType>
void RBTree::Remove(KeyType key) {
  // TODO
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
