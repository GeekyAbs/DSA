#include <cmath>
#include <iostream>
#include <vector>
using namespace std;

class Solution {
public:
  int findDuplicate(vector<int> &nums) {
    int i = 0;
    while (true) {
      int next_idx = abs(nums[i]);
      if (nums[next_idx] < 0)
        return next_idx;
      nums[i] *= -1; // visited
      i = next_idx;  // pointing to next index
    }
  }
};

int main() {
  vector<int> nums = {1, 3, 4, 2, 2};
  Solution sol;
  cout << sol.findDuplicate(nums) << endl;
  return 0;
}
