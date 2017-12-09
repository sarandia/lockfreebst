#ifndef TREE_H
#define TREE_H

/* based on Tarjan's Top-down RB Tree Algorithm */
#include <vector>
#include <iterator>
#include <string>
#include <iostream>
#include <sstream>
#include <atomic>

#define FREE 0
#define OWNED 1

namespace lock_free_rbtree {

enum color_t {red, black};

enum op_t {INSERT, DELETE, NOP};

template <typename KeyType, typename ValueType>
class RBTree;

template <typename KeyType, typename ValueType>
class TreeNode;

template <typename KeyType, typename ValueType>
class DataNode;

template <typename KeyType, typename ValueType>
struct Operation {
  op_t operation;
  KeyType key;
  ValueType value;
};

template <typename KeyType, typename ValueType>
class TreeNode {
  
  friend class RBTree<KeyType, ValueType>;
  
  public:
  typedef Operation<KeyType, ValueType> operation_t;
  
  TreeNode(bool ext) {
    data = new DataNode<KeyType, ValueType>(ext);
  }

  // copy constructor
  TreeNode(TreeNode<KeyType, ValueType> *n) {
    data = new DataNode<KeyType, ValueType>(n->data);
  }

  TreeNode(DataNode<KeyType, ValueType> *n) {
    data = n;
  }

  bool IsExternal();
  bool IsBlack();
  bool IsComplete();

  void SetExternal(bool new_isExt);
  KeyType GetKey();
  ValueType GetValue();
  void SetKey(KeyType new_key);
  void SetValue(ValueType new_value);
  color_t GetColor();
  void SetColor(color_t new_color);
  operation_t *GetOp();
  void SetOp(operation_t *op);
  TreeNode<KeyType, ValueType> * GetLeft();
  TreeNode<KeyType, ValueType> * GetRight();
  void SetLeft(TreeNode<KeyType, ValueType> *new_left);
  void SetRight(TreeNode<KeyType, ValueType> *new_right);
  DataNode<KeyType, ValueType> *Takeover(op_t op, KeyType key, ValueType value, bool need_own);
  DataNode<KeyType, ValueType> *Help();

  DataNode<KeyType, ValueType> *GetData() {
    DataNode<KeyType, ValueType> *data_ptr = data;
    return data_ptr;
  }
  void SetData(DataNode<KeyType, ValueType> *new_data) {
    data = new_data;
  }

  int GetOwn() {
    return ((DataNode<KeyType, ValueType> *) data)->own;
  }

  void SetOwn(int new_own) {
    ((DataNode<KeyType, ValueType> *) data)->own = (int) new_own;
  }

  bool ReplaceChild(TreeNode<KeyType, ValueType> *oldchld, TreeNode<KeyType, ValueType> *newchld) {
    if (((DataNode<KeyType, ValueType> *) data)->left == oldchld) {
      ((DataNode<KeyType, ValueType> *) data)->left = newchld;
      return true;
    }
    if (((DataNode<KeyType, ValueType> *) data)->right == oldchld) {
      ((DataNode<KeyType, ValueType> *) data)->right = newchld;
      return true;
    }
    return false;
  }

  DataNode<KeyType, ValueType> * acquireOwnership(op_t op, KeyType key, ValueType value, \
    DataNode<KeyType, ValueType> **new_data);
  bool releaseOwnership(DataNode<KeyType, ValueType> *old_data);

  private:
    std::atomic<DataNode<KeyType, ValueType> *> data;
    bool swap_data(DataNode<KeyType, ValueType> *new_data, DataNode<KeyType, ValueType> *old_data);
    bool swap_window(TreeNode<KeyType, ValueType> *rbt, DataNode<KeyType, ValueType> *old_data);
    void help(TreeNode<KeyType, ValueType> *oldPointer, DataNode<KeyType, ValueType> *oldData, TreeNode<KeyType, ValueType> *newPointer);
};

template <typename KeyType, typename ValueType>
class DataNode {
  friend class TreeNode<KeyType, ValueType>;

  public:
    typedef Operation<KeyType, ValueType> operation_t;

    DataNode(bool ext) {
      isExternal_ = ext;
      left = NULL;
      right = NULL;
      value = -1;
      if (ext) {
        color = black;
      }
      own = FREE;
    }

    DataNode(DataNode<KeyType, ValueType> *n) {
      key = n->key;
      isExternal_ = n->isExternal_;
      left = n->left;
      right = n->right;
      value = n->value;
      color = n->color;
      own = (int) n->own;

      if (n->op != NULL) {
        op = new operation_t;
        op->key = n->op->key;
        op->value = n->op->value;
        op->operation = n->op->operation;
      } else {
        op = NULL;
      }
    }

    ~DataNode() {
      if (op != NULL) {
        delete op;
      }
    }

    std::string ToString() {
      std::stringstream ss;
      ss << key;

      if (isExternal_) {
        ss << "*";
      } else {
        if (color == red) {
          ss << "R";
        } else {
          ss << "B";
        }
      }
      if (own == FREE) {
        ss << "F";
      } else {
        ss <<"O";
      }

      return ss.str();
    }

  private:
    KeyType key;
    color_t color;
    TreeNode<KeyType, ValueType> *left;
    TreeNode<KeyType, ValueType> *right;
    bool isExternal_;
    ValueType value;
    std::atomic<int> own;
    operation_t *op = NULL;
};

template <typename KeyType, typename ValueType>
class RBTree {
  
  public:
      
  typedef TreeNode<KeyType, ValueType> treenode_t;
  typedef Operation<KeyType, ValueType> operation_t;

  RBTree();
  RBTree(treenode_t *root, bool subTree);
  ~RBTree();

  
  treenode_t * Search(KeyType key);
  
  void Insert(KeyType key, ValueType value);
  
  void Remove(KeyType key);

  void buildVector(treenode_t *root, int depth, std::vector<std::vector<treenode_t *> > &ret);
  void print_tree();
  bool checkCompleteness();
  treenode_t *GetRoot() {return root_;}
  bool checkBlackDepth();
  
  private:
  treenode_t *root_;
  void fix_insert(std::vector<treenode_t*> q);
  bool has_red_child_or_grandchild(treenode_t *cur);
  bool has_red_child(treenode_t *cur);
  void fix_delete(std::vector<treenode_t *> &v);
  void swapNodes(treenode_t *node1, treenode_t *node2);
  void rotateLeft(treenode_t *parent);
  void rotateRight(treenode_t *parent);
  void delete_tree(treenode_t *cur);
  int computeBlackDepth(treenode_t *curNode);
  bool isSubTree;

  // parallel algorithm helpers
  TreeNode<KeyType, ValueType> *fix_window_color(std::vector<treenode_t *> &v, int insert_or_delete);
  void lock_unlock_window(std::vector<treenode_t *> v, int op);
  void lock_unlock_subtree(treenode_t *n, int depth, int op);
  TreeNode<KeyType, ValueType> *copy_window(std::vector<treenode_t *> &v, std::vector<treenode_t *> &new_acc_path);
  TreeNode<KeyType, ValueType> *clone_subtree(treenode_t *n, int depth);
};

template <typename KeyType, typename ValueType>
bool TreeNode<KeyType, ValueType>::IsExternal() {
  return ((DataNode<KeyType, ValueType> *) data)->isExternal_;
}

template <typename KeyType, typename ValueType>
bool TreeNode<KeyType, ValueType>::IsBlack() {
  return ((DataNode<KeyType, ValueType> *) data)->color == black;
}

template <typename KeyType, typename ValueType>
bool TreeNode<KeyType, ValueType>::IsComplete() {
  if (((DataNode<KeyType, ValueType> *) data)->left == NULL && \
    ((DataNode<KeyType, ValueType> *) data)->right == NULL) {
    return true;
  }

  if (((DataNode<KeyType, ValueType> *) data)->left != NULL && \
    ((DataNode<KeyType, ValueType> *) data)->right != NULL) {
    return ((DataNode<KeyType, ValueType> *) data)->left->IsComplete() \
    && ((DataNode<KeyType, ValueType> *) data)->right->IsComplete();
  }

  return false;
}

template <typename KeyType, typename ValueType>
KeyType TreeNode<KeyType, ValueType>::GetKey() {
  return ((DataNode<KeyType, ValueType> *) data)->key;
};


template <typename KeyType, typename ValueType>
ValueType TreeNode<KeyType, ValueType>::GetValue() {
  return ((DataNode<KeyType, ValueType> *) data)->value;
};


template <typename KeyType, typename ValueType>
TreeNode<KeyType, ValueType> * TreeNode<KeyType, ValueType>::GetLeft() {
  return ((DataNode<KeyType, ValueType> *) data)->left;
};

template <typename KeyType, typename ValueType>
TreeNode<KeyType, ValueType> * TreeNode<KeyType, ValueType>::GetRight() {
  return ((DataNode<KeyType, ValueType> *) data)->right;
};

template <typename KeyType, typename ValueType>
void TreeNode<KeyType, ValueType>::SetExternal(bool new_isExt) {
  ((DataNode<KeyType, ValueType> *) data)->isExternal_ = new_isExt;
}

template <typename KeyType, typename ValueType>
void TreeNode<KeyType, ValueType>::SetKey(KeyType new_key) {
  ((DataNode<KeyType, ValueType> *) data)->key = new_key;
}

template <typename KeyType, typename ValueType>
void TreeNode<KeyType, ValueType>::SetValue(ValueType new_value) {
  ((DataNode<KeyType, ValueType> *) data)->value = new_value;
}

template <typename KeyType, typename ValueType>
color_t TreeNode<KeyType, ValueType>::GetColor() {
  return ((DataNode<KeyType, ValueType> *) data)->color;
}

template <typename KeyType, typename ValueType>
void TreeNode<KeyType, ValueType>::SetColor(color_t new_color) {
  ((DataNode<KeyType, ValueType> *) data)->color = new_color;
}

template <typename KeyType, typename ValueType>
void TreeNode<KeyType, ValueType>::SetLeft(TreeNode<KeyType, ValueType> *new_left) {
  ((DataNode<KeyType, ValueType> *) data)->left = new_left;
}

template <typename KeyType, typename ValueType>
void TreeNode<KeyType, ValueType>::SetRight(TreeNode<KeyType, ValueType> *new_right) {
  ((DataNode<KeyType, ValueType> *) data)->right = new_right;
}

template <typename KeyType, typename ValueType>
Operation<KeyType, ValueType> *TreeNode<KeyType, ValueType>::GetOp() {
  return ((DataNode<KeyType, ValueType> *) data)->op;
}

template <typename KeyType, typename ValueType>
void TreeNode<KeyType, ValueType>::SetOp(operation_t *op) {
  ((DataNode<KeyType, ValueType> *) data)->op = op;
}

template <typename KeyType, typename ValueType>
DataNode<KeyType, ValueType> *TreeNode<KeyType, ValueType>::Takeover(op_t op, KeyType key, ValueType value, bool need_own) {
  //printf("stuck in Takeover()\n");
	DataNode<KeyType, ValueType> *old_data;

	if (need_own) {
		DataNode<KeyType, ValueType> *new_data;

    DataNode<KeyType, ValueType> *ret_data = acquireOwnership(op, key, value, &new_data);
    
		return ret_data;
	}
	else {
    old_data = this->GetData();
		while (old_data->own != FREE) {
      //printf("stuck in Takeover()->while loop\n");
      //DataNode<KeyType, ValueType> *helped_datanode = this->Help();
      //helped_datanode->own = FREE;
      /*if (helped_datanode != NULL) {
        bool help_cas = this->data.compare_exchange_strong(old_data, helped_datanode);
        //printf("help_cas = %d\n", help_cas);
        if (help_cas) {
          break;
        }
        old_data = this->GetData();
      }
      else {
        old_data = this->data;
      }*/

      auto newPointer = new TreeNode<KeyType, ValueType>(new DataNode<KeyType, ValueType>(old_data));
      
      //this->help(this, old_data, newPointer);
      
      old_data = this->GetData();
    }
          
		return NULL;
	}
}

template <typename KeyType, typename ValueType>
bool TreeNode<KeyType, ValueType>::releaseOwnership(DataNode<KeyType, ValueType> *old_data) {
  this->SetOwn(FREE);
  //this->GetData()->op = NULL; //garbage collection?
  return true;
}

template <typename KeyType, typename ValueType>
DataNode<KeyType, ValueType> *TreeNode<KeyType, ValueType>::Help() {
  TreeNode<KeyType, ValueType> *new_this = new TreeNode<KeyType, ValueType>(this);
  if (new_this->GetOp() == NULL) return NULL;
  //printf("stuck in Help() recursively\n");
  op_t operation = new_this->GetOp()->operation;
  KeyType help_key = new_this->GetOp()->key;
  ValueType help_value = new_this->GetOp()->value;
  new_this->SetOwn(FREE);
  RBTree<KeyType, ValueType> *rbt = new RBTree<KeyType, ValueType>(new_this, true);
  if (operation == INSERT) {
    //printf("trying to help insertion\n");
    rbt->Insert(help_key, help_value);
  }
  else if (operation == DELETE) {
    rbt->Remove(help_key);
  }
  return rbt->GetRoot()->GetData();
}

template <typename KeyType, typename ValueType>
DataNode<KeyType, ValueType> * TreeNode<KeyType, ValueType>::acquireOwnership(op_t op, KeyType key, ValueType value,
  DataNode<KeyType, ValueType> **new_data_node) {
  DataNode<KeyType, ValueType> *old_data = this->data;
  DataNode<KeyType, ValueType> *npdata;
  TreeNode<KeyType, ValueType> *newPointer = NULL;

  bool isSuccess = false;

  while (true) {
    //printf("stuck in acquireOwnership(), key = %d, own = %d\n", key, (int) old_data->own);
    if (old_data->own == OWNED) {

      if (newPointer != NULL) {
        delete newPointer;
      }
      newPointer = new TreeNode<KeyType, ValueType>(new DataNode<KeyType, ValueType>(old_data));
        //std::cout << key << " " << 1 << std::endl;

      // perform help
      //DataNode<KeyType, ValueType> *helped_datanode = this->Help();
      /*if (helped_datanode != NULL) {
        bool help_cas = this->data.compare_exchange_strong(old_data, helped_datanode);
        printf("help_cas = %d\n", help_cas);
        old_data = helped_datanode;
      }
      else {
        old_data = this->data;
      }*/
      //old_data = this->data;

      //this->help(this, old_data, newPointer);
    }
    if (old_data->own != OWNED) {
      /*int temp = FREE;
      DataNode<KeyType, ValueType> *pdata = this->GetData();
      if (pdata->own.compare_exchange_strong(temp, OWNED)) {
        isSuccess = true;
        break;
      }*/
      npdata = new DataNode<KeyType, ValueType>(old_data);
      npdata->own = OWNED;
      npdata->op = new operation_t();
      npdata->op->key = key;
      npdata->op->value = value;
      npdata->op->operation = op;
      bool cas = this->data.compare_exchange_strong(old_data, npdata);
      if (cas) {
        break;
      }
      else {
        delete npdata;
      }
    }

    old_data = this->data;
  }

  *new_data_node = old_data;
  return npdata;
}

template <typename KeyType, typename ValueType>
void TreeNode<KeyType, ValueType>::help(TreeNode<KeyType, ValueType> *oldPointer, DataNode<KeyType, ValueType> *oldData, TreeNode<KeyType, ValueType> *newPointer) {
  auto *op = newPointer->GetOp();
  newPointer->SetOp(NULL);
  newPointer->SetOwn(FREE);
  if (op->operation == INSERT) {
    RBTree<KeyType, ValueType> ptree(newPointer, true);
    ptree.Insert(op->key, op->value);
  } else if (op->operation == DELETE) {
    RBTree<KeyType, ValueType> ptree(newPointer, true);
    ptree.Remove(op->key);
  }
  if (!oldPointer->data.compare_exchange_strong(oldData, newPointer->GetData())) {
    //std::cout << "failed" << std::endl;
  }
}

template <typename KeyType, typename ValueType>
RBTree<KeyType, ValueType>::RBTree() {
  root_ = NULL;
  isSubTree = false;
}

template <typename KeyType, typename ValueType>
RBTree<KeyType, ValueType>::RBTree(treenode_t *root, bool subTree) {
  root_ = root;
  isSubTree = subTree;
}

template <typename KeyType, typename ValueType>
RBTree<KeyType, ValueType>::~RBTree() {
  if (!isSubTree) {
    delete_tree(root_);
  }
}

template <typename KeyType, typename ValueType>
void RBTree<KeyType, ValueType>::delete_tree(treenode_t *cur) {
  if (cur == NULL) return;
  delete_tree(cur->GetLeft());
  delete_tree(cur->GetRight());
  delete cur;
}

template <typename KeyType, typename ValueType>
TreeNode<KeyType, ValueType> *RBTree<KeyType, ValueType>::Search(KeyType key) {
  treenode_t *cur = root_;
  while (cur != NULL) {
    if (!cur->IsExternal()) {
      if (key <= cur->GetKey()) {
        cur = cur->GetLeft();
      }
      else {
        cur = cur->GetRight();
      }
    }
    else {
      if (cur->GetKey() == key) {
        return cur;
      }
      return NULL;
    }
  }
  return NULL;
}

template <typename KeyType, typename ValueType>
void RBTree<KeyType, ValueType>::Insert(KeyType key, ValueType value) {
  //if (Search(key)) return;
  //printf("stuck in Insert() recursively\n");
  if (root_ == NULL) {
    root_ = new treenode_t(true);
    //printf("acquire root node\n");
    root_->Takeover(INSERT, key, value, true);
    //printf("root node acquired\n");
    root_->SetColor(black);
    root_->SetKey(key);
    root_->SetValue(value);
    //printf("release root node\n");
    root_->releaseOwnership(NULL);
    return;
  }
  // 1. make the top-down invariant true initially
  //if (!isSubTree) {
    DataNode<KeyType, ValueType> *old_data;
    treenode_t *x = root_;
    old_data = x->Takeover(INSERT, key, value, true);
    lock_unlock_subtree(root_->GetLeft(), 0, 0);
    lock_unlock_subtree(root_->GetRight(), 0, 0);
  //}

  if (x->GetColor() == red && !isSubTree) {
    x->SetColor(black);
  }
  // 2. walking down from the current node
  treenode_t *y = x;
  treenode_t *prev = NULL;
  std::vector<treenode_t *> q;
  int successiveBlk = 0;
  while (!y->IsExternal()) {
    // check if y is owned. if so, we perform Takeover()
    if (x != root_) {
      if (y != x) {
        y->Takeover(INSERT, key, value, false);
        lock_unlock_subtree(y->GetLeft(), 0, 0);
        lock_unlock_subtree(y->GetRight(), 0, 0);
      }
    }
    //printf("stuck in insert(), y->key = %d, y->own = %d\n", y->GetKey(), y->GetOwn());
    //printf("y->address = %p, isSubtree = %d\n", y, isSubTree);
    // check if node is black with 2 red chilren
    q.push_back(y);
    prev = y;
    if (y->GetColor() == black) {
      if (y->GetLeft()->GetColor() == red && y->GetRight()->GetColor() == red) {
        successiveBlk ++;
      }
      else {
        successiveBlk = 0;
      }
    }
    else {
      if (prev->GetColor() == red) {
        successiveBlk = 0;
      }
    }
    // c. color z red and its two children black, then proceed as bottom-up
    if (successiveBlk == 4) {
      treenode_t *z = y;
      z->SetColor(red);
      z->GetLeft()->SetColor(black);
      z->GetRight()->SetColor(black);
      // fix color problems
      //DataNode<KeyType, ValueType> *old_data = q[0]->data;
      x->swap_window(fix_window_color(q,0), old_data);
      // replace the current node x by the child of z along the access path
      treenode_t *old_x = x;
      if (key < z->GetKey()) {
        x = z->GetLeft();
        y = x;
      }
      else {
        x = z->GetRight();
        y = x;
      }
      //if (!isSubTree) {
        old_data = x->Takeover(INSERT, key, value, true);
        lock_unlock_subtree(x->GetLeft(), 0, 0);
        lock_unlock_subtree(x->GetRight(), 0, 0);
      //}
      old_x->releaseOwnership(NULL);
      successiveBlk = 0;
      q.clear();
      // repeat the general step
      continue;
    }
    if (key <= y->GetKey()) {
      y = y->GetLeft();
    }
    else if (key > y->GetKey()) {
      y = y->GetRight();
    }
    // b. a black node with a black child is reached
    if (y->GetColor() == black && !y->IsExternal()) {
      if (y->GetLeft()->GetColor() == black || y->GetRight()->GetColor() == black) {
        // replace current window root x by y
        //if (!isSubTree) {
          old_data = y->Takeover(INSERT, key, value, true);
          lock_unlock_subtree(y->GetLeft(), 0, 0);
          lock_unlock_subtree(y->GetRight(), 0, 0);
        //}
        x->releaseOwnership(NULL);
        x = y;
        q.clear();
      }
    }

  } // while
  // node y is external; must perform bottom-up insertion
  //std::cout << "x = " << x->key << std::endl;
  //if (!q.empty()) std::cout << "q[0] = " << q[0]->key << std::endl;
  if (y->GetKey() == key) {
    x->releaseOwnership(NULL);
    return;
  }
  treenode_t *new_int = new treenode_t(false);
  //if (!isSubTree) {
    new_int->Takeover(INSERT, key, value, false);
    lock_unlock_subtree(new_int->GetLeft(), 0, 0);
    lock_unlock_subtree(new_int->GetRight(), 0, 0);
  //}
  treenode_t *new_item = new treenode_t(true);
  //if (!isSubTree) {
    new_item->Takeover(INSERT, key, value, false);
    lock_unlock_subtree(new_item->GetLeft(), 0, 0);
    lock_unlock_subtree(new_item->GetRight(), 0, 0);
  //}
  new_item->SetKey(key);
  new_item->SetValue(value);
  if (y->GetKey() <= key) {
    new_int->SetKey(y->GetKey());
    new_int->SetLeft(y);
    new_int->SetRight(new_item);
  }
  else {
    new_int->SetKey(key);
    new_int->SetLeft(new_item);
    new_int->SetRight(y);
  }
  new_int->SetColor(red);
  if (prev != NULL) {
    if (new_int->GetKey() <= prev->GetKey()) {
      prev->SetLeft(new_int);
    }
    else {
      prev->SetRight(new_int);
    }
  }
  else {
    root_ = new_int;
    if (!isSubTree) root_->SetColor(black);
  }
  if (q.empty()) {
    q.push_back(y);
  }
  if (q.size() == 1 && q.back()->IsExternal()) {
    q.clear(); // pop off y
  }
  q.push_back(new_int);
  new_int->releaseOwnership(NULL);
  new_item->releaseOwnership(NULL);

  // for debugging
  /*std::cout << "Access Path: ";
  for (auto node:q) {
    std::cout << node->GetKey() << ",";
  }
  std::cout << std::endl;
  std::vector<treenode_t *> v;*/

  //fix_insert(q);
  //DataNode<KeyType, ValueType> *old_data = q[0]->GetData();
  //old_data = q[0]->GetData();
  bool cas = q[0]->swap_window(fix_window_color(q, 0), old_data);
  //printf("cas = %d\n", cas);
  x->releaseOwnership(NULL);
  /*for (auto node: q) {
    lock_unlock_subtree(node->GetLeft(), 0, 1);
    lock_unlock_subtree(node->GetRight(), 0, 1);
    node->releaseOwnership(NULL);
  }*/
  //printf("q[0]->own = %d\n", q[0]->GetOwn());
}

template <typename KeyType, typename ValueType>
void RBTree<KeyType, ValueType>::fix_insert(std::vector<treenode_t *> q) {
  if (q.size() == 1) return;
  //std::vector<treenode_t *> q_copy = q;
  //lock_unlock_window(q, 0);
  bool is_violation = true;
  while (is_violation) {
    is_violation = false;
    treenode_t *cur = q.back();
    if (!isSubTree) {
      if (cur != q[0]) cur->Takeover(INSERT, static_cast<KeyType>(NULL), static_cast<ValueType>(NULL), false);
    }
    if (cur->GetColor() == red) {
      treenode_t *parent = q[q.size()-2];
      if (!isSubTree) {
        if (parent != q[0]) parent->Takeover(INSERT, static_cast<KeyType>(NULL), static_cast<ValueType>(NULL), false);
      }
      treenode_t *sibling;
      if (parent == q[0]) break;
      if (parent->GetColor() == red) { //&& sibling->color == red) {
        is_violation = true;
        treenode_t *grandparent = q[q.size()-3];
        if (!isSubTree) {
          if (grandparent != q[0]) grandparent->Takeover(INSERT, static_cast<KeyType>(NULL), static_cast<ValueType>(NULL), false);
        }
        if (grandparent == q[0]) {
          q.pop_back();
          break;
        }
        if (parent->GetKey() <= grandparent->GetKey()) {
          sibling = grandparent->GetRight();
          if (!isSubTree) {
            if (sibling != q[0]) sibling->Takeover(INSERT, static_cast<KeyType>(NULL), static_cast<ValueType>(NULL), false);
          }
        }
        else {
          sibling = grandparent->GetLeft();
          if (!isSubTree) {
            if (sibling != q[0]) sibling->Takeover(INSERT, static_cast<KeyType>(NULL), static_cast<ValueType>(NULL), false);
          }
        }
        if (sibling->GetColor() == red) {
          grandparent->SetColor(red);
          parent->SetColor(black);
          sibling->SetColor(black);
          // we then go up by 1. The current node is now the parent
          q.pop_back();
        }
        if (q.size() == 2) {
          break;
        }
        q.pop_back();
      }
    }
  }

  // if reached the root of the tree, fix the last color violation
  if (q.size() <=2) {
    treenode_t *cur = q[1];
    //if (cur != q[0]) cur->Takeover(INSERT, static_cast<KeyType>(NULL), static_cast<ValueType>(NULL), false);
    treenode_t *window_root = q[0];
    treenode_t *sibling;
    int direction = 0;
    if (window_root->GetLeft() == q[1]) {
      sibling = window_root->GetRight();
      if (!isSubTree) {
        if (sibling != q[0]) sibling->Takeover(INSERT, static_cast<KeyType>(NULL), static_cast<ValueType>(NULL), false);
      }
    }
    else {
      sibling = window_root->GetLeft();
      if (!isSubTree) {
        if (sibling != q[0]) sibling->Takeover(INSERT, static_cast<KeyType>(NULL), static_cast<ValueType>(NULL), false);
      }
      direction = 1;
    }
    // 3-c, both window_root and cur are red, and all children of cur are black
    // also, the sibling needs to be black
    if (window_root->GetColor() == red && sibling->GetColor() == black && cur->GetColor() == red) {
      if (((cur->GetLeft() != NULL && cur->GetLeft()->GetColor() == black) || cur->GetLeft() == NULL) && \
        ((cur->GetRight() != NULL && cur->GetRight()->GetColor() == black) || cur->GetRight() == NULL)) {
        // color the node black
        window_root->SetColor(black);
      }
    }
    else if (window_root->GetColor() == black && cur->GetColor() == red && \
      sibling->GetColor() == red && (cur->GetLeft()->GetColor() == red || cur->GetRight()->GetColor() == red)) {
      window_root->SetColor(black);
      cur->SetColor(black);
      sibling->SetColor(black);
    }
    // 3-d and e, cur is red and has a red child
    else if (window_root->GetColor() == black && cur->GetColor() == red && \
      sibling->GetColor() == black && (cur->GetLeft()->GetColor() == red \
      || cur->GetRight()->GetColor() == red)) {
      // re-color the tree, then rotate
      cur->SetColor(black);
      if (direction == 0) {
        // 3e, left case
        if (cur->GetRight()->GetColor() == red) {
          rotateLeft(cur);
        }
        rotateRight(window_root);
        //std::cout << "rotating dir = 0 on key: " << window_root->key << " color: " << window_root->color << std::endl;
        window_root->SetColor(black);
        window_root->GetRight()->SetColor(red);
        window_root->GetRight()->GetRight()->SetColor(black);
        window_root->GetLeft()->SetColor(red);
        window_root->GetLeft()->GetLeft()->SetColor(black);
        window_root->GetLeft()->GetRight()->SetColor(black);
      }
      else {
        // 3e, right case
        if (cur->GetLeft()->GetColor() == red) {
          rotateRight(cur);
        }
        rotateLeft(window_root);
        //std::cout << "rotating dir = 1 on key: " << window_root->key << " color: " << window_root->color << std::endl;
        window_root->SetColor(black);
        window_root->GetLeft()->SetColor(red);
        window_root->GetLeft()->GetLeft()->SetColor(black);
        window_root->GetRight()->SetColor(red);
        window_root->GetRight()->GetLeft()->SetColor(black);
        window_root->GetRight()->GetRight()->SetColor(black);
      }
    }
  }
  //lock_unlock_window(q_copy, 1);

}


template <typename KeyType, typename ValueType>
bool RBTree<KeyType, ValueType>::has_red_child_or_grandchild(treenode_t *cur) {
  if (cur->GetLeft() != NULL) {
    cur->GetLeft()->Takeover(DELETE, static_cast<KeyType>(NULL), static_cast<ValueType>(NULL), false);
    if (cur->GetLeft()->GetColor() == red) {
      return true;
    }
    if (has_red_child(cur->GetLeft())) {
      return true;
    }
  }
  if (cur->GetRight() != NULL) {
    cur->GetRight()->Takeover(DELETE, static_cast<KeyType>(NULL), static_cast<ValueType>(NULL), false);
    if (cur->GetRight()->GetColor() == red) {
      return true;
    }
    if (has_red_child(cur->GetRight())) {
      return true;
    }
  }

  return false;
}

template <typename KeyType, typename ValueType>
bool RBTree<KeyType, ValueType>::has_red_child(treenode_t *cur) {
  if (cur->GetLeft() != NULL) {
    cur->GetLeft()->Takeover(DELETE, static_cast<KeyType>(NULL), static_cast<ValueType>(NULL), false);
  }
  if (cur->GetLeft() != NULL && !cur->GetLeft()->IsExternal() && cur->GetLeft()->GetColor() == red) {
    return true;
  }
  if (cur->GetRight() != NULL) {
    cur->GetRight()->Takeover(DELETE, static_cast<KeyType>(NULL), static_cast<ValueType>(NULL), false);
  }
  if (cur->GetRight() != NULL && !cur->GetRight()->IsExternal() && cur->GetRight()->GetColor() == red) {
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

	cur = *r_itr;

	r_itr++;
	if (r_itr != v.rend()) {
		par = *r_itr;
	}
	else {
		root_ = NULL;
		delete cur;
		return;
	}

	if (par->GetLeft() == cur) {
		sibling = par->GetRight();
	}
	else {
		sibling = par->GetLeft();
	}

	if (cur->IsExternal()) {

		swapNodes(par, sibling);

		if (sibling->GetColor() == red) {
			delete cur;
			delete sibling;
			return;
		}

		delete cur;
		delete sibling;

		cur = par;

		r_itr++;
		if (r_itr != v.rend()) {
			par = *r_itr;
		}
		else {
			if (!cur->IsExternal()) {
				if (cur->GetColor() == red) {
					cur->SetColor(black);
				}
			}
			return;
		}

		if (par->GetLeft() == cur) {
			sibling = par->GetRight();
		}
		else {
			sibling = par->GetLeft();
		}
	}

	if (cur->GetColor() == red) {
		cur->SetColor(black);
		return;
	}

	while (true) {

		if (par->GetColor() == black && sibling->GetColor() == black && !has_red_child(sibling)) {

			sibling->SetColor(red);

			cur = par;

			if (cur->GetColor() == red) {
				cur->SetColor(black);
				return;
			}

			r_itr++;
			if (r_itr != v.rend()) {
				par = *r_itr;
			}
			else {
				if (!cur->IsExternal()) {
					if (cur->GetColor() == red) {
						cur->SetColor(black);
					}
				}
				return;
			}

			if (par->GetLeft() == cur) {
				sibling = par->GetRight();
			}
			else {
				sibling = par->GetLeft();
			}
		}
		else {
			break;
		}
	}

	if (cur->GetColor() == black && par->GetColor() == black && sibling->GetColor() == red) {
		par->SetColor(red);
		sibling->SetColor(black);
		if (sibling == par->GetRight()) {
			rotateLeft(par);

			par = par->GetLeft();

			cur = par->GetLeft();
			sibling = par->GetRight();
		}
		else {
			rotateRight(par);

			par = par->GetRight();

			cur = par->GetRight();
			sibling = par->GetLeft();
		}
	}

	if (par->GetColor() == red && sibling->GetColor() == black && !has_red_child(sibling)) {
		par->SetColor(black);
		sibling->SetColor(red);
		return;
	}

	if (sibling == par->GetRight()) {
		if (sibling->GetColor() == black && sibling->GetRight()->GetColor() == red) {
			sibling->SetColor(par->GetColor());
			sibling->GetRight()->SetColor(black);
			par->SetColor(black);
			rotateLeft(par);
			return;
		}
		if (sibling->GetColor() == black && sibling->GetLeft()->GetColor() == red) {
			sibling->GetLeft()->SetColor(par->GetColor());
			par->SetColor(black);
			rotateRight(sibling);
			rotateLeft(par);
			return;
		}
	}
	else {
		if (sibling->GetColor() == black && sibling->GetLeft()->GetColor() == red) {
			sibling->SetColor(par->GetColor());
			sibling->GetLeft()->SetColor(black);
			par->SetColor(black);
			rotateRight(par);
			return;
		}
		if (sibling->GetColor() == black && sibling->GetRight()->GetColor() == red) {
			sibling->GetRight()->SetColor(par->GetColor());
			par->SetColor(black);
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
  //treenode_t *recordedWinRoot = NULL;
  DataNode<KeyType, ValueType> *old_data = NULL;
  int black_count = 0;
  
  old_data = curNode->Takeover(DELETE, key, static_cast<ValueType>(NULL), true);

	while (true) {

		if (curNode->IsExternal()) {
      /*if (v.empty()) {
        recordedWinRoot = curNode;
      }*/
			v.push_back(curNode);
			//fix_delete(v);
      /*std::cout << "Access Path: ";
      for (auto node:v) {
        std::cout << node->GetKey() << ",";
      }
      std::cout << std::endl;
      std::vector<treenode_t *> v2;
      TreeNode<KeyType, ValueType> *rbt = copy_window(v, v2);
      RBTree<KeyType, ValueType> *rbt2 = new RBTree<KeyType, ValueType>();
      rbt2->root_ = rbt;
      rbt2->print_tree();
      std::cout << "Copied Access Path: ";
      for (auto node: v2) {
        std::cout << node->GetKey() << ",";
      } 
      std::cout << std::endl;*/

      DataNode<KeyType, ValueType> *old_data = v[0]->data;

      auto oldRoot = v[0];
      oldRoot->swap_window(fix_window_color(v, 1), old_data);

      //recordedWinRoot->releaseOwnership(old_data);
			return;
		}
		else {

			if (black_count == 2) {
				treenode_t *par = *(v.rbegin());

				par->GetRight()->SetColor(red);
				par->GetLeft()->SetColor(red);

        auto oldRoot = v[0];
        oldRoot->swap_window(fix_window_color(v, 1), old_data);
        
				//fix_delete(v);
    
        par = *(v.rbegin());

        v.clear();
        old_data = par->Takeover(DELETE, key, static_cast<ValueType>(NULL), true);
        v.push_back(par);
        
        //oldRoot->releaseOwnership(old_data);
        //recordedWinRoot->releaseOwnership(old_data);
        //recordedWinRoot = par;

				black_count = 0;
			}

			if (curNode->GetColor() != black || has_red_child_or_grandchild(curNode)) {
				if (v.size() > 1) {
          treenode_t *tempNode = NULL;
          tempNode = v.back();

          auto oldRoot = v[0];
          v.clear();
          old_data = tempNode->Takeover(DELETE, key, static_cast<ValueType>(NULL), true);
          v.push_back(tempNode);
          
          // Release
          oldRoot->releaseOwnership(old_data);
          //recordedWinRoot->releaseOwnership(v[0]->data);
          //recordedWinRoot = tempNode;
				}

				black_count = 0;
			}
			else {
				black_count++;
      }
      
      /*if (v.empty()) {
        recordedWinRoot = curNode;
      } */
			v.push_back(curNode);

			if (key <= curNode->GetKey()) {
				curNode = curNode->GetLeft();
			}
			else {
				curNode = curNode->GetRight();
      }
      
      curNode->Takeover(DELETE, key, static_cast<ValueType>(NULL), false);
		}
	}
}

template <typename KeyType, typename ValueType>
void RBTree<KeyType, ValueType>::buildVector(treenode_t *root, int depth, \
  std::vector<std::vector<treenode_t *> > &ret) {
    if(root == NULL) return;
    if(ret.size() == depth)
        ret.push_back(std::vector<treenode_t *>());
    
    ret[depth].push_back(root);
    if (root->GetLeft() != NULL) buildVector(root->GetLeft(), depth + 1, ret);
    if (root->GetRight() != NULL) buildVector(root->GetRight(), depth + 1, ret);
}

template <typename KeyType, typename ValueType>
void RBTree<KeyType, ValueType>::print_tree() {
  std::vector<std::vector<treenode_t *> > ret;
  buildVector(root_, 0, ret);
  int n_nodes = 0;
  for (int level = 0; level < ret.size(); level++) {
    for (int node_i = 0; node_i < ret[level].size(); node_i++) {
      treenode_t *cur = ret[level][node_i];
      n_nodes ++;
      std::string cl;
      if (cur->GetColor() == black) {
        cl = "black";
      }
      else {
        cl = "red";
      }
      //printf("(%d, %d, %s, %d). ", cur->GetKey(), cur->GetValue(), cl.c_str(), cur->IsExternal());
    }
    //printf("\n");
  }
  printf("Depth = %lu\n", ret.size());
  printf("Number of Nodes = %d\n", n_nodes);
}

template <typename KeyType, typename ValueType>
bool RBTree<KeyType, ValueType>::checkBlackDepth() {
    return (computeBlackDepth(root_) != -1);
}

template <typename KeyType, typename ValueType>
int RBTree<KeyType, ValueType>::computeBlackDepth(treenode_t *curNode) {
    // For an empty subtree the answer is obvious
    if (curNode->IsExternal())
        return 1; 

    if (curNode->GetColor() == red) {
      if (curNode->GetLeft()->GetColor() == red || curNode->GetRight()->GetColor() == red) {
        printf("Double red found %d\n", curNode->GetKey());
        return -1; 
      }
    }
    // Computes the height for the left and right child recursively
    int leftHeight = computeBlackDepth(curNode->GetLeft());
    int rightHeight = computeBlackDepth(curNode->GetRight());
    int add = (curNode->GetColor() == black) ? 1 : 0;
    // The current subtree is not a red black tree if and only if
    // one or more of current node's children is a root of an invalid tree
    // or they contain different number of black nodes on a path to a null node.
    if (leftHeight == -1 || rightHeight == -1 || leftHeight != rightHeight) {
        printf("The subtree rooted at key = %d is not a valid RB tree.\n", curNode->GetKey());
        return -1; 
    }
    else {
        return leftHeight + add;
    }
}

template <typename KeyType, typename ValueType>
bool RBTree<KeyType, ValueType>::checkCompleteness() {
  if (root_ == NULL) {
    return true;
  }

  if (root_ != NULL && root_->GetLeft() == NULL && root_->GetRight() == NULL) {
    return true;
  }

  return root_->IsComplete();
};


template <typename KeyType, typename ValueType>
void RBTree<KeyType, ValueType>::swapNodes(treenode_t *node1, treenode_t *node2) {
  KeyType tmpKey = node1->GetKey();
  color_t tmpColor = node1->GetColor();
  treenode_t *tmpLeft = node1->GetLeft();
  treenode_t *tmpRight = node1->GetRight();
  bool tmpIsExternal_ = node1->IsExternal();
  ValueType tmpValue = node1->GetValue();
  //auto tmpOwn = node1->GetData();
  operation_t *tmpOp = node1->GetOp();

  node1->SetKey(node2->GetKey());
  node1->SetColor(node2->GetColor());
  node1->SetLeft(node2->GetLeft());
  node1->SetRight(node2->GetRight());
  node1->SetExternal(node2->IsExternal());
  node1->SetValue(node2->GetValue());
  //node1->SetData(node2->GetData());
  node1->SetOp(node2->GetOp());

  node2->SetKey(tmpKey);
  node2->SetColor(tmpColor);
  node2->SetLeft(tmpLeft);
  node2->SetRight(tmpRight);
  node2->SetExternal(tmpIsExternal_);
  node2->SetValue(tmpValue);
  //node2->SetData(tmpOwn);
  node2->SetOp(tmpOp);
}

template <typename KeyType, typename ValueType>
void RBTree<KeyType, ValueType>::rotateLeft(treenode_t *parent) {

  treenode_t *tempRight = parent->GetRight();
  parent->SetRight(parent);
  swapNodes(parent, tempRight);

  parent = tempRight;
  
  treenode_t *newPar = parent->GetRight();
  treenode_t *temp = parent->GetRight()->GetLeft();
  parent->GetRight()->SetLeft(parent);
  parent->SetRight(temp);
}

template <typename KeyType, typename ValueType>
void RBTree<KeyType, ValueType>::rotateRight(treenode_t *parent) {

  treenode_t *tempLeft = parent->GetLeft();
  parent->SetLeft(parent);
  swapNodes(parent, tempLeft);

  parent = tempLeft;

  treenode_t *temp = parent->GetLeft()->GetRight();
  parent->GetLeft()->SetRight(parent);
  parent->SetLeft(temp);
}

template <typename KeyType, typename ValueType>
TreeNode<KeyType, ValueType> *RBTree<KeyType, ValueType>::fix_window_color(std::vector<treenode_t *> &v, int insert_or_delete) {
  std::vector<treenode_t *> new_acc_path;
  TreeNode<KeyType, ValueType> *w_copy = copy_window(v, new_acc_path);
  //treenode_t *window_root = w_copy->GetRoot();
  if (insert_or_delete == 0) {
    fix_insert(new_acc_path);
  }
  else {
    fix_delete(new_acc_path);
    v = new_acc_path;
  }
  // return the data node of the root TreeNode for later swapping
  return w_copy;
}

template <typename KeyType, typename ValueType>
void RBTree<KeyType, ValueType>::lock_unlock_window(std::vector<treenode_t *> v, int op) {
  printf("called lock_unlock_window()\n");
  for (int i = 0; i < v.size()-1; i++) {
    // takeover v
    treenode_t *cur = v[i];
    //if (op == 0) {
    //  cur->Takeover(NOP, static_cast<KeyType>(NULL), static_cast<ValueType>(NULL), true);
    //}
    //else {
    //  cur->releaseOwnership(NULL);
    //}
    // take over subtree up to 3 levels
    treenode_t *next = v[i+1];
    if (next == cur->GetLeft()) {
      lock_unlock_subtree(cur->GetRight(), op, 0);
    }
    else {
      lock_unlock_subtree(cur->GetLeft(), op, 0);
    }
  }
  //if (op == 0) {
  //  v.back()->Takeover(NOP, static_cast<KeyType>(NULL), static_cast<ValueType>(NULL), true);
  //}
  //else {
  //  v.back()->releaseOwnership(NULL);
  //}
  printf("lock_unlock_window() finished\n");
}

template <typename KeyType, typename ValueType>
void RBTree<KeyType, ValueType>::lock_unlock_subtree(treenode_t *n, int depth, int op) {
  if (n == NULL) return;
  if (op == 0) {
    n->Takeover(NOP, static_cast<KeyType>(NULL), static_cast<ValueType>(NULL), false);
  }
  else {
    n->releaseOwnership(NULL);
  }
  if (depth < 3) {
    lock_unlock_subtree(n->GetLeft(), depth+1, op);
    lock_unlock_subtree(n->GetRight(), depth+1, op);
  }
}

template <typename KeyType, typename ValueType>
TreeNode<KeyType, ValueType> *RBTree<KeyType, ValueType>::copy_window(std::vector<treenode_t *> &v,\
  std::vector<treenode_t *> &new_acc_path) {
  if (v.size() == 0) return NULL;
  // copy all nodes that are connected to the access path
  treenode_t *dup_w_root = new treenode_t(v[0]);
  dup_w_root->SetOwn(FREE);
  /*dup_w_root->SetOwn(FREE);
  if (dup_w_root->GetOp() != NULL) {
    delete dup_w_root->GetOp();
  }
  dup_w_root->SetOp(NULL);*/
  //dup_w_root->Takeover(NOP, static_cast<KeyType>(NULL), static_cast<ValueType>(NULL), false);
  new_acc_path.push_back(dup_w_root);
  treenode_t *prev_node = NULL;
  treenode_t *cur_w_node = dup_w_root;
  for (auto node: v) {
    // if node is window root
    if (prev_node == NULL) {
      prev_node = node;
      continue;
    }
    // if the access path goes left, copy the right subtree of prev
    if (prev_node->GetLeft() == node) {
      cur_w_node->SetRight(clone_subtree(prev_node->GetRight(), 0));
      node->Takeover(NOP, static_cast<KeyType>(NULL), static_cast<ValueType>(NULL), false);
      treenode_t *new_w_node = new treenode_t(node);
      cur_w_node->SetLeft(new_w_node);
      new_acc_path.push_back(new_w_node);
      cur_w_node = new_w_node;
    }
    else {
      cur_w_node->SetLeft(clone_subtree(prev_node->GetLeft(), 0));
      node->Takeover(NOP, static_cast<KeyType>(NULL), static_cast<ValueType>(NULL), false);
      treenode_t *new_w_node = new treenode_t(node);
      cur_w_node->SetRight(new_w_node);
      new_acc_path.push_back(new_w_node);
      cur_w_node = new_w_node;
    }
    prev_node = node;
  }
  //RBTree<KeyType, ValueType> *rbt = new RBTree<KeyType, ValueType>();
  //rbt->root_ = dup_w_root;
  return dup_w_root;
}

template <typename KeyType, typename ValueType>
TreeNode<KeyType, ValueType> *RBTree<KeyType, ValueType>::clone_subtree(treenode_t *n, int depth) {
  //std::cout << depth << std::endl;
  treenode_t *new_root;
  if (n == NULL) return NULL;
  n->Takeover(NOP, static_cast<KeyType>(NULL), static_cast<ValueType>(NULL), false);
  new_root = new treenode_t(n);
  new_root->SetOwn(FREE);
  if (depth < 3) {
    new_root->SetLeft(clone_subtree(n->GetLeft(), depth+1));
    new_root->SetRight(clone_subtree(n->GetRight(), depth+1));
  }
  return new_root;
}

template <typename KeyType, typename ValueType>
bool TreeNode<KeyType, ValueType>::swap_window(TreeNode<KeyType, ValueType> *rbt, \
  DataNode<KeyType, ValueType> *old_data) {
  // TODO: need compare_and_swap here
  //data = rbt->GetRoot()->data;
  bool res = data.compare_exchange_strong(old_data, rbt->GetData());
  //if (!res) {
  //  std::cout << "Swap window failed: old: " << old_data->ToString() << " new: " << rbt->GetData()->ToString() << std::endl;
  //  std::cout << "data = " << data << " old_data = " << old_data << std::endl;
    //std::cout << "target data: " << data->ToString() << std::endl;
  //}
  return res;
}

template <typename KeyType, typename ValueType>
bool TreeNode<KeyType, ValueType>::swap_data(DataNode<KeyType, ValueType> *new_data, \
  DataNode<KeyType, ValueType> *old_data) {
  // TODO: need compare_and_swap here
  //data = rbt->GetRoot()->data;
  bool res = data.compare_exchange_strong(old_data, new_data);
  if (!res) {
    std::cout << "Swap data failed: old: " << old_data->ToString() << " new: " << new_data->ToString() << std::endl;
  }
  return res;
}

}

#endif