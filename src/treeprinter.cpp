#include <iostream>

#include "tree.h"
#include "treeprinter.h"

using namespace std;

namespace lock_free_rbtree {
static int COUNT = 5;


static void print2DUtil(TreeNode<int, int> *root, int space)
{
    // Base case
    if (root == NULL)
        return;
 
    // Increase distance between levels
    space += COUNT;
 
    // Process right child first
    print2DUtil(root->GetRight(), space);
 
    // Print current node after space
    // count
    printf("\n");
    for (int i = COUNT; i < space; i++)
        printf(" ");
    
    char ext;
    if (root->IsExternal())
        ext = '*';
    else if (root->IsBlack())
        ext = 'B';
    else
        ext = 'R';

    char own;
    if (root->GetOwn()) {
        own = 'O';
    }
    else {
        own = 'F';
    }
    printf("%d%c%c\n", root->GetKey(), ext, own);
 
    // Process left child
    print2DUtil(root->GetLeft(), space);
}


void print2D(TreeNode<int, int> *root)
{
   // Pass initial space count as 0
   print2DUtil(root, 0);
}

}
