#ifndef OPTIMAL_CACHE_HPP
#define OPTIMAL_CACHE_HPP

#include <unordered_map>
#include <deque>
#include <vector>


template <typename key_t, typename item_t>
struct OPT_cache
{
private:
    std::size_t capacity_;
    std::size_t amount_of_items_;
    std::size_t hits_counter_;

    using request_t = typename std::pair<key_t, item_t>;
    using index_t   = typename std::size_t;

    using IndexMapType = typename std::unordered_map<key_t, std::deque<index_t>>;
    IndexMapType map_future_;

    using CacheMapType = typename std::unordered_map<key_t, item_t>;
    CacheMapType cache_map_;

    using input_vector = typename std::vector<request_t>;
    input_vector items_ = 0;

public:
    OPT_cache() : capacity_(0), amount_of_items_(0), hits_counter_(0), items_(0) {}
    OPT_cache(std::size_t input_capacity, std::size_t input_amount_of_items, const input_vector &input_items) 
             : capacity_(input_capacity), amount_of_items_(input_amount_of_items), hits_counter_(0), items_(input_items)
    {
        items_.reserve(input_amount_of_items);  
        load_map_of_future();  
    }
    
/*    bool add_cache(const key_t &key, const item_t &item)
    {


    } */

    void run_optimal_cache()
    {
        for (std::size_t i = 0; i < items_.size(); i++)
        {
            key_t  curr_key  = items_[i].first;
            item_t curr_item = items_[i].second;
            hits_counter_ = 0;

            if (cache_map_.find(curr_key) != cache_map_.end())
                hits_counter_ = 0;

            else if (cache_map_.size() < capacity_)
                    cache_map_[curr_key] = curr_item;
            else    
            {
                remove_farest();
                cache_map_[curr_key] = curr_item;
            }
        }
    }

    std::size_t get_hit_count() const
    {
        return hits_counter_ - 1;
    }

    ~OPT_cache() 
    {
        map_future_.clear();
    }

private:
    void remove_farest()
    {
        index_t farthest_next_index = -1;
        key_t   farthest_key  = -1;

        for (const auto &pair : cache_map_)
        {
            key_t key_in_cache = pair.first;
            index_t next_index = map_future_[key_in_cache].front();
            
            if (map_future_[key_in_cache].empty())
            {
                farthest_key = key_in_cache;
                break;
            }
            else if (next_index > farthest_next_index)
            {
                farthest_next_index = next_index;
                farthest_key  = key_in_cache;
            }
        }

        if (farthest_key != -1)
        {
            cache_map_.erase(farthest_key);
            map_future_[farthest_key].pop_front();
        }
    }
    void load_map_of_future()
    {
        for (index_t i = 0; i < items_.size(); i++)
        {
            key_t key = items_[i].first;
            map_future_[key].push_back(i);        
        }
    }
};

#endif