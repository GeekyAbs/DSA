/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left),
 * right(right) {}
 * };
 */
// A height-balanced binary tree is a binary tree in which the depth of the two
// subtrees of every node never differs by more than one.
class Solution {
public:
  int flag = 0;
  int height(TreeNode *root) {
    if (root == NULL || flag == 1)
      return 0;
    int lh = height(root->left);
    int rh = height(root->right);
    if (abs(lh - rh) > 1) {
      flag = 1;
      return 0;
    }
    return 1 + max(lh, rh);
  }
  bool isBalanced(TreeNode *root) {
    if (!root)
      return true;
    height(root);
    return flag == 0;
  }
};
