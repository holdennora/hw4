#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here
bool equalUntil(Node* node, int lenPath, int &lenPrev)
{
    if (!node->right && !node->left)
    {
        if (lenPrev == -1)
        {
            lenPrev = lenPath;
            return true;
        }
        return lenPrev == lenPath;
    }

    bool leftSubT = true;
    if (node->left)
    {
       leftSubT = equalUntil(node->left, lenPath+1, lenPrev);
    }
    bool rightSubT = true;
    if (node->right)
    {
        rightSubT = equalUntil(node->right, lenPath+1, lenPrev);
    }


    return leftSubT && rightSubT;
}




bool equalPaths(Node * root)
{
    // Add your code below
    if (!root)
    {
        return true;
    }
    int lenPrev = -1;
    return equalUntil(root, 0, lenPrev);
}

