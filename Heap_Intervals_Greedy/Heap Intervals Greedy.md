# **Section 3: Heap, Intervals, and Greedy**

### **1: Section Intro, Heap, Intervals, and Greedy**

This section covers three patterns that share a common theme: making the best decision at each step. Heaps give you the best candidate right now (smallest or largest) without sorting the entire collection. Interval problems ask you to reason about overlapping or conflicting time ranges, and the key is always sorting first. Greedy algorithms prove that choosing the locally optimal option at each step leads to the globally optimal result.

These three patterns are grouped together because they frequently combine. Interval problems often use sorting plus a greedy selection strategy. Heap problems often use a greedy "take the best next candidate" approach. Understanding how they interact is as important as knowing each pattern individually.

**What We'll Cover:**

1. Min heap and max heap fundamentals
2. Top K pattern
3. K-way merge pattern
4. Interval sorting and overlap detection
5. Greedy algorithms and correctness proofs
6. Decision framework: heap vs sort vs greedy

**Where This Appears in Interviews:**

| Company Type       | How This Section Appears                                                           |
| ------------------ | ---------------------------------------------------------------------------------- |
| FAANG / Big Tech   | Top K Frequent Elements, Merge K Sorted Lists, and Merge Intervals are top-20 most-asked |
| Mid-size companies | Meeting Rooms and interval scheduling are common phone screen questions            |
| Startups           | Priority queue usage for task scheduling, event processing                         |
| Online assessments | Interval problems and Top K are common timed questions                             |

**Core Idea for This Section:**  
Use heaps when you repeatedly need the best next candidate. Use interval sorting when the problem is about overlap. Use greedy when a local choice can be justified globally.

---

### **2: Min Heap and Max Heap Fundamentals**

**What a Heap Is:**  
A heap is a data structure that gives you the smallest (min heap) or largest (max heap) element in O(1) and supports insertion and removal in O(log n). It is the right tool when you need repeated access to the extreme value in a changing collection.

**C++ `std::priority_queue`:**  
Unlike Python, C++ provides both heaps natively. The default `priority_queue` is a **max heap**; add `greater<int>` to get a min heap. No value negation tricks needed.

```cpp
#include <queue>
#include <vector>
using namespace std;

// Min heap: smallest element on top
priority_queue<int, vector<int>, greater<int>> minHeap;
minHeap.push(5);
minHeap.push(2);
minHeap.push(8);
int smallest = minHeap.top(); minHeap.pop();   // 2
int peek = minHeap.top();                      // O(1) peek, does not remove

// Max heap: largest element on top (C++ default)
priority_queue<int> maxHeap;
maxHeap.push(5);
maxHeap.push(2);
maxHeap.push(8);
int largest = maxHeap.top(); maxHeap.pop();    // 8
```

**Key Operations and Complexities:**

| Operation                                   | Time     | What It Does                                        |
| ------------------------------------------- | -------- | --------------------------------------------------- |
| `push(x)`                                   | O(log n) | Insert an element                                   |
| `pop()`                                     | O(log n) | Remove the root (smallest or largest)               |
| `top()`                                     | O(1)     | Peek at the root without removing it                |
| `priority_queue(first, last)` / `make_heap` | O(n)     | Convert a range into a heap in-place                |
| `pop()` then `push(x)`                      | O(log n) | C++ has no `heappushpop`; two separate O(log n) ops |

**Custom orderings:** For pairs or tuples, the heap compares lexicographically (first element, then second, …) just like Python tuples. To order by a secondary field, put it first in the pair — or supply a custom comparator.

**When to Use a Heap vs Sorting:**

| Scenario                                      | Heap                          | Sort                                |
| --------------------------------------------- | ----------------------------- | ----------------------------------- |
| Need all elements in order                    | —                             | Sort once: O(n log n), better       |
| Need only the top K elements                  | Heap of size K: O(n log k)    | Heap is better when k << n          |
| Elements arrive one at a time (streaming)     | Heap: O(log n) per insertion  | Cannot sort a stream                |
| Need repeated min/max after modifications     | Heap: O(log n) per operation  | Re-sorting is O(n log n) each time  |

Heaps appear whenever you see "top K," "kth largest/smallest," "median of a stream," or "merge K sorted." Recognizing these signals and reaching for a heap instead of sorting is the key skill.

**Interview Relevance:** State the operation you need ("I repeatedly need the smallest of the elements I'm keeping") and the heap choice follows. Knowing the O(1) `top()` vs O(log n) `push`/`pop` costs lets you justify the complexity in one sentence.

---

### **3: Top K Pattern**

**Recognition Signal:**  
The problem asks for the K largest, K smallest, K most frequent, or Kth ranked element. You do not need all elements sorted, just the top K.

**Two Approaches:**

**Approach 1: Min Heap of Size K (for K largest)**  
Maintain a min heap of size K. For each element, if it is larger than the heap's minimum, replace the minimum. After processing all elements, the heap contains the K largest.

```cpp
vector<int> topKLargest(vector<int>& nums, int k) {
    // 1. Heapify first K elements in O(k): min heap of the K largest so far
    priority_queue<int, vector<int>, greater<int>> heap(nums.begin(), nums.begin() + k);
    
    // 2. Process the rest of the array
    for (int i = k; i < (int)nums.size(); i++) {
        if (nums[i] > heap.top()) {
            heap.pop();
            heap.push(nums[i]);   
        }
    }
    
    // 3. Manually pop elements into a vector
    vector<int> result;
    while (!heap.empty()) {
        result.push_back(heap.top());
        heap.pop();
    }
    
    return result; 
}
```

Time: O(n log k). Space: O(k).

**Approach 2: Quickselect for Kth Element**  
Partition the array around a pivot (like quicksort). If the pivot lands at position k, you are done. Average time: O(n). Worst case: O(n²).

**Why Min Heap for K Largest (Not Max Heap):**  
A min heap of size K keeps the smallest of the K largest at the top. Any new element larger than this minimum deserves to be in the top K, so it replaces the minimum. The heap always contains exactly the K largest elements seen so far. (A max heap of size K would keep the *largest* of the kept elements on top — useless for deciding whether a new element belongs.)

**Interview Tip:** For "top K" problems, always mention both approaches: heap (O(n log k), guaranteed) and quickselect (O(n) average, O(n²) worst). Interviewers appreciate seeing you weigh trade-offs.

Top K is one of the most common heap patterns. It appears directly in Top K Frequent Elements and Kth Largest Element, and indirectly in many streaming and ranking problems.

**Interview Relevance:** The one-line justification interviewers listen for: "a min heap of size K gives me the threshold — anything bigger than the root replaces it — so I never sort the whole array."

---

### **4: Problem Walkthrough, Top K Frequent Elements**

**Problem: Top K Frequent Elements (LeetCode 347)**

Given an integer array `nums` and an integer `k`, return the k most frequent elements.

Given: `nums = [1,1,1,2,2,3], k = 2`  
Expected: `[1, 2]`

Step 1: Build a frequency map in O(n).  
Step 2: Use a min heap of size k to find the k most frequent.

**Approach: Frequency Map + Heap**

```cpp
class Solution {
public:

	vector<int> topKFrequent(vector<int>& nums, int k) {
		unordered_map<int, int> mp;
		for (int &num : nums) {
		mp[num]++;
	}
	
	priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> heap;
	
	for (auto [key, value] : mp) {
		if (heap.size() < k) {
		heap.push({value, key});
		}
		else {
			if (heap.top().first < value) {
				heap.pop();
				heap.push({value, key});
			}
		}
	}
	
	vector<int> ans;
	while (!heap.empty()) {
		ans.push_back(heap.top().second);
		heap.pop();
	}
	return ans;
	}

};
```

Time: O(n log k). Space: O(n) for the frequency map.

Pairs compare lexicographically in both Python and C++, so `(count, num)` with a min heap always exposes the smallest count — exactly like the Python tuple version.

**Alternative: Bucket Sort Approach (O(n))**

```cpp
vector<int> topKFrequentBucket(vector<int>& nums, int k) {
    unordered_map<int, int> freq;
    for (int num : nums) freq[num]++;

    vector<vector<int>> buckets(nums.size() + 1);   // index = count
    for (auto& [num, count] : freq) buckets[count].push_back(num);

    vector<int> result;
    for (int i = (int)buckets.size() - 1; i >= 0 && (int)result.size() < k; i--) {
        for (int num : buckets[i]) {
            result.push_back(num);
            if ((int)result.size() == k) return result;
        }
    }
    return result;
}
```

Time: O(n). Space: O(n). This is faster but uses more space and is harder to write under pressure.

Top K Frequent Elements is a top-15 most-asked problem. Knowing both the heap approach and the bucket sort approach, and being able to discuss the trade-offs, is a strong signal.

**Interview Relevance:** The bucket sort follow-up ("can you do it in O(n)?") is common. Answering it on the spot — bucket by count, scan buckets from the end — shows you understand why frequencies are bounded by n.

---

### **5: Problem Walkthrough, Kth Largest Element in an Array**

**Problem: Kth Largest Element in an Array (LeetCode 215)**

Given an integer array `nums` and an integer `k`, return the kth largest element (not the kth distinct element).

Given: `nums = [3,2,1,5,6,4], k = 2`  
Expected: `5`

**Approach 1: Min Heap of Size K**

```cpp
int findKthLargest(vector<int>& nums, int k) {
    priority_queue<int, vector<int>, greater<int>> heap(nums.begin(), nums.begin() + k);
    for (int i = k; i < nums.size(); i++) {
        if (nums[i] > heap.top()) {
            heap.pop();
            heap.push(nums[i]);
        }
    }
    return heap.top();
}
```

Time: O(n log k). Space: O(k).

The heap contains the k largest elements. The smallest of those (the heap's root) is the kth largest overall.

**Approach 2: Quickselect**

```cpp
#include <cstdlib>  // rand

int findKthLargestQS(vector<int>& nums, int k) {
    int target = (int)nums.size() - k;   // kth largest == (n-k)th smallest

    function<int(int, int)> quickselect = [&](int left, int right) -> int {
        int pivotIdx = left + rand() % (right - left + 1);   // random pivot
        swap(nums[pivotIdx], nums[right]);
        int store = left;
        for (int i = left; i < right; i++) {
            if (nums[i] < nums[right]) {
                swap(nums[i], nums[store]);
                store++;
            }
        }
        swap(nums[store], nums[right]);   // pivot in final position

        if (store == target) return nums[store];
        else if (store < target) return quickselect(store + 1, right);
        else return quickselect(left, store - 1);
    };
    return quickselect(0, (int)nums.size() - 1);
}
```

Time: O(n) average, O(n²) worst case. Space: O(1) extra (in-place), plus O(n) recursion depth worst case.

**Comparison:**

| Approach     | Time           | Space | Guarantee            |
| ------------ | -------------- | ----- | -------------------- |
| Min heap     | O(n log k)     | O(k)  | Worst case guaranteed |
| Sort         | O(n log n)     | O(n)  | Worst case guaranteed |
| Quickselect  | O(n) average   | O(1)  | O(n²) worst case     |

Kth Largest is a top-20 most-asked problem. Start with the heap approach (safer, easier to write), then mention quickselect as an optimization. The randomized pivot makes the worst case extremely unlikely in practice.

**Interview Relevance:** The `target = n - k` translation (kth largest → index in sorted order) is the first thing to state. Getting that mapping right keeps the partition logic clean.

**[Kth Largest Element in a Stream](https://leetcode.com/problems/kth-largest-element-in-a-stream/)**

```C++
class KthLargest {

public:
	priority_queue<int, vector<int>, greater<int>> heap;
	int K;
	
	KthLargest(int k, vector<int>& nums) {
		K = k;
		for (int i=0; i<nums.size(); i++) {
		add(nums[i]);
		}
	}
	
	int add(int val) {
		if (heap.size() < K) {
			heap.push(val);
		}
		else if (val > heap.top()) {
		heap.pop();
		heap.push(val);
		}
		return heap.top();
	}

};
```

---

### **6: K-Way Merge Pattern**

**Recognition Signal:**  
The problem gives you K sorted sequences (lists, arrays, linked lists, or streams) and asks you to merge them into one sorted sequence, or to find the kth element across all sequences.

**How It Works:**  
Use a min heap to always know which sequence has the smallest current element. Pop the smallest, add it to the result, and push the next element from that same sequence.

**Template:**

```cpp
vector<int> kWayMerge(const vector<vector<int>>& sortedLists) {
    using Entry = tuple<int, int, int>;   // (value, list_index, element_index)
    priority_queue<Entry, vector<Entry>, greater<Entry>> heap;

    for (int i = 0; i < (int)sortedLists.size(); i++) {
        if (!sortedLists[i].empty()) heap.push({sortedLists[i][0], i, 0});
    }

    vector<int> result;
    while (!heap.empty()) {
        auto [val, listIdx, elemIdx] = heap.top(); heap.pop();
        result.push_back(val);
        if (elemIdx + 1 < (int)sortedLists[listIdx].size()) {
            heap.push({sortedLists[listIdx][elemIdx + 1], listIdx, elemIdx + 1});
        }
    }
    return result;
}
```

Time: O(N log K) where N is the total number of elements across all lists and K is the number of lists. Each of the N elements is pushed and popped from a heap of size K.

**Why the Heap Has Three Values:**  
The tuple `(value, list_index, element_index)` stores the value (for ordering), which list it came from (to push the next element from that list), and the position within that list (to know what comes next). `std::tuple` compares lexicographically, the same way Python tuples do — value first, then list index as a tiebreaker.

K-Way Merge is the pattern behind Merge K Sorted Lists, a top-25 most-asked problem. It also appears in external sort algorithms and database merge operations, making it a pattern with real-world application.

**Interview Relevance:** The three-value tuple is the whole trick. Value orders the heap, list index breaks ties, element index tells you what to push next. Say those three roles out loud and the template writes itself.

---

### **7: Problem Walkthrough, Merge K Sorted Lists**

**Problem: Merge K Sorted Lists (LeetCode 23)**

Given an array of k sorted linked lists, merge all lists into one sorted linked list.

```cpp
class Solution {
public:
    ListNode* mergeKLists(vector<ListNode*>& lists) {
        // min heap of (value, list_index, node*)
        using Entry = tuple<int, int, ListNode*>;
        priority_queue<Entry, vector<Entry>, greater<Entry>> heap;

        for (int i = 0; i < (int)lists.size(); i++) {
            if (lists[i]) heap.push({lists[i]->val, i, lists[i]});
        }

        ListNode dummy(0);
        ListNode* current = &dummy;
        while (!heap.empty()) {
            auto [val, i, node] = heap.top(); heap.pop();
            current->next = node;
            current = current->next;
            if (node->next) heap.push({node->next->val, i, node->next});
        }
        return dummy.next;
    }
};
```

**Why the List Index `i` Is in the Tuple:**  
`std::tuple` compares element by element, just like Python tuples. If two nodes have the same value, comparison would fall through to the `ListNode*` — and ordering unrelated pointers is unspecified and meaningless. Including the unique list index as a tiebreaker keeps every comparison well-defined. (Only one node per list is ever in the heap, so with the index present, the pointer is never compared at all.)

**Trace (abbreviated):**

```
lists = [1->4->5, 1->3->4, 2->6]

heap = [(1,0,node), (1,1,node), (2,2,node)]
Pop (1,0,node1): result = 1, push (4,0,node4)
Pop (1,1,node1): result = 1->1, push (3,1,node3)
Pop (2,2,node2): result = 1->1->2, push (6,2,node6)
Pop (3,1,node3): result = 1->1->2->3, push (4,1,node4)
Pop (4,0,node4): result = 1->1->2->3->4, push (5,0,node5)
Pop (4,1,node4): result = 1->1->2->3->4->4, no next
Pop (5,0,node5): result = ..->5, no next
Pop (6,2,node6): result = ..->6, no next

Final: 1->1->2->3->4->4->5->6
```

Time: O(N log K). Space: O(K) for the heap.

Merge K Sorted Lists is a hard-rated problem but straightforward once you know the K-way merge pattern. It combines the heap pattern from this section with the linked list merge pattern covered in the previous course.

**Interview Relevance:** The dummy-node trick and the index tiebreaker are the two details interviewers check. Everything else is the K-way merge template applied to pointers instead of array positions.

---

### **8: Interview Practice, Heap Patterns**

**Test yourself. Identify the heap variant before reading the answer.**

**Problem 1:**  
Given a list of points on a 2D plane, find the K closest points to the origin.

What pattern applies?

Answer: Top K pattern (section 3). Use a max heap of size K (C++'s default `priority_queue` is already a max heap — keep the K *smallest* distances, with the largest of them on top). For each point, compute the squared distance (no need for square root since we only compare). If the new distance is smaller than the max in the heap, replace it. Time: O(n log k).

**Problem 2:**  
Given a stream of integers, find the median after each insertion.

What pattern applies?

Answer: Two heaps. Maintain a max heap (default `priority_queue`) for the lower half and a min heap (`priority_queue<int, vector<int>, greater<int>>`) for the upper half. The median is the top of the max heap (if odd count) or the average of both tops (if even count). Each insertion is O(log n). This is the "Find Median from Data Stream" problem.

**Problem 3:**  
Given K sorted arrays, find the kth smallest element across all arrays.

What pattern applies?

Answer: K-way merge (section 6), but stop after popping k elements instead of merging everything. Time: O(k log K). No need to process all N elements.

Every heap problem follows the same structure: maintain a heap of bounded size, push and pop as you process elements, and the heap's root is always the current answer. What changes between problems is what you push (values, distances, frequencies) and the heap's size constraint.

**Key Insight:** These three problems cover the three main heap use cases: top K, streaming median, and k-way merge. Recognizing which variant applies from the problem description is the critical skill.

**Interview Relevance:** Name the variant before writing code. "This is a two-heaps problem" or "this is k-way merge" tells the interviewer you have the pattern, and the implementation becomes mechanical.

---

### **9: Interval Problems, Sort First Then Reason**

**Recognition Signal:**  
The problem involves time ranges, meetings, schedules, or any pairs of [start, end] values. Words like "overlap," "conflict," "merge," "intersect," or "schedule" signal an interval problem.

**The Universal First Step: Sort by Start Time**  
Nearly every interval problem begins by sorting the intervals by their start time. Once sorted, you process intervals left to right, and each interval only needs to be compared with its immediate predecessor or the current state.

**What Overlap Means:**  
Two intervals `[a, b]` and `[c, d]` overlap if and only if `a < d && c < b` (assuming a <= b and c <= d). After sorting by start time, this simplifies: if the current interval's start is less than the previous interval's end, they overlap.

```
Overlap:      |---A---|
                             |---B---|
              a < d and c < b  → overlap

No overlap:   |---A---|
                             |---B---|
              b <= c  → no overlap
```

**Common Interval Problem Types:**

| Type                                     | What You Do                                  | Example                     |
| ---------------------------------------- | -------------------------------------------- | --------------------------- |
| Merge overlapping                        | Combine intervals that overlap               | Merge Intervals             |
| Find conflicts                           | Count or detect overlapping intervals        | Meeting Rooms               |
| Remove minimum to eliminate overlap      | Greedy selection of which to keep            | Non-overlapping Intervals   |
| Insert into sorted intervals             | Find position and merge if needed            | Insert Interval             |
| Find free time                           | Gaps between sorted intervals                | Employee Free Time          |

Interval problems are highly pattern-recognizable. Once you sort by start time, the remaining logic is almost always a single linear scan comparing each interval to the previous one or to a running state.

**Interview Relevance:** Say "I'll sort by start time" before anything else. It is almost never the wrong first move, and it immediately reduces a seemingly complex problem to a linear scan.

---

### **10: Problem Walkthrough, Merge Intervals**

**Problem: Merge Intervals (LeetCode 56)**

Given an array of intervals, merge all overlapping intervals and return the non-overlapping result.

Given: `intervals = [[1,3],[2,6],[8,10],[15,18]]`  
Expected: `[[1,6],[8,10],[15,18]]`

**Approach: Sort by Start, Then Merge Linearly**

```cpp
vector<vector<int>> merge(vector<vector<int>>& intervals) {
    sort(intervals.begin(), intervals.end());   // lexicographic → sorts by start
    vector<vector<int>> merged = {intervals[0]};
    for (int i = 1; i < (int)intervals.size(); i++) {
        int start = intervals[i][0], end = intervals[i][1];
        if (start <= merged.back()[1]) {
            merged.back()[1] = max(merged.back()[1], end);
        } else {
            merged.push_back({start, end});
        }
    }
    return merged;
}
```

Time: O(n log n) for sorting. Space: O(n) for the result.

A default `sort` on `vector<vector<int>>` compares vectors lexicographically — first by start, then by end — which is exactly the "sort by start time" the Python version does explicitly with `key=lambda x: x[0]`.

**How It Works:**  
After sorting, scan left to right. If the current interval's start is less than or equal to the last merged interval's end, they overlap. Extend the last merged interval's end to `max(last_end, current_end)`. If they do not overlap, start a new merged interval.

**Why `max(merged.back()[1], end)` Instead of Just `end`:**  
The current interval might be entirely contained within the previous one. For example, [1, 10] and [2, 5]. The merged result should be [1, 10], not [1, 5]. Taking the max of the ends handles this correctly.

Merge Intervals is a top-10 most-asked medium problem. It is the foundation for all other interval problems. Once you can merge intervals, Meeting Rooms, Insert Interval, and Non-overlapping Intervals are all minor variations.

**Interview Relevance:** The `max()` when extending the end is the one-line detail that separates a correct merge from a buggy one. Mention the contained-interval case ([1,10] vs [2,5]) unprompted to show you've thought about it.

---

### **10B: Merge Intervals, Overlap Detection Logic**

**Step-by-Step Trace:**

```
Given: intervals = [[1,3],[2,6],[8,10],[15,18]]
After sorting (already sorted): [[1,3],[2,6],[8,10],[15,18]]

Start: merged = [[1,3]]
[2,6]:   start=2 <= merged.back()[1]=3 → overlap
         merged.back()[1] = max(3, 6) = 6
         merged = [[1,6]]
[8,10]:  start=8 > merged.back()[1]=6 → no overlap
         merged.push_back({8,10})
         merged = [[1,6],[8,10]]
[15,18]: start=15 > merged.back()[1]=10 → no overlap
         merged.push_back({15,18})
         merged = [[1,6],[8,10],[15,18]]
```

**Edge Case: Contained Interval**

```
Given: [[1,10],[2,5],[3,7]]
After sorting: [[1,10],[2,5],[3,7]]

Start: merged = [[1,10]]
[2,5]: start=2 <= 10 → overlap
       merged.back()[1] = max(10, 5) = 10
       merged = [[1,10]]   (no change, [2,5] was contained)
[3,7]: start=3 <= 10 → overlap
       merged.back()[1] = max(10, 7) = 10
       merged = [[1,10]]   (no change, [3,7] was also contained)

Result: [[1,10]]. Both smaller intervals were absorbed.
```

**Common Mistake:**  
Using `start < merged.back()[1]` (strict less than) instead of `start <= merged.back()[1]`. Intervals like [1,3] and [3,5] are touching and should be merged. Using strict less than would miss this case and leave them separate.

The "touching intervals" edge case (where one ends exactly where another begins) is a common interviewer follow-up question. Using `<=` instead of `<` handles it correctly.

**Interview Relevance:** Be ready for the follow-up: "what if one interval ends exactly where the next begins?" The answer is `<=` for merging — and the next section's Meeting Rooms sweep handles the same tie automatically via event ordering.

---

### **11: Problem Walkthrough, Meeting Rooms**

**Problem: Meeting Rooms II (LeetCode 253)**

Given an array of meeting time intervals, find the minimum number of conference rooms required.

Given: `intervals = [[0,30],[5,10],[15,20]]`  
Expected: 2 (meetings [0,30] and [5,10] overlap, requiring 2 rooms)

Split each interval into two events: a start event (+1 room) and an end event (-1 room). Sort all events by time. When a start and end happen at the same time, process the end first (a room frees up before the next meeting starts).

**Approach: Sort Events and Count Active Meetings**

```cpp
int minMeetingRooms(vector<vector<int>>& intervals) {
    vector<pair<int, int>> events;
    for (auto& iv : intervals) {
        events.push_back({iv[0], 1});    // start: +1 room
        events.push_back({iv[1], -1});   // end:   -1 room
    }
    sort(events.begin(), events.end());

    int maxRooms = 0, currentRooms = 0;
    for (auto& [time, change] : events) {
        currentRooms += change;
        maxRooms = max(maxRooms, currentRooms);
    }
    return maxRooms;
}
```

Sorting `pair<int,int>` compares time first, then change — so at the same time, `-1` sorts before `+1`. The end event is automatically processed before the start event, exactly the tie-break rule the problem requires.

**Trace:**

```
Events sorted: [(0,+1), (5,+1), (10,-1), (15,+1), (20,-1), (30,-1)]

time=0:  rooms=1, max=1
time=5:  rooms=2, max=2
time=10: rooms=1, max=2
time=15: rooms=2, max=2
time=20: rooms=1, max=2
time=30: rooms=0, max=2

Result: 2 rooms required.
```

Time: O(n log n) for sorting. Space: O(n) for the events.

**Interview Tip:** The event-splitting technique (converting intervals into +1/-1 events) generalizes to many scheduling problems. It works for "maximum concurrent users," "peak bandwidth," "maximum overlapping ranges," and any problem that asks for the peak of simultaneous activity.

Meeting Rooms II is a top-20 most-asked medium problem. The event-splitting approach is cleaner and more generalizable than the heap-based alternative, though both are accepted in interviews.

**Interview Relevance:** If asked for the heap alternative, mention it briefly (push end times, pop when a meeting starts early), then return to events as your preferred solution — it is shorter and the sort already gives you the tie-break for free.

---

### **12: Problem Walkthrough, Non-overlapping Intervals**

**Problem: Non-overlapping Intervals (LeetCode 435)**

Given an array of intervals, return the minimum number of intervals to remove to make the rest non-overlapping.

Given: `intervals = [[1,2],[2,3],[3,4],[1,3]]`  
Expected: 1 (remove [1,3])

**This Is a Greedy Problem.**  
Sort as in all interval problems. And when collision happens keep the one with the least end time. This ensures we can minimise the number of deletions. Because taking an interval with  longer end increases the probability that it's going to overlap again with another interval.

```cpp
class Solution {

public:
	int eraseOverlapIntervals(vector<vector<int>>& intervals) {
	sort(intervals.begin(), intervals.end());
	int res=0;
	int prev = intervals[0][1]; 
	
	for (int i=1; i<intervals.size(); i++) {
		int start = intervals[i][0];
		if (start >= prev) {
			prev = intervals[i][1];
		}
		else {
			res++;
			prev = min(intervals[i][1], prev);
		}
	}
	return res;
	}

};
```

Time: O(n log n). Space: O(1) extra.

Non-overlapping Intervals is a classic greedy problem. It is equivalent to the "Activity Selection Problem" from algorithm textbooks. The greedy choice (keep the interval that ends earliest) is provably optimal.

---

### **13: Interview Practice, Interval Patterns**

**Test yourself. Identify the approach before reading the answer.**

**Problem 1:**  
Given a new interval and a list of non-overlapping sorted intervals, insert the new interval and merge if necessary.

What approach applies?

Answer: Insert Interval (LeetCode 57). Three phases: add all intervals that end before the new one starts, merge all intervals that overlap with the new one (tracking the min start and max end), then add all intervals that start after the new one ends. Time: O(n). No sorting needed since the input is already sorted.

**Problem 2:**  
Given a set of intervals representing employees' working hours, find all intervals when all employees are free.

What approach applies?

Answer: Merge all intervals across all employees, sort by start time, then find the gaps between merged intervals. The gaps are the free times. Alternatively, flatten all intervals, merge, and return the spaces between them.

**Problem 3:**  
Given an array of meeting intervals, determine if a person can attend all meetings (no overlaps).

What approach applies?

Answer: Meeting Rooms I. Sort by start time. Scan linearly. If any meeting's start is less than the previous meeting's end, return false. This is Merge Intervals simplified: instead of merging, just detect the first overlap.

Every interval problem follows the same structure: sort, then linear scan. What changes is whether you merge, count, remove, or detect during the scan. The sorting step is never optional.

**Key Insight:** Interval problems are among the most pattern-recognizable in all of LeetCode. Once you sort and set up the linear scan, the remaining logic is almost always straightforward comparison.

**Interview Relevance:** These four interval problems (merge, rooms, remove, insert) share one skeleton. Practice them as a set — knowing which sort key each one needs (start vs end vs event) is the only real decision.

---

### **14: Greedy Algorithms, When Local Choices Work**

**What a Greedy Algorithm Is:**  
A greedy algorithm makes the locally optimal choice at each step, hoping this leads to the globally optimal result. Unlike dynamic programming (Section 5), greedy does not consider all possibilities. It commits to the best current choice and never looks back.

**When Greedy Works:**  
Greedy algorithms only produce correct results when the problem has two properties:

1. **Greedy choice property:** A locally optimal choice leads to a globally optimal solution.
2. **Optimal substructure:** An optimal solution to the problem contains optimal solutions to subproblems.

**Examples of Greedy Working:**

| Problem                   | Greedy Choice                                        | Why It Works                                      |
| ------------------------- | ---------------------------------------------------- | ------------------------------------------------- |
| Non-overlapping Intervals | Keep the interval ending earliest                    | Leaves maximum room for future intervals          |
| Jump Game                 | Track the farthest reachable position                | Any position reachable from an earlier valid position |
| Assign Cookies            | Give the smallest sufficient cookie to each child    | Using a larger cookie wastes resources            |
| Gas Station               | Start from the station where cumulative gas is lowest | If total gas >= total cost, a valid start exists  |

**Examples of Greedy Failing:**

| Problem                              | Why Greedy Fails                                   | Correct Approach    |
| ------------------------------------ | -------------------------------------------------- | ------------------- |
| Coin Change (arbitrary denominations) | Largest coin first misses combinations            | Dynamic programming |
| 0/1 Knapsack                        | Highest value-per-weight first misses better combinations | Dynamic programming |
| Longest Increasing Subsequence       | Greedily extending fails to find the global longest | Dynamic programming |

The hardest part of greedy problems is proving correctness. Interviewers accept greedy solutions only when you can explain why the greedy choice is safe. The next section covers how to do this.

**Interview Relevance:** Before coding a greedy solution, name the greedy choice property out loud: what are you optimizing, and why does taking the best local option never lock you out of a better global answer?

---

### **15: Proving a Greedy Approach Is Correct**

**Two Proof Techniques:**

**1. Exchange Argument:**  
Assume there exists an optimal solution that does not make the greedy choice at some step. Show that you can swap that step for the greedy choice without making the solution worse. Therefore, the greedy choice is at least as good as any alternative.

**Example: Non-overlapping Intervals**  
Greedy choice: keep the interval ending earliest.

Exchange argument: suppose the optimal solution keeps interval A (ending later) instead of interval B (ending earlier). Since B ends before A, everything that does not conflict with A also does not conflict with B. So swapping A for B keeps at least as many intervals. The greedy choice is at least as good.

**2. Stays Ahead Argument:**  
Show that after each greedy step, the greedy solution is at least as good as any other solution at that point. Since greedy is always ahead or equal, it must be at least as good at the end.

**In Practice for Interviews:**  
You do not need a formal proof. You need a convincing one-paragraph explanation. Something like: "I sort by end time and greedily keep each non-conflicting interval. This works because keeping the interval that ends earliest always leaves the most room for future intervals. If I kept one that ends later, I could only do worse."

**Common Mistake:**  
Assuming greedy works without justification. "It seems like sorting by start time and always picking the first would work" is not an argument. State what you are optimizing, why the greedy choice does not miss better options, and ideally give a brief exchange argument.

Interviewers expect you to justify greedy choices. A correct greedy solution without justification is weaker than one with a clear explanation. The exchange argument is the most commonly expected justification format.

**Interview Relevance:** Practice saying the exchange argument in 15 seconds. The structure is always: "Suppose an optimal solution takes X instead of the greedy choice Y — I can swap X for Y and not do worse, so Y is safe."

---

### **16: Heap vs Sort vs Greedy Decision Framework**

**When to Use Each:**

| Signal in the Problem                          | Best Tool                     | Why                                          |
| ---------------------------------------------- | ----------------------------- | -------------------------------------------- |
| "Top K" or "Kth largest/smallest"              | Heap                          | O(n log k) vs O(n log n) for full sort       |
| "Merge K sorted sequences"                     | Heap (K-way merge)            | Maintains K pointers with O(log K) per operation |
| "Median of a stream"                           | Two heaps                     | Dynamically maintains sorted halves          |
| Overlapping time ranges                        | Sort by start, then linear scan | Sorting enables single-pass overlap detection |
| "Minimum rooms" or "maximum concurrent"        | Sort events (+1/-1)           | Event splitting reduces to a sweep line      |
| "Remove minimum to avoid conflicts"            | Sort by end, greedy selection | Ending earliest leaves maximum future room   |
| Scheduling to maximize items                   | Sort by end, greedy selection | Activity selection problem                   |

**When Greedy Fails and You Need DP:**

| Problem                     | Why Greedy Fails                                  | What to Use    |
| --------------------------- | ------------------------------------------------- | -------------- |
| Coin Change (arbitrary coins) | Largest first may not reach the target           | DP (Section 5) |
| Knapsack                    | Best ratio first may not fill optimally           | DP (Section 5) |
| Word Break                  | Longest match first may block valid splits        | DP (Section 5) |

**The Test for Greedy vs DP:**  
Ask: "Does the locally optimal choice always lead to the global optimum?" If yes, use greedy. If the answer depends on future choices you have not made yet, you need DP.

Choosing between greedy and DP is one of the highest-value skills in interviews. Greedy solutions are simpler and faster to code, so correctly identifying when greedy works saves significant time.

**Interview Relevance:** Run this framework in the first minute of any optimization or scheduling problem. Naming your tool and why — "this is greedy because picking the earliest-ending interval never hurts future options" — is exactly what interviewers are listening for.

---

### **17: Common Mistakes and Interview Tips**

**Common Interview Mistakes in This Section:**

| Mistake                                                     | Why It Fails                                                                 | Better Approach                                                                 |
| ----------------------------------------------------------- | ---------------------------------------------------------------------------- | ------------------------------------------------------------------------------- |
| Sorting intervals by start when end-time sorting is needed  | Produces incorrect results for overlap removal problems                      | For "maximum non-overlapping"/"minimum removal," sort by end-time              |
| Using a max heap of size K for "K largest"                  | Max heap of size n and popping K times is O(n + K log n), worse than min heap of size K which is O(n log K) | Use a min heap of size K. Its top is the Kth largest — the threshold for replacement |
| Forgetting the tiebreaker index in heap tuples              | Ties fall back to comparing node pointers — an unspecified, meaningless order | Always include a unique index as the second element: (value, index, object)     |
| Assuming greedy works without proof                         | Greedy is wrong for many optimization problems                               | State why the greedy choice does not miss better options before coding          |
| Using `start < prevEnd` instead of `start <= prevEnd` for overlap detection | Misses touching intervals that should be merged                    | Use `<=` for merge, `<` for strict overlap depending on the problem definition  |

**Interview Tip:**  
For interval problems, always clarify with the interviewer: "If one meeting ends at time 5 and another starts at time 5, do they conflict?" The answer determines whether you use `<` or `<=` in your overlap check. This question shows attention to detail.

The mistakes above are the primary reasons candidates fail heap and interval problems. Each one is preventable with 10 seconds of careful thought before coding.

**Interview Relevance:** The touching-meetings question and the sort-key question (start vs end) are the two clarifications that most often change your code. Ask them before you write, not after you fail a test case.

---

### **18: Section Cheat Sheet**

**Heap, Intervals, and Greedy, Quick Reference**

**Pattern Templates:**

```
TOP K (min heap of size K):
    priority_queue<int, vector<int>, greater<int>> heap(nums.begin(), nums.begin() + k);
    for (int i = k; i < (int)nums.size(); i++) {
        if (nums[i] > heap.top()) {
            heap.pop();
            heap.push(nums[i]);
        }
    }

K-WAY MERGE:
    using Entry = tuple<int, int, int>;                // (value, list_index, elem_index)
    priority_queue<Entry, vector<Entry>, greater<Entry>> heap;
    for each non-empty list i: heap.push({list[i][0], i, 0});
    while (!heap.empty()) {
        auto [val, li, ei] = heap.top(); heap.pop();
        result.push_back(val);
        if (ei + 1 < lists[li].size()) heap.push({lists[li][ei + 1], li, ei + 1});
    }

MERGE INTERVALS:
    sort by start time
    for each interval:
        if (start <= merged.back()[1]) merged.back()[1] = max(merged.back()[1], end);
        else merged.push_back({start, end});

MEETING ROOMS (max concurrent):
    events = {{start, +1}, {end, -1}} for each interval
    sort events                                   // ties: -1 before +1 automatically
    sweep: current += change, track max

NON-OVERLAPPING (minimum removals):
    sort by end time
    for each interval:
        if (start >= lastKeptEnd) lastKeptEnd = end;   // keep
        else count++;                                  // remove
```

**Key Complexities:**

| Pattern                       | Time          | Space  |
| ----------------------------- | ------------- | ------ |
| Top K with heap               | O(n log k)    | O(k)   |
| Quickselect (kth element)     | O(n) average  | O(1)   |
| K-way merge                   | O(N log K)    | O(K)   |
| Merge Intervals               | O(n log n)    | O(n)   |
| Meeting Rooms (event sweep)   | O(n log n)    | O(n)   |
| Non-overlapping (greedy)      | O(n log n)    | O(1)   |

All of these are dominated by either one sort — O(n log n) — or one heap pass — O(n log k). If your solution has worse complexity than that (for example, repeatedly re-sorting inside a loop, O(n² log n)), you are missing the standard pattern.

**Interview Relevance:** These five templates cover this section's problems. Review the sort key for each interval template (start for merge, events for rooms, end for removal) — mixing them up is the most common cheat-sheet-level mistake.

---

### **19: Section Summary and Pattern Recognition Signals**

**What We Covered:**

| Pattern               | Recognition Signal                                      | Key Insight                                                   |
| --------------------- | ------------------------------------------------------- | ------------------------------------------------------------- |
| Heap fundamentals     | Repeated access to min/max in a changing collection     | `priority_queue`: default max heap, `greater<int>` for min heap |
| Top K                 | "Top K", "Kth largest/smallest", most frequent          | Min heap of size K holds the K largest; root is the threshold |
| K-way merge           | Merge K sorted sequences / find kth across sequences    | Heap of (value, list index, element index); O(N log K)        |
| Intervals, sort first | Overlap, conflict, schedule, [start, end] pairs         | Always sort first, then a single linear scan                  |
| Merge intervals       | "Merge overlapping ranges"                              | Sort by start, extend with `max(lastEnd, end)`, `<=` for touching |
| Event splitting       | "Minimum rooms", "maximum concurrent"                   | +1/-1 events, sort, sweep and track the peak                  |
| Greedy by end time    | "Remove minimum to avoid conflicts"                     | Keep the interval that ends earliest — leaves maximum room    |
| Greedy proofs         | Any greedy choice in an optimization problem            | Exchange argument: swap in the greedy choice, never do worse  |
| Heap vs sort vs greedy | Decision framework for this section's tools            | Top K → heap; overlap → sort; local choice provably safe → greedy |

**How This Connects to Other Sections:**

| This Section's Pattern    | Where It Reappears Later                                   |
| ------------------------- | ---------------------------------------------------------- |
| Min heap / priority queue | Dijkstra's shortest path uses a heap of candidate distances |
| Top K                     | Streaming and ranking problems, quota/selection follow-ups |
| Sort + linear scan        | The skeleton for many array problems beyond intervals      |
| Event splitting (+1/-1)   | Sweep-line problems: coverage, bandwidth, concurrency peaks |
| Greedy by end time        | Kruskal's MST (Union-Find from Section 2) picks edges greedily |
| Greedy vs DP test         | The core decision in Section 5: when a local choice is not enough, DP enumerates |
| Two heaps                 | Median maintenance reappears in streaming variants         |

**What Comes Next:**  
Section 5 introduces dynamic programming — the tool for problems where greedy fails because today's choice changes what is possible tomorrow. The greedy-vs-DP test from section 16 is the bridge: if a local choice is always safe, you stay here; if the answer depends on future choices, you move to DP.

**Interview Relevance:** This section's patterns are among the fastest to execute in interviews — a heap problem is 15 lines, an interval problem is 10. The risk is not implementation but selection: picking the wrong sort key or reaching for greedy without a justification. State your sort key, state your greedy choice, and these become among the highest-ROI problems on the board.
