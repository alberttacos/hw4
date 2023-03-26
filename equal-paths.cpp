#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)
#include <algorithm>
#include <iostream>
#include <cstdlib>
#include "equal-paths.h"
#endif




// You may add any prototypes of helper functions here

using namespace std;



// You may add any prototypes of helper functions here
int lengthOFpath(Node* root, int lvl);





int lengthOFpath(Node* root,int lvl){
    if(root == nullptr){
        return true;
    }else if(root->left == nullptr && root->right == nullptr){
      return lvl;
    }
    else {
        int leftnode = lengthOFpath(root->left, lvl + 1);
        int rightnode = lengthOFpath(root ->right, lvl + 1);
        return leftnode;
        return rightnode;
    }
}

bool leafcheck(Node* root, int lvl, int leaf){
  if (root == nullptr){
    return true;
  }
  if(root->left == nullptr && root->right == nullptr){
    return lvl == leaf;
  }
  return (leafcheck(root->left, lvl + 1, leaf) && leafcheck(root->right, lvl + 1, leaf));
}

bool equalPaths(Node * root)
{
    // Add your code below
    int lvl = 0;
    int leaf = lengthOFpath(root,lvl);
    return leafcheck(root,lvl,leaf);
}
