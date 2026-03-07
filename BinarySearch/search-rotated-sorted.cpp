// step 1 is to figure out which part is the sorted part of the array
// step 2 then check if in that part the target exists
// if yes, search that part, otherwise move to the other part
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
  int search(vector<int> &nums, int target) {
    int l = 0, r = nums.size() - 1;
    while (l <= r) {
      int mid = l + (r - l) / 2;
      if (nums[mid] == target)
        return mid;
      else if (nums[l] < nums[mid]) {
        if (target >= nums[l] && target < nums[mid])
          r = mid - 1;
        else
          l = mid + 1;
      } else {
        if (target <= nums[r] && target > nums[mid])
          l = mid + 1;
        else
          r = mid - 1;
      }
    }
    return -1;
  }
};
int main() {
  vector<int> nums = {4, 5, 6, 7, 0, 1, 2};
  int target = 0;
  cout << Solution().search(nums, target) << endl;
}
