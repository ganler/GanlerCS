# Combination&Search

## Set

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

