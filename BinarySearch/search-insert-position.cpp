// such a hidden in plain sight problem
// make sure to go till the last iteration of a binary search
#include <bits/stdc++.h>
using namespace std;
class Solution {
public:
  int searchInsert(vector<int> &nums, int target) {
    int l = 0, r = nums.size() - 1;
    while (l <= r) {
      int mid = l + (r - l) / 2;
      if (nums[mid] == target)
        return mid;
      else if (target < nums[mid])
        r = mid - 1;
      else
        l = mid + 1;
    }
    return l;
  }
};
int main() {
  vector<int> nums = {1, 3, 5, 6};
  int target = 7;
  Solution sol;
  cout << sol.searchInsert(nums, target) << endl;
}
