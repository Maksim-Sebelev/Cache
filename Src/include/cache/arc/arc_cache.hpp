#ifndef CACHE_ARC_CACHE_HPP
#define CACHE_ARC_CACHE_HPP

//---------------------------------------------------------------------------------------------------------------

#include <cstddef>
#include <list>
#include <unordered_map>

//---------------------------------------------------------------------------------------------------------------

template <typename key_type, typename item_t>
struct cache_t
{
    private:
        size_t              cache_size_; 
        std::list<key_type> cache_list_; 

        using list_iterator_t = typename std::list<key_type>::iterator;
        std::unordered_map<item_t, list_iterator_t> hash_table_;
        
    public:

};

//---------------------------------------------------------------------------------------------------------------

#endif // CACHE_ARC_CACHE_HPP
