#ifndef TREE_H
#define TREE_H

/* based on Tarjan's Top-down RB Tree Algorithm */
#include <vector>
#include <iterator>
#include <string>

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
    left = NULL;
    right = NULL;
    value = -1;
    if (ext) {
      color = black;
    }
  }

  bool IsExternal();
  bool IsBlack();
  bool IsComplete();

  KeyType GetKey();
  ValueType GetValue();
  TreeNode<KeyType, ValueType> * GetLeft();
  TreeNode<KeyType, ValueType> * GetRight();

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

  void buildVector(treenode_t *root, int depth, std::vector<std::vector<treenode_t *> > &ret);
  void print_tree();
  bool checkCompleteness();
  treenode_t *GetRoot() {return root_;}
  bool checkBlackDepth();
  
  private:
  treenode_t *root_;
  void fix_insert(std::vector<treenode_t*> &v);
  bool has_red_child_or_grandchild(treenode_t *cur);
  bool has_red_child(treenode_t *cur);
  void fix_delete(std::vector<treenode_t *> &v);
  void swapNodes(treenode_t *node1, treenode_t *node2);
  void rotateLeft(treenode_t *parent);
  void rotateRight(treenode_t *parent);
  void delete_tree(treenode_t *cur);
  int computeBlackDepth(treenode_t *curNode);
};

template <typename KeyType, typename ValueType>
bool TreeNode<KeyType, ValueType>::IsExternal() {
  return isExternal_;
}

template <typename KeyType, typename ValueType>
bool TreeNode<KeyType, ValueType>::IsBlack() {
  return color == black;
}

template <typename KeyType, typename ValueType>
bool TreeNode<KeyType, ValueType>::IsComplete() {
  if (left == NULL && right == NULL) {
    return true;
  }

  if (left != NULL && right != NULL) {
    return left->IsComplete() && right->IsComplete();
  }

  return false;
}

template <typename KeyType, typename ValueType>
KeyType TreeNode<KeyType, ValueType>::GetKey() {
  return key;
};


template <typename KeyType, typename ValueType>
ValueType TreeNode<KeyType, ValueType>::GetValue() {
  return value;
};


template <typename KeyType, typename ValueType>
TreeNode<KeyType, ValueType> * TreeNode<KeyType, ValueType>::GetLeft() {
  return left;
};


template <typename KeyType, typename ValueType>
TreeNode<KeyType, ValueType> * TreeNode<KeyType, ValueType>::GetRight() {
  return right;
};


    template <typename KeyType, typename ValueType>
RBTree<KeyType, ValueType>::RBTree() {
  root_ = NULL;
}

template <typename KeyType, typename ValueType>
RBTree<KeyType, ValueType>::~RBTree() {
  delete_tree(root_);
}

template <typename KeyType, typename ValueType>
void RBTree<KeyType, ValueType>::delete_tree(treenode_t *cur) {
  if (cur == NULL) return;
  delete_tree(cur->left);
  delete_tree(cur->right);
  delete cur;
}

template <typename KeyType, typename ValueType>
TreeNode<KeyType, ValueType> *RBTree<KeyType, ValueType>::Search(KeyType key) {
  treenode_t *cur = root_;
  while (cur != NULL) {
    if (!cur->IsExternal()) {
      if (key <= cur->key) {
        cur = cur->left;
      }
      else {
        cur = cur->right;
      }
    }
    else {
      if (cur->key == key) {
        return cur;
      }
      return NULL;
    }
  }
  return NULL;
}

template <typename KeyType, typename ValueType>
void RBTree<KeyType, ValueType>::Insert(KeyType key, ValueType value) {
  if (root_ == NULL) {
    root_ = new treenode_t(true);
    root_->color = black;
    root_->key = key;
    root_->value = value;
    return;
  } 
// 1. make the top-down invariant true initially
  treenode_t *x = root_;
  if (x->color == red) x->color = black;
  //if (x->left != NULL && x->left->color == red \
  //    && x->right != NULL && x->right->color == red) {
  //  x->left->color = black;
  //  x->right->color = black;
  //}
  // 2. walking down from the current node
  treenode_t *y = x;
  treenode_t *prev = NULL;
  std::vector<treenode_t *> q;
  int successiveBlk = 0;
  while (!y->IsExternal()) {
    // check if node is black with 2 red chilren
    //std::cout << "successiveBlk = " << successiveBlk << std::endl;
    q.push_back(y);
    prev = y;
    if (y->color == black) {
      if (y->left->color == red && y->right->color == red) {
        successiveBlk ++;
      }
      //else if (y->left->color == black && key <= y->key){
      //  successiveBlk ++;
      //}
      //else if (y->right->color == black && key > y->key) {
      //  successiveBlk ++;
      //}
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
    if (key <= y->key) {
      y = y->left;
    }
    else if (key > y->key) {
      y = y->right;
    }
    // b. a black node with a black child is reached
    if (y->color == black && !y->IsExternal()) {
      if (y->left->color == black || y->right->color == black) {
        // repeat current node x by y
        x = y;
        q.clear();
      }
    }

  } // while
  // node y is external; must perform bottom-up insertion
  //std::cout << "x = " << x->key << std::endl;
  //if (!q.empty()) std::cout << "q[0] = " << q[0]->key << std::endl;
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
  if (prev != NULL) {
    if (new_int->key <= prev->key) {
      prev->left = new_int;
    }
    else {
      prev->right = new_int;
    }
  }
  else {
    root_ = new_int;
    root_->color = black;
  }
  if (q.empty()) {
    q.push_back(y);
  }
  if (q.back() == x && x->IsExternal()) {
    q.pop_back(); // pop off y
  }
  q.push_back(new_int);
  //q.push_back(new_item);
  //for (auto key:q) {
  //  std::cout << key->key << ",";
  //}
  //std::cout << std::endl;
  //q.pop_back();
  //std::cout << "FIXING INSERT:..." << std::endl;
  //print_tree();
  //print2D(root_);
  //std::cout << "------------------" << std::endl;
  fix_insert(q);
}

template <typename KeyType, typename ValueType>
void RBTree<KeyType, ValueType>::fix_insert(std::vector<treenode_t *> &q) {
  if (q.size() == 1) return;
  bool is_violation = true;
  while (is_violation) {
    is_violation = false;
    treenode_t *cur = q.back();
    if (cur->key == 4) {
      printf("a");
    }
    if (cur->color == red) {
      treenode_t *parent = q[q.size()-2];
      treenode_t *sibling;
      if (parent == q[0]) break;
      if (parent->color == red) { //&& sibling->color == red) {
        is_violation = true;
        treenode_t *grandparent = q[q.size()-3];
        if (grandparent == q[0]) {
          q.pop_back();
          break;
        }
        if (parent->key <= grandparent->key) {
          sibling = grandparent->right;
        }
        else {
          sibling = grandparent->left;
        }
        if (sibling->color == red) {
          grandparent->color = red;
          parent->color = black;
          sibling->color = black;
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
    if (window_root->color == red && sibling->color == black && cur->color == red) {
      if (((cur->left != NULL && cur->left->color == black) || cur->left == NULL) && \
        ((cur->right != NULL && cur->right->color == black) || cur->right == NULL)) {
        // color the node black
        window_root->color = black;
      }
    }
    else if (window_root->color == black && cur->color == red && sibling->color == red) {
      window_root->color = black;
      cur->color = black;
      sibling->color = black;
    }
    // 3-d and e, cur is red and has a red child
    else if (window_root->color == black && cur->color == red && \
      sibling->color == black && (cur->left->color == red \
      || cur->right->color == red)) {
      // re-color the tree, then rotate
      cur->color = black;
      if (direction == 0) {
        // 3e, left case
        if (cur->right->color == red) {
          rotateLeft(cur);
        }
        rotateRight(window_root);
        std::cout << "rotating dir = 0 on key: " << window_root->key << " color: " << window_root->color << std::endl;
        window_root->color = black;
        window_root->right->color = red;
        window_root->right->right->color = black;
        window_root->left->color = red;
        window_root->left->left->color = black;
        window_root->left->right->color = black;
      }
      else {
        // 3e, right case
        if (cur->left->color == red) {
          rotateRight(cur);
        }
        rotateLeft(window_root);
        std::cout << "rotating dir = 1 on key: " << window_root->key << " color: " << window_root->color << std::endl;
        window_root->color = black;
        window_root->left->color = red;
        window_root->left->left->color = black;
        window_root->right->color = red;
        window_root->right->left->color = black;
        window_root->right->right->color = black;
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
  if (cur->left != NULL && !cur->left->IsExternal() && cur->left->color == red) {
    return true;
  }
  if (cur->right != NULL && !cur->right->IsExternal() && cur->right->color == red) {
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

  if (cur->IsExternal()) {

    swapNodes(par, sibling);

    if (sibling->color == red) {
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
    } else {
      if (!cur->IsExternal()) {
        if (cur->color == red) {
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
  }

  while (true) {

    if (par->color == black && sibling->color == black && !has_red_child(sibling)) {

      sibling->color = red;

      cur = par;

      r_itr++;
      if (r_itr != v.rend()) {
        par = *r_itr;
      } else {
        if (!cur->IsExternal()) {
          if (cur->color == red) {
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
    } else {
      break;
    }
  }

  if (cur->color == black && par->color == black && sibling->color == red) {
    par->color = red;
    sibling->color = black;
    if (sibling == par->right) {
      rotateLeft(par);

      par = par->left;

      cur = par->left;
      sibling = par->right;
    } else {
      rotateRight(par);

      par = par->right;

      cur = par->right;
      sibling = par->left;
    }
  }

  if (par->color == red && sibling->color == black && !has_red_child(sibling)) {
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
          tempNode = v.back();
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
void RBTree<KeyType, ValueType>::buildVector(treenode_t *root, int depth, \
  std::vector<std::vector<treenode_t *> > &ret) {
    if(root == NULL) return;
    if(ret.size() == depth)
        ret.push_back(std::vector<treenode_t *>());
    
    ret[depth].push_back(root);
    if (root->left != NULL) buildVector(root->left, depth + 1, ret);
    if (root->right != NULL) buildVector(root->right, depth + 1, ret);
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
      if (cur->color == black) {
        cl = "black";
      }
      else {
        cl = "red";
      }
      printf("(%d, %d, %s, %d). ", cur->key, cur->value, cl.c_str(), cur->IsExternal());
    }
    printf("\n");
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
    // Computes the height for the left and right child recursively
    int leftHeight = computeBlackDepth(curNode->left);
    int rightHeight = computeBlackDepth(curNode->right);
    int add = (curNode->color == black) ? 1 : 0;
    // The current subtree is not a red black tree if and only if
    // one or more of current node's children is a root of an invalid tree
    // or they contain different number of black nodes on a path to a null node.
    if (leftHeight == -1 || rightHeight == -1 || leftHeight != rightHeight) {
        printf("The subtree rooted at key = %d is not a valid RB tree.\n", curNode->key);
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

  if (root_ != NULL && root_->left == NULL && root_->right == NULL) {
    return true;
  }

  return root_->IsComplete();
};


template <typename KeyType, typename ValueType>
void RBTree<KeyType, ValueType>::swapNodes(treenode_t *node1, treenode_t *node2) {
  KeyType tmpKey = node1->key;
  color_t tmpColor = node1->color;
  treenode_t *tmpLeft = node1->left;
  treenode_t *tmpRight = node1->right;
  bool tmpIsExternal_ = node1->isExternal_;
  ValueType tmpValue = node1->value;

  node1->key = node2->key;
  node1->color = node2->color;
  node1->left = node2->left;
  node1->right = node2->right;
  node1->isExternal_ = node2->isExternal_;
  node1->value = node2->value;

  node2->key = tmpKey;
  node2->color = tmpColor;
  node2->left = tmpLeft;
  node2->right = tmpRight;
  node2->isExternal_ = tmpIsExternal_;
  node2->value = tmpValue;
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

#endif