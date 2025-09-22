#ifndef OPTIMAL_CACHE_HPP 
#define OPTIMAL_CACHE_HPP

//---------------------------------------------------------------------------------------------------------------

#include <unordered_map>
#include <deque>
#include <vector>
#include <iostream>
#include <cstdint>

#include "global.hpp"

//---------------------------------------------------------------------------------------------------------------

template <typename key_t, typename item_t>
struct OPT_cache
{
    private:
        // struct typedef
        using index_t      = ssize_t;
        using request_t    = typename std::pair<key_t, item_t>;
        using IndexMapType = typename std::unordered_map<key_t, std::deque<index_t>>;
        using CacheMapType = typename std::unordered_map<key_t, item_t>;
        using input_vector = typename std::vector<request_t>;

        // class private constants
        static constexpr key_t   POISON_KEY = static_cast<ssize_t>(0XEDAA);
        static constexpr index_t INF_INDEX  = ~((index_t) 0); // index_t max value

        // class private variables
        ssize_t      capacity_;
        ssize_t      hits_counter_;

        IndexMapType map_future_;
        CacheMapType cache_map_;


    public:
                    OPT_cache        ();
                    OPT_cache        (ssize_t input_capacity);
        ssize_t     run_cache        (const input_vector &key_items) ;
        ssize_t     get_hit_count    ()                              ;

    private:
        // struct private methods

        bool        remove_farest     (const key_t &insert_key);
        void        load_map_of_future(const input_vector &key_items);

        ON_DEBUG(
        void        cache_map_dump    ();
        )
};

// class public methods
//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------

// ctor
template <typename key_t, typename item_t>
OPT_cache<key_t, item_t>::OPT_cache() :
capacity_(0), hits_counter_(0)
{}

//---------------------------------------------------------------------------------------------------------------

template <typename key_t, typename item_t>
OPT_cache<key_t, item_t>::OPT_cache(ssize_t input_capacity) :
capacity_(input_capacity), hits_counter_(0)
{}

//---------------------------------------------------------------------------------------------------------------

template <typename key_t, typename item_t>
ssize_t
OPT_cache<key_t, item_t>::run_cache(const typename OPT_cache<key_t, item_t>::input_vector &key_items)
{
    if (capacity_ <= 0LL) 
        return 0;

    load_map_of_future(key_items);

    //todo: impl add_cache() and add to cache interface
    for (ssize_t i = 0; i < key_items.size(); i++)
    {
        const auto &[curr_key, curr_item] = key_items[i];

        if (!map_future_[curr_key].empty())
            map_future_[curr_key].pop_front();

        if (cache_map_.find(curr_key) != cache_map_.end())
            hits_counter_++;

        else if (cache_map_.size() < capacity_)
            cache_map_[curr_key] = curr_item;

        else
        {
            bool should_be_replace = remove_farest(curr_key);
            if (should_be_replace) cache_map_[curr_key] = curr_item;
        }
    }

    return hits_counter_;
}

//---------------------------------------------------------------------------------------------------------------

template <typename key_t, typename item_t>
ssize_t
OPT_cache<key_t, item_t>::get_hit_count()
{
    return hits_counter_;
}

// struct private methods
//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------

template <typename key_t, typename item_t>
bool
OPT_cache<key_t, item_t>::remove_farest(const key_t &insert_key)
{
    index_t       farthest_next_index = 0;
    key_t         farthest_key        = POISON_KEY;
    const index_t next_insert_index   = (map_future_[insert_key].empty()) ?  
                                         INF_INDEX : map_future_[insert_key].front();

    for (const auto &[key_in_cache, item] : cache_map_)
    {
        if (map_future_[key_in_cache].empty())
        {
            farthest_key = key_in_cache;
            break;
        }

        index_t next_index = map_future_[key_in_cache].front();
    
        if (next_index <= farthest_next_index)
            continue;
    
        farthest_next_index = next_index;
        farthest_key        = key_in_cache;
    }

    if (!(farthest_next_index > next_insert_index && farthest_key != POISON_KEY))
        return false;

    cache_map_.erase(farthest_key);
    return true;
}

//---------------------------------------------------------------------------------------------------------------

template <typename key_t, typename item_t>
void
OPT_cache<key_t, item_t>::load_map_of_future(const input_vector &key_items)
{
    for (index_t i = 0; i < key_items.size(); i++)
        map_future_[key_items[i].first].push_back(i + 1);        
}

//---------------------------------------------------------------------------------------------------------------

ON_DEBUG(
template <typename key_t, typename item_t>
void
OPT_cache<key_t, item_t>::cache_map_dump()
{
    for (const auto &pair : cache_map_)
    {
        key_t key_in_cache = pair.first;
        item_t item_in_cache = pair.second;

        std::cout << "key: " << key_in_cache << " item: " << item_in_cache << std::endl;
    }
}
)

//---------------------------------------------------------------------------------------------------------------

#endif // OPTIMAL_CACHE_HPP
