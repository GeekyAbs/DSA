// for almost any monotonic stack problems like NGE
// while(!stack.empty() && bad_condition)
//    stack.pop();
#include <bits/stdc++.h>
using namespace std;

vector<int> dailyTemperatures(vector<int> &temperatures){
  int n = temperatures.size();
  vector<int> ans(n);
  stack<int> s;

  for(int i=n-1; i>=0; i--){
    int curr = temperatures[i];
    while (!s.empty() && temperatures[s.top()] <= curr){
      s.pop();
    }
    if (s.empty()){
      ans[i] = 0;
      s.push(i);
    }
    else{
      ans[i] = s.top() - i;
      s.push(i);
    }
  }
  return ans;
}

int main(){
  vector<int> temperatures = {73,74,75,71,69,72,76,73};
  vector<int> ans = dailyTemperatures(temperatures);
  for(int x : ans) cout << x << " ";
}
