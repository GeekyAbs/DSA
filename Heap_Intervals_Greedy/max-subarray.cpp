// Kadane's Algorithm to find the maximum subarray
#include <bits/stdc++.h>
using namespace std;
class Solution {
public:
  int maxSubArray(vector<int> &nums) {
    int max_sum = nums[0];
    int sum = 0;
    for (int num : nums) {
      sum += num;
      max_sum = max(max_sum, sum);
      if (sum < 0)
        sum = 0;
    }
    return max_sum;
  }
};

int main() {
  vector<int> nums = {-2, 1, -3, 4, -1, 2, 1, -5, 4};
  Solution sol;
  int res = sol.maxSubArray(nums);
  cout << res << endl;
  return 0;
}
