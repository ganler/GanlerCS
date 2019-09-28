#pragma once

#include <iostream>
#include <unordered_map>
#include <stdexcept>
#include <memory>
#include <type_traits>

#define gassert(X) \
if(!(X))\
{\
  std::cerr << "GASSERT FAILED :=> line " << __LINE__ << ":   " << #X << '\n';\
  std::exit(-1);\
}

namespace ganler
{

    template <typename T>
    class trie
    {
    private:
        struct node_t;
    public:
        using value_type = T;
        using node_ptr_t = std::unique_ptr<node_t>;
    private:
        struct node_t : std::unordered_map<value_type, node_ptr_t>{
            bool ends = false;
        };
    public:

        template <typename Iter>
        void inline insert(Iter beg, Iter end)
        {
            if(beg > end)
                throw std::logic_error("fuck, your beg is larger than end!");
            auto p = root.get();
            while(beg != end) {
                auto&& try_find = p->find(*beg);
                if(try_find == p->end())
                {
                    p = ((*p)[*beg++] = std::make_unique<node_t>()).get();
                    continue;
                }
                p = (*p)[*beg++].get();
            }
            p->ends = true;
        }
        template <typename C>
        void inline insert(C&& container)
        {
            insert(std::begin(container), std::end(container));
        }
        template <typename Iter>
        bool inline search(Iter beg, Iter end) const
        {
            if(beg > end)
                throw std::logic_error("fuck, your beg is larger than end!");
            else if(beg == end)
                throw std::logic_error("fuck, your want to search `empty`?");
            auto p = root.get();
            while(beg != end) {
                auto try_find = p->find(*beg++);
                if (try_find == p->end())
                    return false;
                p = try_find->second.get();
            }
            return p->ends;
        }
        template <typename C>
        bool inline search(const C& container) const
        {
            return search(std::begin(container), std::end(container));
        }
    private:
        node_ptr_t root = std::make_unique<node_t>();
    };
}

// int main()
// {
//     using namespace ganler;
//     trie<char> my_trie;
//     auto v1 = std::initializer_list<char>{1, 2, 3};
//     auto v2 = std::initializer_list<char>{1, 2, 2};
//     auto v3 = std::initializer_list<char>{1, 2, 2, 2};
//     auto v4 = std::initializer_list<char>{1, 2, 2, 3, 2};
//     auto v5 = std::initializer_list<char>{9, 2, 2, 3};

//     my_trie.insert(v1);
//     my_trie.insert(v2);
//     my_trie.insert(v3);

//     gassert(my_trie.search(v1));
//     gassert(my_trie.search(v2));
//     gassert(my_trie.search(v3));
//     gassert(!my_trie.search(v4));
//     gassert(!my_trie.search(v5));
// }