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

#### 单词串翻转

```c++
/*
 * @lc app=leetcode.cn id=151 lang=cpp
 *
 * [151] 翻转字符串里的单词
 * 
 * // 检测front和back的时候注意先判断是否empty
 * // 整个先翻转，再将内部单次翻转
 */
class Solution {
public:
    template<typename Str>
    	Str&& reverseWords(Str&& s) 
    {
        reverse(s.begin(), s.end());
        auto beg_ = s.begin(), end_ = s.begin();
        while(beg_ != s.end())
        {
            beg_ = find_if(end_, s.end(), [](auto&& x){ return x != ' ';});
            end_ = find_if(beg_, s.end(), [](auto&& x){ return x == ' ';});
            reverse(beg_, end_);
        }
        int l = 0;
        for(int i=0; i<s.length(); ++i)
            if(s[i] != ' ')
                s[l++] = s[i];
            else if(l != 0 && s[l-1] != ' ') // ' '
                s[l++] = ' ';
        s.erase(s.begin()+l, s.end());
        if(!s.empty() && s.back() == ' ')
            s.pop_back();
        return s;
    }
};
```

#### 手动平方根

> 边界条件很恶心：
>
> - 二分防越界：`mid = l+(r-l)/2`
> - 自平方防越界：`mid <= x/mid`
> - 防除0：`(l+r)/2 != 0 => r > 1`

```c++
class Solution {
public:
    int mySqrt(int x) {
        if(x <= 1)
            return x;
        int l = 0, r = x;
        while(l < r)
        {
            int mid = l+(r-l)/2;
            if(mid <= x / mid) // avoid overflow
                l = mid+1;
            else
                r = mid;
        }
        return r-1;
    }
};
```

#### 牛顿法平方根

$$
y=f(x)=x^2\\
f(x_0)=x_0^2=y_0\\
知y_0,求x_0\\
g(x)=x^2-y_0\\
即求其为0的时候的解\\
g'(x)=2x\\
其(u,u^2-y_0)点的切线为\\
y =2u(x-u)+u^2-y_0=2ux-u^2-y_0\\
x_0'=\frac{1}{2}(u^2+y_0)/u=\frac{1}{2}(u+\frac{y_0}{u})
$$

```c++
class Solution {
public:
    int mySqrt(int x) {
        uint64_t ret = x;
      	while(ret * ret > x)
        		ret = (ret + x / ret) / 2;
      	return ret;
    }
};
```

