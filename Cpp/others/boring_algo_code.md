# Codes

### Permutation

#### Naive permutation(unique version)

```c++
vector<vector<int>> ret;
sort(tmp.begin(), tmp.end()); // Important!
do{
  ret.push_back(tmp);
}while(next_permutation(tmp.begin(), tmp.end()));
```

```c++
// Non-STL version.
vector<vector<int>> permute(vector<int>& nums) {
  const int n = nums.size();
  vector<vector<int>> ans;
  ans.reserve(fact(n));

  std::function<void(vector<int>&,int)> permutation = 
    [&ans, n, &permutation](vector<int>& nums, int k) {
    if(k == n - 1) ans.push_back(nums);
    for(int i = k; i < n; ++i) {
      std::swap(nums[k], nums[i]);
      permutation(nums, k+1);
      std::swap(nums[k], nums[i]);
    }            
  };
  
  permutation(nums, 0);
  return ans;
}
```

#### Kth Permutation

给出集合 `[1,2,3,…,n]`，其所有元素共有 *n*! 种排列。

按大小顺序列出所有排列情况，并一一标记，当 *n* = 3 时, 所有排列如下：

1. `"123"`
2. `"132"`
3. `"213"`
4. `"231"`
5. `"312"`
6. `"321"`

给定 *n* 和 *k*，返回第 *k* 个排列。

```c++
string getPermutation(int n, int k) {
  string str(_str.begin(), _str.begin() + n);
  for(int i = 0; i < k-1; ++i)
    next_permutation(str.begin(), str.end());
  return str;
}
```

#### Dfs

```
输入: candidates = [2,3,6,7], target = 7,
所求解集为:
[
  [7],
  [2,2,3]
]
```

```c++
vector<vector<int>> combinationSum(vector<int>& candidates, int target) {
    vector<vector<int>> ret;
    vector<int> tmp;
    sort(candidates.begin(), candidates.end());
    
  	static function<void(int, int)> dfs = [&](int left, int index) {
        if (left == 0) { ret.push_back(tmp); return; }

        for (int i = index; i < candidates.size(); ++i)
        {
            if (left < candidates[i]) break;
            if (i != index && candidates[i-1] == candidates[i]) continue;
            
          	tmp.push_back(candidates[i]);
            dfs(left - candidates[i], i);
            tmp.pop_back();
        }
    };

    dfs(target, 0);
    return ret;
}
```

### subset

#### Subsets of non-duplicate elements

```c++
// Use bit op.
vector<vector<int>> subsets(vector<int>& nums)
{// 2^n
  if(nums.empty()) return {{}};
  vector<vector<int>> ret(pow(2, nums.size()));
  for(int i = 0; i < ret.size(); ++i)
    for(int j = 0; j < nums.size(); ++j)
      if((1 << j) & i)
        ret[i].push_back(nums[j]);
  return ret;
}

vector<vector<int>> subsets(vector<int>& nums)
{// 2^n
  vector<vector<int>> ret = {{}};
  ret.reserve(pow(2, nums.size()));
  for(int i = 0; i < nums.size(); ++i)
    for(int j = 0, sz = ret.size(); j < sz; ++j)
    {
      ret.push_back(ret[j]);
      ret.back().push_back(nums[i]);
    }
  return ret;
}
```

#### Subsets of duplicate elements

```c++
vector<vector<int>> subsetsWithDup(vector<int>& nums) {
  if (nums.empty()) return { {} };
  sort(nums.begin(), nums.end());
  vector<vector<int>> ret = { {} };
  uint32_t last_begin = 0;
  for (uint32_t i = 0; i < nums.size(); ++i) // 插入的数字
  {
    auto now_sz = ret.size();
    for (uint32_t j = (i != 0 && nums[i] == nums[i - 1]) ? last_begin : 0; j < now_sz; ++j)
    {
      ret.push_back(ret[j]);
      ret.back().push_back(nums[i]);
    }
    last_begin = now_sz;
  }
  return ret;
}
```

#### LRU

```c++
/*
 * @lc app=leetcode.cn id=146 lang=cpp
 *
 * [146] LRU缓存机制
 */

// 自己定义规则find的话用find_if
// 直接对应value去find才用find
// LRU其实就是把用到的东西放到front
// 优化的方法就是hash_table + linked_list ... 

class LRUCache {
public:
    LRUCache(int capacity){
        max_sz = capacity;
    }

    int get(int key)
    {
        auto aim = std::find_if(data.begin(), data.end(), [key](const auto& x){ return x.key == key; });
        if(aim == data.end())
            return -1;
        auto next_front = *aim;
        data.erase(aim);
        data.push_front(next_front);
        return data.front().val;
    }

    void put(int key, int value)
    {
        auto aim = std::find_if(data.begin(), data.end(), [key](const auto& x){ return x.key == key; });
        if(aim == data.end())
        {
            if(data.size() == max_sz)
                data.pop_back();
            data.push_front({key, value});
            return;
        }
        data.erase(aim);
        data.push_front({key, value});
    }
private:
    struct cache
    {
        int key = -1;
        int val  = -1;
    };
    list<cache> data;
    int max_sz;
};
/**
 * Your LRUCache object will be instantiated and called as such:
 * LRUCache* obj = new LRUCache(capacity);
 * int param_1 = obj->get(key);
 * obj->put(key,value);
 */
```

#### 二叉树遍历

```c++
// 先不考虑前序、中序和后序，大概是这样的调用loop。
class Solution {
public:
    vector<int> inorderTraversal(TreeNode* root) {
        vector<int> ret;
        stack<TreeNode*> stk;
        while(root != NULL || !stk.empty())
        {
            while(root != NULL)
            {// 前序放这
                stk.push(root);
                root = root->left;
            }
            if(!stk.empty())
            {
                root = stk.top();
                stk.pop();//中序放这
                root = root->right;
            }
        }
        return ret;
    }
};

// 后续遍历的话，将递归的过程反过来看，做一个rev_postorder:
/*
 * @lc app=leetcode.cn id=145 lang=cpp
 *
 * [145] 二叉树的后序遍历
 */
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */

/*
-> rev:
		if NULL: end;
		out();
		rev(r);
		rev(l);
-> rev:

*/
class Solution {
public:
    vector<int> postorderTraversal(TreeNode* root) {
        if(root == NULL)
            return {};
        vector<int> ret;
        stack<TreeNode*> stk;
        stk.push(root);
        while(!stk.empty())
        {
            root = stk.top();
            stk.pop();
            ret.push_back(root->val);
            if(root->left)
                stk.push(root->left);
            if(root->right)
                stk.push(root->right);
        }
        reverse(ret.begin(), ret.end());
        return ret;
    }
};
```

