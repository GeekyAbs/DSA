#include <bits/stdc++.h>
using namespace std;
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
// i know how to do bfs, that is what this question asks
// what i need to figure out is that how do i return vector of levels
// for that i need to know how many nodes there are in each level
// build a level array and then push it to main ans after each level is complete

struct TreeNode {
  int val;
  TreeNode *left;
  TreeNode *right;
  TreeNode() : val(0), left(nullptr), right(nullptr) {}
  TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
  TreeNode(int x, TreeNode *left, TreeNode *right)
      : val(x), left(left), right(right) {}
};

class Solution {
public:
  vector<vector<int>> levelOrder(TreeNode *root) {
    vector<vector<int>> ans;
    if (!root)
      return ans;

    queue<TreeNode *> q;
    q.push(root);

    while (!q.empty()) {
      int n = q.size();
      vector<int> level;
      for (int i = 0; i < n; i++) {
        TreeNode *curr = q.front();
        q.pop();
        level.push_back(curr->val);

        if (curr->left)
          q.push(curr->left);
        if (curr->right)
          q.push(curr->right);
      }
      ans.push_back(level);
    }
    return ans;
  }
};

int main() {
  TreeNode *root = new TreeNode(1);
  root->left = new TreeNode(2);
  root->right = new TreeNode(3);
  root->left->left = new TreeNode(4);
  root->left->right = new TreeNode(5);
  root->right->right = new TreeNode(6);

  Solution obj;
  vector<vector<int>> result = obj.levelOrder(root);

  cout << "Level Order Traversal:\n";
  cout << "[";
  for (int i = 0; i < result.size(); i++) {
    cout << "[";
    for (int j = 0; j < result[i].size(); j++) {
      cout << result[i][j];
      if (j != result[i].size() - 1)
        cout << ",";
    }
    cout << "]";
    if (i != result.size() - 1)
      cout << ",";
  }
  cout << "]" << endl;

  return 0;
}
