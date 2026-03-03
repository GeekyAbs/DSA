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
// think of height of a sub tree formula
// think of diameter formula
// think about recursively calculating left and right height of a node and
// update
class Solution {
public:
  int max_dia = 0;
  int height(TreeNode *root) {
    if (root == NULL)
      return 0;
    int lh = height(root->left);
    int rh = height(root->right);
    max_dia = max(lh + rh, max_dia);
    return 1 + max(lh, rh);
  }
  int diameterOfBinaryTree(TreeNode *root) {
    height(root);
    return max_dia;
  }
};
