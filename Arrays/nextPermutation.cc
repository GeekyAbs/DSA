#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
  void nextPermutation(vector<int> &nums) {
    int idx = -1, n = nums.size();
    // find deflection point
    for (int i = n - 2; i >= 0; i--) {
      if (nums[i] > nums[i + 1]) {
        idx = i;
        break;
      }
    }
    if (idx == -1) {
      reverse(nums.begin(), nums.end());
      return;
    }
    // find the swap element and swap
    for (int i = n - 1; i > idx; i--) {
      if (nums[i] > nums[idx]) {
        swap(nums[i], nums[idx]);
        break;
      }
    }
    // reverse after the idx now
    reverse(nums.begin() + idx + 1, nums.end());
    return;
  }
};

int main() {
  int x;
  vector<int> nums;
  while (cin >> x) {
    nums.push_back(x);
  }

  Solution sol;
  sol.nextPermutation(nums);

  for (int i = 0; i < nums.size(); i++) {
    cout << nums[i];
    if (i < nums.size() - 1)
      cout << " ";
  }
  cout << endl;
  return 0;
}
