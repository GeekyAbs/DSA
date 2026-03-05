// think, you will realize you need to back track from the end
// now backtracking won't be the optimized solution
// so iterate from the end and update the goal if you can reach that index
// if i can reach the start index, return true
#include <bits/stdc++.h>
#include <ios>
using namespace std;

class Solution {
public:
  bool canJump(vector<int> &nums) {
    int n = nums.size();
    if (n == 1)
      return true;
    int goal = n - 1;
    for (int i = goal - 1; i >= 0; i--) {
      if (i + nums[i] >= goal)
        goal = i;
    }
    return goal == 0;
  }
};
int main() {
  vector<int> nums = {2, 3, 1, 1, 4};
  Solution sol;
  cout << boolalpha << sol.canJump(nums) << endl;
}
