#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
  int findMin(vector<int> &nums) {
    int l = 0, r = nums.size() - 1;
    while (l < r) {
      int mid = l + (r - l) / 2;
      if (nums[mid] > nums[r])
        l = mid + 1;
      else
        r = mid;
    }
    return nums[l];
  }
};
int main() {
  vector<int> nums = {4, 5, 6, 7, 0, 1, 2};
  Solution sol;
  int ans = sol.findMin(nums);
  cout << ans << endl;
}
