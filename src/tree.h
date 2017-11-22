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
void rotateLeftChildLeft(TreeNode *parent);
void rotateLeftChildRight(TreeNode *parent);
void rotateRightChildLeft(TreeNode *parent);
void rotateRightChildRight(TreeNode *parent);
