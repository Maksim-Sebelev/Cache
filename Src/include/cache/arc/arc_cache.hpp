#ifndef ARC_CACHE_HPP
#define ARC_CACHE_HPP

//---------------------------------------------------------------------------------------------------------------

#include <algorithm>
#include <iostream>
#include <list>
#include <vector>
#include <unordered_map>
#include <cassert>

//---------------------------------------------------------------------------------------------------------------

#include "CacheInterface.hpp"

//---------------------------------------------------------------------------------------------------------------

template <typename key_t, typename item_t> 
class ARCCache : public CacheInterface<key_t, item_t>
{
    private:
        // class private definition
        enum class ListLocation
        {
            FIRST_LIST,
            FREQUENT_LIST,
            FIRST_LIST_GHOST,
            FREQUENT_LIST_GHOST,
            NOT_FOUND
        };

        struct CacheEntry
        {
            key_t key;
            item_t item;

            CacheEntry() : key(), item() {}
            CacheEntry(const key_t &k, const item_t &i) : key(k), item(i) {}
        };

        using ListType = typename std::list<CacheEntry>;
        using ListIter = typename ListType::iterator;
        
        struct LocationInfo
        {
            ListLocation location;
            ListIter list_iter;
        
            LocationInfo() : location(ListLocation::NOT_FOUND), list_iter() {}
            LocationInfo(ListLocation loc, ListIter it) : location(loc), list_iter(it) {}    
        };

        using CacheMapType = typename std::unordered_map<key_t, LocationInfo>;
        using CacheMapIter = typename CacheMapType::iterator;


        // class private variables

        size_t       capacity_           ;
        size_t       hits_counter_       ;
        double       adapt_param_        ;

        ListType     list_first_         ;
        ListType     list_frequent_      ;
        ListType     list_first_ghost_   ;
        ListType     list_frequent_ghost_;

        CacheMapType cache_map_          ;

        // class private methods

        void adapt_ghost           (      ListLocation  location                                                                       );
        bool not_empty_and_adaptive(                                                                                                   ) const;
        void move_tail_to_front    (      ListType    & src          ,       ListType    &  dest    ,       ListLocation  dest_location);
        void replace_for_adapt     (                                                                                                   );
        void update_cache_map      (const key_t       & key          ,       ListLocation   location,       ListIter      list_it      );
        void move_to_dest_front    (      ListType    & src          ,       ListType    &  dest    , 
                                          ListIter      list_iterator, const ListLocation&  location, const key_t       & key          );
        void move_to_frequent      (      ListLocation  loc          , const key_t       &  key     , const ListIter    & list_iterator);
        void handle_ghost          (      ListLocation  location     , key_t                key     ,       ListIter      list_iterator);
        bool handle_existing_item  (const CacheMapIter& cache_map_it , const key_t       &  key                                        );
        void handle_cache_overflow (                                                                                                   );
        void add_new_item          (const key_t       &  key         , const item_t      &  item                                       );

    public:
        // class public methods
    
        explicit ARCCache     (                                                                                ); // ctor 1
        explicit ARCCache     (      size_t                                  capacity                          ); // ctor 1
        item_t   get_item     (const key_t                                 & key                               ) const;
        bool     add_cache    (const key_t                                 & key           , const item_t &item);
        size_t   run_cache    (const std::vector<std::pair<key_t, item_t>> & input_key_item                    )       override;
        size_t   get_hit_count(                                                                                ) const override;
        

};

//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------

// ctor 1
template <typename key_t, typename item_t> 
ARCCache<key_t, item_t>::ARCCache() :
capacity_(0), hits_counter_(0), adapt_param_(0.0)
{}

//---------------------------------------------------------------------------------------------------------------

// ctor 2
template <typename key_t, typename item_t> 
ARCCache<key_t, item_t>::ARCCache(size_t capacity) :
capacity_(capacity), hits_counter_(0), adapt_param_(0.0) 
{}

//---------------------------------------------------------------------------------------------------------------

// class private methods
//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------

template <typename key_t, typename item_t>
void
ARCCache<key_t, item_t>::adapt_ghost(ListLocation location)
{
    assert(location != ListLocation::NOT_FOUND);

    const auto &nominator   = (location == ListLocation::FIRST_LIST_GHOST) 
                            ?  list_frequent_ghost_.size() : list_first_ghost_.size();

    const auto &denominator = (location == ListLocation::FIRST_LIST_GHOST) 
                            ? list_first_ghost_.size() : list_frequent_ghost_.size();
    assert(denominator != 0);
    double      ratio_between_ghosts_size = static_cast<double>(nominator) / 
                                    static_cast<double>(denominator);

    double max_  = std::max(1.0, ratio_between_ghosts_size);
    adapt_param_ += (location == ListLocation::FIRST_LIST_GHOST) ? max_ : -max_;
    adapt_param_ = std::clamp(adapt_param_, 0.0, static_cast<double>(capacity_)); 
}

//---------------------------------------------------------------------------------------------------------------

template <typename key_t, typename item_t>
bool
ARCCache<key_t, item_t>::not_empty_and_adaptive() const
{
    const size_t list_size_tmp = list_first_.size(); // size1

    bool is_list_not_empty = list_size_tmp >= 1;
    bool is_list_adaptive_enough = list_size_tmp > static_cast<size_t>(adapt_param_) || 
                                    (list_size_tmp == static_cast<size_t>(adapt_param_) && !list_frequent_ghost_.empty());

    return is_list_not_empty && is_list_adaptive_enough;
}
//---------------------------------------------------------------------------------------------------------------

template <typename key_t, typename item_t>
void
ARCCache<key_t, item_t>::move_tail_to_front(ListType &src, ListType &dest, ListLocation dest_location)
{
    assert(!src.empty());

    const CacheEntry tail_element = std::move(src.back());
    src.pop_back();

    dest.push_front(tail_element);
    update_cache_map(tail_element.key, dest_location, dest.begin());
}
//---------------------------------------------------------------------------------------------------------------

template <typename key_t, typename item_t>
void
ARCCache<key_t, item_t>::replace_for_adapt()
{
    if (not_empty_and_adaptive())
        move_tail_to_front(list_first_, list_first_ghost_, ListLocation::FREQUENT_LIST);
    else
        move_tail_to_front(list_frequent_, list_frequent_, ListLocation::FREQUENT_LIST);
}

//---------------------------------------------------------------------------------------------------------------

template <typename key_t, typename item_t>
void
ARCCache<key_t, item_t>::update_cache_map(const key_t &key, ListLocation location, ListIter list_it)
{
    cache_map_[key] = LocationInfo(location, list_it);
}

//---------------------------------------------------------------------------------------------------------------

template <typename key_t, typename item_t>
void 
ARCCache<key_t, item_t>::move_to_dest_front(ListType&    src          , ListType&           dest    , 
                                            ListIter     list_iterator, const ListLocation &location,
                                            const key_t& key)
{
    CacheEntry element = std::move(*list_iterator);
    src.erase(list_iterator);

    dest.push_front(std::move(element));
    update_cache_map(key, location, dest.begin());
}

//---------------------------------------------------------------------------------------------------------------

template <typename key_t, typename item_t>
void
ARCCache<key_t, item_t>::move_to_frequent(ListLocation loc, const key_t &key, const ListIter &list_iterator)
{
    const auto &tmp = *list_iterator;
    switch(loc)
    {
        case ListLocation::FIRST_LIST:
            move_to_dest_front(list_first_, list_frequent_, list_iterator, loc, key);
            break;

        case ListLocation::FIRST_LIST_GHOST:
            move_to_dest_front(list_first_ghost_, list_frequent_, list_iterator, loc, key);
            break;
        
        case ListLocation::FREQUENT_LIST_GHOST:
            move_to_dest_front(list_frequent_ghost_, list_frequent_, list_iterator, loc, key);
            break;
        
        case ListLocation::FREQUENT_LIST:
            return;
    }

    list_frequent_.push_front(tmp);
    update_cache_map(key, ListLocation::FREQUENT_LIST, list_frequent_.begin());
}

//---------------------------------------------------------------------------------------------------------------

template <typename key_t, typename item_t>
void
ARCCache<key_t, item_t>::handle_ghost(ListLocation location, key_t key, ListIter list_iterator)
{
    adapt_ghost(location);
    replace_for_adapt();
    move_to_frequent(location, key, list_iterator);
}

//---------------------------------------------------------------------------------------------------------------

template <typename key_t, typename item_t>
bool
ARCCache<key_t, item_t>::handle_existing_item(const CacheMapIter &cache_map_it, const key_t &key)
{      
    ListLocation location  = cache_map_it->second.location;
    ListIter list_iterator = cache_map_it->second.list_iter;

    switch(location)
    {
        case ListLocation::FIRST_LIST:
            move_to_frequent(location, key, list_iterator);             
            [[fallthrough]];
        case ListLocation::FREQUENT_LIST: 
            hits_counter_++;
            return true;
    
        case ListLocation::FIRST_LIST_GHOST:
            [[fallthrough]];
        case ListLocation::FREQUENT_LIST_GHOST:
            handle_ghost(location, key, list_iterator);
            return false;
            
        case ListLocation::NOT_FOUND:
            return false;
    };

    return false;
}

//---------------------------------------------------------------------------------------------------------------

template <typename key_t, typename item_t>
void
ARCCache<key_t, item_t>::handle_cache_overflow()
{
    size_t list1_size   = list_first_.size();
    size_t list1gh_size = list_first_ghost_.size();
    size_t list2_size   = list_frequent_.size();
    size_t list2gh_size = list_frequent_ghost_.size();


    size_t sum_size_lists = list1_size + list2_size + list1gh_size + list2gh_size;
    if (list1_size + list1gh_size == capacity_)
    {
        if (list1_size < capacity_)
        {
            if (!list_first_ghost_.empty())
            {
                cache_map_.erase(list_first_ghost_.back().key);
                list_first_ghost_.pop_back();
            }
            replace_for_adapt();
        }
        else if (!cache_map_.empty())
        {
            cache_map_.erase(list_first_.back().key);
            list_first_.pop_back();
        }
    }
    else if (list1_size + list1gh_size < capacity_ && sum_size_lists >= capacity_)      
    {    
        if ((sum_size_lists == 2 * capacity_) &&
            (!list_frequent_ghost_.empty())
        )
        { 
            cache_map_.erase(list_frequent_ghost_.back().key);
            list_frequent_ghost_.pop_back();       
        }
    
        replace_for_adapt();
    }
}

//---------------------------------------------------------------------------------------------------------------

template <typename key_t, typename item_t>
void
ARCCache<key_t, item_t>::add_new_item(const key_t &key, const item_t &item)
{
    handle_cache_overflow();

    list_first_.push_front(CacheEntry(key, item));
    update_cache_map(key, ListLocation::FIRST_LIST, list_first_.begin());
}

//---------------------------------------------------------------------------------------------------------------

template <typename key_t, typename item_t>
item_t
ARCCache<key_t, item_t>::get_item(const key_t &key) const
{
    CacheMapIter cache_map_it = cache_map_.find(key);
    
        if (cache_map_it == cache_map_.end()) return item_t();

    return cache_map_it->second.list_iter->item;
}

//---------------------------------------------------------------------------------------------------------------

template <typename key_t, typename item_t>
bool
ARCCache<key_t, item_t>::add_cache(const key_t &key, const item_t &item)
{
    if (capacity_ == 0) return false;

    CacheMapIter cache_map_it = cache_map_.find(key); 

    if (cache_map_it != cache_map_.end()) 
        return handle_existing_item(cache_map_it, key);

    add_new_item(key, item); 
    return false; 
}
        
//---------------------------------------------------------------------------------------------------------------

template <typename key_t, typename item_t>
size_t
ARCCache<key_t, item_t>::run_cache(const std::vector<std::pair<key_t, item_t>> &input_key_item)
{
    if (capacity_ <  0) return -1;
    if (capacity_ == 0) return  0;
    
    for (size_t i = 0; i < input_key_item.size(); i++)
        add_cache(input_key_item[i].first, input_key_item[i].second);

    return get_hit_count();
}

//---------------------------------------------------------------------------------------------------------------

template <typename key_t, typename item_t>
size_t
ARCCache<key_t, item_t>::get_hit_count() const
{
    return hits_counter_;
}

//---------------------------------------------------------------------------------------------------------------

#endif // ARC_CACHE_HPP
