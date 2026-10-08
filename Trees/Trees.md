# **Section 1: Tree Patterns**

### **1: Section Intro, Tree Patterns**

**Section 1: Tree Patterns**

Trees are one of the most heavily tested data structures in coding interviews. A tree is a linked list that branches: instead of one `next` pointer, each node has `left` and `right`. This branching structure means that iteration alone is not enough. You need recursion (or an explicit stack/queue) to traverse the tree, and you need to reason about what information flows down from parent to child and what information returns upward from child to parent.

Most tree problems reduce to a single question: should I use DFS (go deep first) or BFS (go level by level)? And for DFS: what do I pass down, and what do I return up? Once you can answer these two questions for any tree problem, the code follows naturally.

**What We’ll Cover:**

1. Tree terminology and mental model
2. Recursive DFS: preorder, inorder, postorder
3. Returning values upward in recursion
4. Iterative DFS with explicit stack
5. BFS and level order traversal
6. BST invariants and validation
7. Tree path problems and lowest common ancestor

**Where This Appears in Interviews:**

| Company Type       | How This Section Appears                                                                |
| ------------------ | --------------------------------------------------------------------------------------- |
| FAANG / Big Tech   | Diameter of Binary Tree, Validate BST, and Lowest Common Ancestor are top-20 most-asked |
| Mid-size companies | Maximum Depth, Level Order Traversal, and Path Sum are common phone screen questions    |
| Startups           | Tree problems are less frequent but appear when testing recursive thinking              |
| Online assessments | Tree traversal and BST problems are common timed questions                              |

**Core Idea for This Section:**  
Most tree problems are DFS or BFS plus a decision about what information flows down (parameters) and what information returns upward (return values).

---

### **2: Tree Terminology and Mental Model**

**Key Terms:**

| Term                     | Definition                                                              |
| ------------------------ | ----------------------------------------------------------------------- |
| Root                     | The topmost node, has no parent                                         |
| Leaf                     | A node with no children (both left and right are `nullptr`)             |
| Edge                     | A connection between a parent and a child                               |
| Depth                    | Distance from the root to a node (root has depth 0)                     |
| Height                   | Distance from a node to its deepest leaf descendant (leaf has height 0) |
| Subtree                  | A node plus all of its descendants                                      |
| Binary tree              | Each node has at most two children                                      |
| Binary search tree (BST) | Left subtree values < node value < right subtree values                 |
| Balanced tree            | Height difference between left and right subtrees is at most 1          |
| Complete tree            | Every level is full except possibly the last, which fills left to right |

**The Standard Node Definition:**

```cpp
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right)
        : val(x), left(left), right(right) {}
};
```

**Mental Model:**  
Think of every tree problem as a set of decisions made at each node. At each node, you can: look at the value, recurse into the left subtree, recurse into the right subtree, combine results from left and right, and return something upward. The order in which you do these things determines whether it is preorder, inorder, or postorder traversal.

**Interview Relevance:** Interviewers assume you know these terms. If you confuse depth and height, or cannot define a BST invariant, it signals a gap in fundamentals. Spend 5 minutes reviewing this terminology before any tree-focused interview.

---

### **3: Recursive DFS, Preorder, Inorder, Postorder**

**Three Ways to Visit Nodes:**  
The difference between preorder, inorder, and postorder is when you process the current node relative to its children.

| Traversal | Order             | When to Use                                                                              |
| --------- | ----------------- | ---------------------------------------------------------------------------------------- |
| Preorder  | Node, Left, Right | When you need to process the node before its children (copying a tree, serialization)    |
| Inorder   | Left, Node, Right | When you need sorted order from a BST                                                    |
| Postorder | Left, Right, Node | When you need children’s results before processing the node (computing height, diameter) |

**Templates:**

```cpp
void preorder(TreeNode* node) {
    if (!node) return;
    process(node->val);
    preorder(node->left);
    preorder(node->right);
}

void inorder(TreeNode* node) {
    if (!node) return;
    inorder(node->left);
    process(node->val);
    inorder(node->right);
}

void postorder(TreeNode* node) {
    if (!node) return;
    postorder(node->left);
    postorder(node->right);
    process(node->val);
}
```

**Inorder on a BST Gives Sorted Order:**  
If you perform an inorder traversal on a valid BST, the values come out in ascending order. This property is the basis for validating a BST and for finding the kth smallest element.

**Interview Tip:** When an interviewer asks “traverse this tree,” ask which order they want. If they say “doesn’t matter,” use preorder (it is the most intuitive). If the tree is a BST and order matters, use inorder.

**Interview Relevance:** The traversal order you choose signals whether you understand the problem structure. Using postorder for a height problem and inorder for a BST problem shows you are thinking about information flow, not just applying a template.

---

### **4: Problem Walkthrough, Maximum Depth of Binary Tree**

**Problem: Maximum Depth of Binary Tree (LeetCode 104)**

Given the root of a binary tree, return its maximum depth. The maximum depth is the number of nodes along the longest path from root to leaf.

```cpp
int maxDepth(TreeNode* root) {
    if (!root) return 0;
    int leftDepth = maxDepth(root->left);
    int rightDepth = maxDepth(root->right);
    return max(leftDepth, rightDepth) + 1;
}
```

**Trace on a sample tree:**

```
        3
       / \
      9   20
         / \
        15   7

maxDepth(3):
  maxDepth(9):
    maxDepth(nullptr) = 0
    maxDepth(nullptr) = 0
    return max(0, 0) + 1 = 1
  maxDepth(20):
    maxDepth(15) = 1
    maxDepth(7) = 1
    return max(1, 1) + 1 = 2
  return max(1, 2) + 1 = 3
```

Result: 3.

**Why This Is Postorder:**  
You need the depths of both children before you can compute the depth of the current node. This means children are processed first, then the current node. That is postorder.

**Base Case:**  
When `root` is `nullptr`, the depth is 0. This is the standard base case for almost every tree recursion. A null node contributes nothing.

**Interview Relevance:** Maximum Depth is the simplest tree recursion problem and is often the first tree question in an interview. It establishes the pattern of “recurse left, recurse right, combine results, return upward” that applies to nearly every DFS tree problem.

---

### **5: Returning Values Upward in Recursion**

**The Most Important Concept in Tree Problems:**

In tree recursion, information flows in two directions. Parameters carry information downward from parent to child. Return values carry information upward from child to parent. Choosing what to return upward is the key design decision in most tree problems.

**Examples of What Returns Upward:**

|Problem|What Each Recursive Call Returns|How the Parent Uses It|
|---|---|---|
|Maximum Depth|Height of the subtree|max(left, right) + 1|
|Diameter|Height of the subtree|Diameter at this node = left_height + right_height|
|Is Balanced|Height of the subtree (or -1 if unbalanced)|Check if difference > 1|
|Path Sum|bool (does a path with target sum exist)|Return left OR right|
|Lowest Common Ancestor|The LCA node if found in this subtree, else nullptr|Combine left and right results|
|Validate BST|bool (is this subtree a valid BST)|Return false if any subtree is invalid|

**The Pattern:**  
For each problem, ask two questions:

1. What does each recursive call need to return?
2. How does the current node combine the results from left and right?

If you can answer these two questions, the code writes itself.

**Common Mistake:**  
Using a global variable to track state instead of returning values upward. Global variables work but make the code harder to reason about and easier to break. Returning values upward is cleaner, more functional, and what interviewers expect.

**Interview Relevance:** This section describes the single most important skill in tree problems. Every tree problem from this point forward uses this “return upward” pattern. If you internalize it here, the remaining tree problems become variations of the same idea.

---

### **6: Problem Walkthrough, Diameter of Binary Tree**

**Problem: Diameter of Binary Tree (LeetCode 543)**

The diameter is the length of the longest path between any two nodes. The path does not need to pass through the root. The length is measured in number of edges.

**The Key Insight:**  
At every node, the longest path through that node is `left_height + right_height`. The diameter of the tree is the maximum of this value across all nodes. Each recursive call returns the height of its subtree (for the parent to use), but also updates a captured maximum diameter.

```cpp
// Helper function to calculate height and track the maximum diameter
int height(TreeNode* node, int& maxDiameter) {
    if (node == nullptr) {
        return 0;
    }
    
    int leftH = height(node->left, maxDiameter);
    int rightH = height(node->right, maxDiameter);
    
    // Update the maximum diameter found so far
    maxDiameter = std::max(maxDiameter, leftH + rightH);
    
    // Return the height of this subtree
    return std::max(leftH, rightH) + 1;
}

int diameterOfBinaryTree(TreeNode* root) {
    int maxDiameter = 0;
    height(root, maxDiameter);
    return maxDiameter;
}
```

Time: O(n), each node visited once. Space: O(h) for the recursion stack, where h is the height of the tree.

**Why `maxDiameter` Is Captured by Reference:**  
The inner `height` lambda captures `maxDiameter` with `[&]`, so every recursive call updates the single outer variable. If you captured by value (`[=]`), each call would get its own copy and the running maximum would never propagate. Alternatively, make the tracker a member variable of a `Solution` class.

**Interview Relevance:** Diameter of Binary Tree is a top-15 most-asked tree problem. It is the first problem most candidates encounter where the return value (height) is different from the answer (diameter). Understanding this distinction is critical.

---

### **6B: Diameter, How Information Flows Up the Tree**

**Trace:**

```
        1
       / \
      2   3
     / \
    4   5

height(1):
  height(2):
    height(4):
      left=0, right=0
      diameter at 4: 0+0=0, maxDiameter=0
      return max(0,0)+1 = 1
    height(5):
      left=0, right=0
      diameter at 5: 0+0=0, maxDiameter=0
      return 1
    diameter at 2: 1+1=2, maxDiameter=2
    return max(1,1)+1 = 2
  height(3):
    left=0, right=0
    diameter at 3: 0+0=0, maxDiameter=2
    return 1
  diameter at 1: 2+1=3, maxDiameter=3
  return max(2,1)+1 = 3

Final diameter: 3 (path: 4 -> 2 -> 1 -> 3)
```

**Two Different Values at Each Node:**

|At Node|Height Returned Upward|Diameter Through This Node|
|---|---|---|
|Node 4|1|0|
|Node 5|1|0|
|Node 2|2|2 (path 4-2-5)|
|Node 3|1|0|
|Node 1|3|3 (path 4-2-1-3)|

The height returns upward for the parent to use. The diameter is computed at each node and compared against the captured maximum. These are two separate computations happening in the same recursive call.

**Note It Down:** In many tree problems, the value you return upward is not the final answer. The return value serves the parent. The answer is tracked separately (often in a variable captured by reference, or a class member). Recognizing when the return value differs from the answer is the single most important insight for tree recursion.

**Interview Relevance:** Interviewers specifically test whether you can explain why the function returns height but the answer is diameter. Being able to articulate this dual-purpose recursion separates strong candidates from those who memorized the solution.

---

### **7: Interview Practice, DFS Tree Patterns**

**Test yourself. Identify what the recursive call should return before reading the answer.**

**Problem 1:**  
Given a binary tree, determine if it is height-balanced (every node’s left and right subtree heights differ by at most 1).

What should each recursive call return?

Answer: The height of the subtree, or -1 as a sentinel indicating the subtree is unbalanced. At each node, compute left and right heights. If either is -1 or the difference exceeds 1, return -1. Otherwise return `max(left, right) + 1`. Check if the root’s result is not -1.

**Problem 2:**  
Given a binary tree, invert it (mirror image). Every left child becomes the right child and vice versa.

What should each recursive call return?

Answer: The root of the inverted subtree. At each node, recursively invert left and right, then swap: `swap(node->left, node->right)`. Return the node. This is preorder: swap first, then the recursive calls handle the subtrees.

**Problem 3:**  
Given two binary trees, check if they are the same tree (identical structure and values).

What should each recursive call return?

Answer: `bool`. Base cases: both `nullptr` returns true, one `nullptr` returns false. Recursive case: values must match, and both left subtrees and both right subtrees must be the same. Return `p->val == q->val && same(p->left, q->left) && same(p->right, q->right)`.

**Interview Relevance:** These three problems all follow the “return upward” pattern from section 5. The skill is identifying what to return before writing code. Practice this mental step for every tree problem.

---

### **8: Iterative DFS with Explicit Stack**

**Why Iterative DFS Exists:**  
Recursive DFS uses the call stack implicitly. For very deep trees (depth exceeding the available stack space), this causes a stack overflow. Iterative DFS uses an explicit `std::stack`, which is heap-allocated and has no practical depth limit.

**Iterative Preorder Template:**

```cpp
#include <stack>
#include <vector>
using namespace std;

vector<int> preorderIterative(TreeNode* root) {
    vector<int> result;
    if (!root) return result;
    stack<TreeNode*> st;
    st.push(root);
    while (!st.empty()) {
        TreeNode* node = st.top(); st.pop();
        result.push_back(node->val);
        if (node->right) st.push(node->right);
        if (node->left) st.push(node->left);
    }
    return result;
}
```

**Why Right Is Pushed Before Left:**  
The stack is LIFO. If you push right first, left is on top and gets processed first. This gives the correct preorder sequence: node, left subtree, right subtree.

**Iterative Inorder Template:**

```cpp
vector<int> inorderIterative(TreeNode* root) {
    vector<int> result;
    stack<TreeNode*> st;
    TreeNode* current = root;
    while (current || !st.empty()) {
        while (current) {
            st.push(current);
            current = current->left;
        }
        current = st.top(); st.pop();
        result.push_back(current->val);
        current = current->right;
    }
    return result;
}
```

Inorder is trickier because you need to go as far left as possible before processing. The inner while loop pushes all left children onto the stack.

**When to Use Iterative vs Recursive:**

|Situation|Use|
|---|---|
|Default for interviews|Recursive (simpler to write and explain)|
|Tree might be very deep (100,000+ nodes)|Iterative (avoids stack overflow)|
|Interviewer asks for iterative explicitly|Iterative|
|Need to process nodes in a specific non-recursive order|Iterative (more control over traversal)|

**Interview Relevance:** Most interviewers accept recursive solutions. However, being able to write iterative DFS when asked shows deeper understanding of how recursion works under the hood, connecting directly to the stack concepts from the previous course.

---

### **9: BFS and Level Order Traversal**

**Recognition Signal:**  
The problem asks about levels, layers, depth, or requires processing all nodes at the same depth before moving deeper. Words like “level by level,” “shortest path in an unweighted tree,” or “layer” signal BFS.

**How BFS Works on Trees:**  
Use a queue. Start with the root. For each node, process it and add its children to the queue. Process all nodes at the current level before moving to the next level.

**Why BFS Processes Level by Level:**  
The queue ensures FIFO (first in, first out) order. All nodes at depth d are added to the queue before any node at depth d+1. So when you process the queue, you naturally process one level at a time.

**When to Use BFS vs DFS:**

|Use BFS When|Use DFS When|
|---|---|
|Need to process level by level|Need to explore full paths to leaves|
|Need shortest path (unweighted)|Need to return information upward|
|Need to find something at minimum depth|Need to check properties of subtrees|
|Zigzag or level-specific operations|Height, diameter, path sum calculations|

**Interview Tip:** If the problem mentions “level,” “depth,” “layer,” or “shortest,” default to BFS. If it mentions “path,” “height,” “subtree,” or “validate,” default to DFS. This heuristic is correct about 90% of the time.

**Interview Relevance:** BFS on trees is simpler than BFS on graphs (no visited set needed, since trees have no cycles). Mastering tree BFS first makes graph BFS in the next section easier to learn.

---

### **10: BFS Template**

**Level Order BFS Template:**

```cpp
#include <queue>
using namespace std;

vector<vector<int>> levelOrder(TreeNode* root) {
    vector<vector<int>> result;
    if (!root) return result;
    queue<TreeNode*> q;
    q.push(root);
    while (!q.empty()) {
        int levelSize = q.size();
        vector<int> currentLevel;
        for (int i = 0; i < levelSize; i++) {
            TreeNode* node = q.front(); q.pop();
            currentLevel.push_back(node->val);
            if (node->left) q.push(node->left);
            if (node->right) q.push(node->right);
        }
        result.push_back(currentLevel);
    }
    return result;
}
```

**The Key Trick: `int levelSize = q.size();`**  
At the start of each level, `q.size()` tells you how many nodes are at the current level. Process exactly that many nodes, adding their children as you go. The children are added to the back of the queue and will be processed in the next iteration of the outer while loop. Note that `q.size()` is captured before the inner loop runs, so pushes inside the loop do not affect it.

**Why `std::queue` and Not a `vector`:**  
`vector.erase(v.begin())` is O(n) because all remaining elements shift left. `std::queue` (deque-backed) `pop()` is O(1). For a tree with n nodes, using a vector with front-erase turns BFS from O(n) to O(n²). Always use `std::queue` (or `std::deque`) for BFS queues.

**Note It Down:** The BFS template has two loops. The outer while loop runs once per level. The inner for loop runs once per node at that level. If you need per-level results (like a vector of vectors), collect nodes inside the inner loop and push the vector after the inner loop.

**Interview Relevance:** The level-size trick is the single most important BFS implementation detail. Without it, you cannot separate nodes by level, which makes level-specific operations (zigzag, level averages, right side view) impossible.

---

### **11: Problem Walkthrough, Binary Tree Level Order Traversal**

**Problem: Binary Tree Level Order Traversal (LeetCode 102)**

Given the root of a binary tree, return the level order traversal as a vector of vectors.

```
        3
       / \
      9   20
         / \
        15   7
```

Expected: `{{3}, {9, 20}, {15, 7}}`

**Trace Using the BFS Template:**

```
queue = [3], result = []

Level 1: levelSize = 1
  pop 3, add children 9, 20
  currentLevel = [3]
  queue = [9, 20]
  result = [[3]]

Level 2: levelSize = 2
  pop 9, no children
  pop 20, add children 15, 7
  currentLevel = [9, 20]
  queue = [15, 7]
  result = [[3], [9, 20]]

Level 3: levelSize = 2
  pop 15, no children
  pop 7, no children
  currentLevel = [15, 7]
  queue = []
  result = [[3], [9, 20], [15, 7]]

Queue empty → return result
```

**Common Variants That Use the Same Template:**

|Variant|What Changes|
|---|---|
|Level Order Traversal|Collect all values per level|
|Zigzag Level Order|Reverse every other level’s vector|
|Binary Tree Right Side View|Take only the last node of each level|
|Average of Levels|Compute mean instead of collecting values|
|Largest Value in Each Row|Take max instead of collecting values|

Every variant uses the same BFS template from section 10. The only thing that changes is what you do with `currentLevel` after the inner loop.

**Interview Relevance:** Level Order Traversal is a top-20 most-asked tree problem. Once you know the template, all five variants above become trivial modifications, which is exactly the pattern-based thinking this course teaches.

---

### **12: BST Invariants and Properties**

**What Makes a Binary Search Tree Special:**

For every node in a BST:

- All values in the left subtree are strictly less than the node’s value
- All values in the right subtree are strictly greater than the node’s value
- Both the left and right subtrees are also BSTs

**Key Properties That Follow:**

|Property|Why It Is True|Use Case|
|---|---|---|
|Inorder traversal gives sorted order|Left (smaller) is visited first, then node, then right (larger)|Validate BST, find kth smallest|
|Search is O(h) where h is height|At each node, you go left or right, eliminating one subtree|Lookup, insert, delete|
|Minimum is the leftmost node|Keep going left until nullptr|Find min in O(h)|
|Maximum is the rightmost node|Keep going right until nullptr|Find max in O(h)|
|Successor is the leftmost node in the right subtree|Next larger value is the smallest in the right subtree|Inorder successor|

**BST Search:**

```cpp
TreeNode* searchBST(TreeNode* root, int target) {
    if (!root) return nullptr;
    if (target == root->val) return root;
    else if (target < root->val) return searchBST(root->left, target);
    else return searchBST(root->right, target);
}
```

This is binary search on a tree structure. At each node, you eliminate one subtree entirely. The same O(log n) principle from binary search in the previous course, applied to a branching structure instead of an array.

**Interview Relevance:** BST properties are assumed knowledge in interviews. If the problem says “binary search tree,” every property above is available to exploit. Missing the sorted-inorder property in a BST problem is one of the most common mistakes.

---

### **13: Problem Walkthrough, Validate Binary Search Tree**

**Problem: Validate Binary Search Tree (LeetCode 98)**

Given the root of a binary tree, determine if it is a valid BST.

**The Naive (Wrong) Approach:**  
Check that each node’s left child is smaller and right child is larger.

```
        5
       / \
      1   6
         / \
        3   7
```

This tree passes the naive check (1 < 5, 6 > 5, 3 < 6, 7 > 6) but is NOT a valid BST. Node 3 is in the right subtree of 5, so it must be greater than 5. But 3 < 5. The naive approach only checks the immediate parent, not all ancestors.

**The Correct Approach: Range-Based Validation**  
Each node has a valid range. The root can be anything. The left child must be less than the parent. The right child must be greater. These constraints propagate downward, narrowing the range at each level.

```cpp
class Solution {

private:

	bool validate(TreeNode* root, long long minn, long long maxx){
	
	if(!root) return true;
	
	if(root->val <= minn || root->val >= maxx) return false;
	
	return validate(root->left, minn, root->val) && validate(root->right, root->val, maxx);
	
	}

public:

	bool isValidBST(TreeNode* root) {
	
	return validate(root, LLONG_MIN, LLONG_MAX);
	
	}

};
```

Use `long long` bounds so that a node valued `INT_MIN` or `INT_MAX` at the root is still valid.

Time: O(n). Space: O(h) for the recursion stack.

**Interview Relevance:** Validate BST is a top-15 most-asked tree problem. The naive “check only the parent” approach is the most common wrong answer. Interviewers use this problem specifically to test whether you understand that BST constraints apply to the entire subtree, not just the immediate children.

---

### **13B: BST Validation, Range-Based Approach**

**Trace:**

```
        5
       / \
      1   6
         / \
        3   7

validate(5, -inf, +inf):
  5 in (-inf, +inf) ✓
  validate(1, -inf, 5):
    1 in (-inf, 5) ✓
    validate(nullptr) = true
    validate(nullptr) = true
    return true
  validate(6, 5, +inf):
    6 in (5, +inf) ✓
    validate(3, 5, 6):
      3 in (5, 6)? NO → 3 <= 5 → return false ✗
    return false
  return false
```

Node 3 fails because its valid range is (5, 6), but 3 is not in that range. The range correctly captures the constraint that everything in the right subtree of 5 must be greater than 5.

**How the Range Propagates:**

|Node|Valid Range|Passed?|
|---|---|---|
|5 (root)|(-inf, +inf)|Yes|
|1 (left of 5)|(-inf, 5)|Yes|
|6 (right of 5)|(5, +inf)|Yes|
|3 (left of 6)|(5, 6)|No, 3 is not > 5|
|7 (right of 6)|(6, +inf)|Yes|

**Alternative: Inorder Traversal Approach**  
Since inorder traversal of a valid BST produces sorted values, you can do an inorder traversal and check that each value is strictly greater than the previous.

```cpp
bool isValidBSTInorder(TreeNode* root) {
    long long prev = numeric_limits<long long>::min();
    function<bool(TreeNode*)> inorder = [&](TreeNode* node) -> bool {
        if (!node) return true;
        if (!inorder(node->left)) return false;
        if (node->val <= prev) return false;
        prev = node->val;
        return inorder(node->right);
    };
    return inorder(root);
}
```

Both approaches are O(n) time and O(h) space. The range approach is more commonly expected in interviews.

**Interview Relevance:** Being able to discuss both the range-based and inorder approaches shows flexibility. If the interviewer asks for a follow-up, pivoting from one approach to the other demonstrates depth.

---

### **14: Interview Practice, BFS and BST Patterns**

**Test yourself. Identify the approach before reading the answer.**

**Problem 1:**  
Given a binary tree, return the values of nodes visible from the right side (rightmost node at each level).

What approach applies? DFS or BFS?

Answer: BFS with the level order template (section 10). At each level, the last node in `currentLevel` is the right-side visible node. Alternatively, DFS with right-child-first traversal tracking the first node seen at each depth. BFS is more intuitive here.

**Problem 2:**  
Given a BST and two values p and q, find their lowest common ancestor. Both values are guaranteed to exist.

What approach applies?

Answer: Exploit the BST property. If both p and q are less than the current node, the LCA is in the left subtree. If both are greater, the LCA is in the right subtree. If they split (one on each side), the current node is the LCA. Time: O(h). This is simpler than the general binary tree LCA on section 17 because the BST ordering tells you which subtree to search.

```C++
class Solution {

public:
	
	TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
	
	if(!root || root==p || root==q) {
	return root;
	}
	
	TreeNode* left = lowestCommonAncestor(root->left, p, q);
	TreeNode* right = lowestCommonAncestor(root->right, p, q);
	
	if(!left) return right;
	
	if(!right) return left;
	
	else return root;
	}
};
```

**Problem 3:**  
Given a BST, find the kth smallest element.

What approach applies?

Answer: Inorder traversal (section 3). Since inorder on a BST gives sorted order, the kth element in the inorder sequence is the kth smallest. Use either recursive or iterative inorder and count nodes until you reach k.

```C++
class Solution {

public:

	vector<int> tree;
	
	void inorderTraversal(TreeNode* root){
	if(!root) return;
	inorderTraversal(root->left);
	tree.push_back(root->val);
	inorderTraversal(root->right);
	}
	
	int kthSmallest(TreeNode* root, int k) {
	inorderTraversal(root);
	return tree[k-1];
	}

};
```

**Interview Relevance:** These three problems demonstrate three different uses of BST properties: level structure (BFS), ordering (ancestor search), and sorted traversal (inorder). Recognizing which property to exploit is the key skill.

---

### **15: Tree Path Problems**

**Recognition Signal:**  
The problem mentions a path from one node to another, or asks about sums/properties along a root-to-leaf path, or requires finding a specific path through the tree.

**Types of Tree Path Problems:**

|Type|Description|Example|
|---|---|---|
|Root-to-leaf path|Path starts at root and ends at a leaf|Path Sum, Root-to-Leaf Numbers|
|Any-to-any path|Path between any two nodes (through their LCA)|Diameter, Binary Tree Maximum Path Sum|
|Path with target sum|Find a path where node values sum to a target|Path Sum, Path Sum II, Path Sum III|
|Longest/shortest path|Find the path with maximum or minimum length|Diameter, Minimum Depth|

**How Path Problems Map to Recursion:**

For root-to-leaf paths, pass the running sum (or path) downward as a parameter. Check the condition at each leaf.

For any-to-any paths, return information upward (like height or partial sum) and compute the path metric at each node by combining left and right results.

**The Decision:**

|Information flows…|Direction|Mechanism|
|---|---|---|
|Downward (running sum, constraints, path so far)|Root to children|Function parameters|
|Upward (height, subtree sum, whether target was found)|Children to root|Return values|
|Both (need running sum AND subtree info)|Both directions|Parameters down + return up|

**Interview Relevance:** Path problems are among the most common medium and hard tree questions. The key is deciding whether information needs to flow down, up, or both. This decision determines the entire structure of your recursive solution.

---

### **16: Problem Walkthrough, Path Sum**

**Problem: Path Sum (LeetCode 112)**

Given the root of a binary tree and a target sum, determine if the tree has a root-to-leaf path where all values sum to the target.

Given: target = 22

```
        5
       / \
      4   8
     /   / \
    11  13   4
   / \        \
  7   2        1
```

Path 5 -> 4 -> 11 -> 2 = 22. Return true.

**Approach: Pass Remaining Sum Downward**

```cpp
bool hasPathSum(TreeNode* root, int targetSum) {
    if (!root) return false;
    targetSum -= root->val;
    if (!root->left && !root->right) return targetSum == 0;
    return hasPathSum(root->left, targetSum) ||
           hasPathSum(root->right, targetSum);
}
```

**Trace (abbreviated):**

```
hasPathSum(5, 22): remaining = 17
  hasPathSum(4, 17): remaining = 13
    hasPathSum(11, 13): remaining = 2
      hasPathSum(7, 2): remaining = -5
        leaf, -5 != 0 → false
      hasPathSum(2, 2): remaining = 0
        leaf, 0 == 0 → true ✓
      return true
    return true
  return true
```

**Why Check for Leaf Nodes:**  
A leaf is a node with no children. The check `!root->left && !root->right` ensures the path ends at a leaf, not at an internal node. Without this check, a path stopping at an internal node could falsely match.

**Common Mistake:**  
Checking `if (!root) return targetSum == 0;`. This triggers on null children of leaf nodes, which is incorrect. A null node is not a leaf. A leaf is a node that exists but has no children. The check must be on the node itself, not on null.

**Interview Relevance:** Path Sum is a common easy-level tree problem. The leaf-check edge case is the main thing interviewers evaluate. Getting it wrong is a common source of bugs.

---

### **17: Problem Walkthrough, Lowest Common Ancestor**

**Problem: Lowest Common Ancestor of a Binary Tree (LeetCode 236)**

Given a binary tree and two nodes p and q, find their lowest common ancestor (LCA). The LCA is the deepest node that has both p and q as descendants (a node can be a descendant of itself).

**The Key Insight:**  
Search both subtrees. If p is found in the left subtree and q is found in the right subtree (or vice versa), the current node is the LCA. If both are found in the same subtree, the LCA is deeper in that subtree.

```cpp
TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
    if (!root || root == p || root == q) return root;
    TreeNode* left = lowestCommonAncestor(root->left, p, q);
    TreeNode* right = lowestCommonAncestor(root->right, p, q);
    if (left && right) return root;
    return left ? left : right;
}
```

**How It Works:**

|Scenario|Left Result|Right Result|Return|
|---|---|---|---|
|p in left, q in right|p (or LCA)|q (or LCA)|Current node is LCA|
|Both in left subtree|LCA from left|nullptr|Pass left result upward|
|Both in right subtree|nullptr|LCA from right|Pass right result upward|
|Current node is p or q|Return self|Recurse other subtree|Self is LCA if other is found below|

Time: O(n). Space: O(h).

**Interview Relevance:** Lowest Common Ancestor is a top-10 most-asked tree problem. It tests the “return upward and combine” pattern at its most elegant. The solution is only 8 lines but requires understanding how recursive results propagate upward and combine at each node.

---

### **17B: Lowest Common Ancestor, Tracing Paths Through the Tree**

**Trace:**

```
        3
       / \
      5   1
     / \ / \
    6  2 0  8
      / \
     7   4

Find LCA of 5 and 1:

lca(3, 5, 1):
  lca(5, 5, 1):
    root == p → return 5
  left = 5

  lca(1, 5, 1):
    root == q → return 1
  right = 1

  left=5, right=1 → both non-null → return 3

LCA(5, 1) = 3
```

**Trace for LCA of 5 and 4:**

```
lca(3, 5, 4):
  lca(5, 5, 4):
    root == p → return 5
  left = 5

  lca(1, 5, 4):
    lca(0, 5, 4) → returns nullptr
    lca(8, 5, 4) → returns nullptr
    left=nullptr, right=nullptr → return nullptr
  right = nullptr

  left=5, right=nullptr → return left = 5

LCA(5, 4) = 5 (node 5 is an ancestor of 4 and also itself)
```

**Why `root == p || root == q` Returns Early:**  
When the current node is one of the targets, return it immediately. The other target must be either in this node’s subtree (making this node the LCA) or in a different branch (and the parent will combine the results). Either way, returning early is correct.

**Common Mistake:**  
Not handling the case where p is an ancestor of q (or vice versa). The early return on `root == p || root == q` handles this case. Without it, the recursion would search the subtree of p for q, find q, and return q, but the parent would get only one non-null result and return it. The early return ensures the ancestor itself is returned as the LCA.

**Interview Relevance:** The LCA trace is a common whiteboard exercise. Interviewers ask you to walk through the recursion step by step to verify you understand how the base cases and recursive combination produce the correct answer.

---

### **18: DFS vs BFS Decision Framework for Trees**

**When to Use DFS:**

|Signal|Why DFS|Example|
|---|---|---|
|“Height” or “depth” of the tree|Height requires going all the way to leaves|Maximum Depth|
|“Path from root to leaf”|DFS naturally follows root-to-leaf paths|Path Sum|
|“Validate a property of subtrees”|Need subtree results to validate parent|Validate BST|
|“Diameter” or “longest path”|Need heights from left and right subtrees|Diameter|
|“Serialize/deserialize”|Preorder DFS gives a natural serialization|Serialize Binary Tree|
|“Compare two trees”|Structural comparison is naturally recursive|Same Tree, Symmetric Tree|

**When to Use BFS:**

|Signal|Why BFS|Example|
|---|---|---|
|“Level by level”|BFS processes one level at a time|Level Order Traversal|
|“Shortest path”|BFS finds shortest path in unweighted graphs/trees|Minimum Depth|
|“Right side view” or “left side view”|Last/first node at each level|Right Side View|
|“Zigzag” or “spiral” order|Requires level-aware processing|Zigzag Level Order|
|“Connect nodes at same level”|Level grouping needed|Populating Next Right Pointers|

**The Default:**  
When neither BFS nor DFS is clearly better, default to DFS. DFS is simpler to write (fewer lines, no queue), uses O(h) space vs O(w) for BFS (where w is the maximum width), and handles the majority of tree problems.

**Interview Relevance:** Choosing the wrong traversal does not mean you cannot solve the problem, but it makes the solution harder and signals to the interviewer that you did not recognize the problem structure. Spending 10 seconds on this decision saves minutes of implementation.

---

### **19: Common Mistakes and Interview Tips**

**Common Interview Mistakes in This Section:**

|Mistake|Why It Fails|Better Approach|
|---|---|---|
|Confusing local and global state in tree recursion|Capturing a tracker by value instead of by reference, so each subtree updates its own copy|Return values upward for per-subtree results; capture trackers with `[&]` or use a class member|
|Not defining the base case clearly|Missing the `if (!node) return ...` base case causes a null-pointer dereference|Every tree recursive function starts with `if (!node)` returning a default value|
|Checking only immediate children for BST validation|Misses the constraint that all nodes in a subtree must satisfy the BST property|Use the range-based approach: pass valid (low, high) bounds downward|
|Using a `vector` with `erase(v.begin())` for BFS queue|O(n) per pop turns O(n) BFS into O(n²)|Use `std::queue` with `pop()` for O(1)|
|Not handling the “node is descendant of itself” case in LCA|Misses the case where one target is an ancestor of the other|The `root == p || root == q` early return handles this automatically|

**Interview Tip:** Before writing any tree recursion, answer these two questions out loud: “What does each recursive call return?” and “What is the base case when the node is nullptr?” If you can answer both, the code writes itself. If you cannot, step back and think before coding.

**Interview Relevance:** Tree problems have a high pass rate among well-prepared candidates because the patterns are consistent. The mistakes above are the primary reasons candidates fail tree problems despite knowing the patterns. Eliminating them makes tree questions nearly free points.

---

### **20: Section Cheat Sheet**

**Tree Patterns, Quick Reference**

**Pattern Templates:**

```
DFS (POSTORDER, returning upward):
    int dfs(TreeNode* node) {
        if (!node) return base_value;
        int left = dfs(node->left);
        int right = dfs(node->right);
        return combine(left, right, node->val);
    }

BFS (LEVEL ORDER):
    queue<TreeNode*> q;
    q.push(root);
    while (!q.empty()) {
        int levelSize = q.size();
        for (int i = 0; i < levelSize; i++) {
            TreeNode* node = q.front(); q.pop();
            process(node);
            if (node->left) q.push(node->left);
            if (node->right) q.push(node->right);
        }
    }

BST VALIDATION (range-based):
    bool validate(TreeNode* node, long long low, long long high) {
        if (!node) return true;
        if (node->val <= low || node->val >= high) return false;
        return validate(node->left, low, node->val) &&
               validate(node->right, node->val, high);
    }

LOWEST COMMON ANCESTOR:
    if (!root || root == p || root == q) return root;
    TreeNode* left = lca(root->left, p, q);
    TreeNode* right = lca(root->right, p, q);
    if (left && right) return root;
    return left ? left : right;

PATH SUM (pass remaining downward):
    remaining -= node->val;
    if (leaf) return remaining == 0;
    return dfs(left, remaining) || dfs(right, remaining);
```

**Key Complexities:**

|Pattern|Time|Space|
|---|---|---|
|DFS (any order)|O(n)|O(h) recursion stack|
|BFS (level order)|O(n)|O(w) queue width|
|BST search|O(h)|O(h) recursion stack|
|BST validation|O(n)|O(h)|
|LCA|O(n)|O(h)|

**Interview Relevance:** h is O(log n) for balanced trees and O(n) for skewed trees. Stating this distinction when discussing space complexity shows you understand the difference between best-case and worst-case tree structure.

---

### **21: Section Summary and Pattern Recognition Signals**

**What We Covered:**

|Pattern|Recognition Signal|Key Insight|
|---|---|---|
|Recursive DFS|Need to explore full paths or compute subtree properties|Recurse left and right, combine results, return upward|
|Return upward|Each node needs information from its children|Return value serves the parent, answer may be tracked separately|
|Iterative DFS|Need explicit control or tree is very deep|Use a `std::stack` instead of the call stack|
|BFS level order|Problem mentions levels, layers, or shortest depth|Queue with levelSize trick to separate levels|
|BST properties|Tree is a BST, need sorted order or search|Inorder gives sorted values, search is O(h)|
|BST validation|Verify the BST invariant|Range-based: pass (low, high) bounds downward|
|Path sum|Root-to-leaf path with a target|Subtract node value from target, check at leaves|
|LCA|Find deepest common ancestor of two nodes|Search both subtrees, combine results at each node|

**How This Connects to Other Sections:**

|This Section’s Pattern|Where It Reappears Later|
|---|---|
|DFS recursion|Graph DFS uses the same recursive structure with a visited set|
|BFS with queue|Graph BFS is the same template plus a visited set|
|BST binary search|BST search is binary search applied to a tree structure|
|Return upward pattern|Backtracking returns results upward while exploring decision trees|
|Path problems|Some DP problems on trees use similar “pass down, return up” structures|

**What Comes Next:**  
In the next section, we cover Graph Patterns. Trees are a special case of graphs: they are connected, acyclic, and have a single root. Graphs remove all three constraints. They can be disconnected, have cycles, and have no root. This means we need a visited set to avoid infinite loops and must handle multiple connected components.

**Interview Relevance:** Tree problems account for roughly 15% of coding interview questions, making them the second most common category after arrays. Mastering these patterns gives you a strong foundation for the graph patterns that follow.
