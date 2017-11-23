/* based on Tarjan's Top-down RB Tree Algorithm */
#include <queue>

namespace lock_free_rbtree {

enum color_t {red, black};

template <typename KeyType, typename ValueType>
class RBTree;

template <typename KeyType, typename ValueType>
class TreeNode {
  
  friend RBTree<KeyType, ValueType>;
  
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
  
  friend RBTree<KeyType, ValueType>;
  
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

  treenode_t * Search(KeyType key);
  
  void Insert(KeyType key, ValueType value);
  
  void Remove(KeyType key);
  
  private:
  treenode_t *root_;
  
  void rotateLeftChildLeft(treenode_t *parent);
  void rotateLeftChildRight(treenode_t *parent);
  void rotateRightChildLeft(treenode_t *parent);
  void rotateRightChildRight(treenode_t *parent);
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
// TODO
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
void RBTree<KeyType, ValueType>::Remove(KeyType key) {
  std::queue<TreeNode *> q;

  q.push_back(q);


}

template <typename KeyType, typename ValueType>
void RBTree<KeyType, ValueType>::rotateLeftChildLeft(treenode_t *parent) {
  TreeNode *temp = parent->left->right->left;
  TreeNode *newLeft = parent->left->right;
  parent->left->right->left =  parent->left;
  parent->left->right = temp;
  parent->left = newLeft;
}

template <typename KeyType, typename ValueType>
void RBTree<KeyType, ValueType>::rotateLeftChildRight(treenode_t *parent) {
  TreeNode *newLeft = parent->left->left;
  TreeNode *temp = parent->left->left->right;
  parent->left->left->right = parent->left;
  parent->left->left = temp;
  parent->left = newLeft;
}

template <typename KeyType, typename ValueType>
void RBTree<KeyType, ValueType>::rotateRightChildLeft(treenode_t *parent) {
  TreeNode *newRight = parent->right->right;
  TreeNode *temp = parent->right->right->left;
  parent->right->right->left = parent->right;
  parent->right->right = temp;
  parent->right = newRight;
}

template <typename KeyType, typename ValueType>
void RBTree<KeyType, ValueType>::rotateRightChildRight(treenode_t *parent) {
  TreeNode *newRight = parent->right->left;
  TreeNode *temp = parent->right->left->right;
  parent->right->left->right = parent->right;
  parent->right->left = temp;
  parent->right = newRight;
}

}