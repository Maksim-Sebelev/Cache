#ifndef ARC_CACHE_HPP
#define ARC_CACHE_HPP

//---------------------------------------------------------------------------------------------------------------

#include <algorithm>
#include <iostream>
#include <list>
#include <vector>
#include <unordered_map>
#include <cassert>

#include "global.hpp"

//---------------------------------------------------------------------------------------------------------------

// #include "cache_interface.hpp"

//---------------------------------------------------------------------------------------------------------------

template <typename key_t, typename item_t> 
class ARCCache
{
    private:
        // class private definition
        enum class ListLocation
        {
            FIRST_LIST         ,
            FREQUENT_LIST      ,
            FIRST_LIST_GHOST   ,
            FREQUENT_LIST_GHOST,
            NOT_FOUND          ,
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

        void adapt_ghost           (      ListLocation   location                                                                       );
        bool not_empty_and_adaptive(                                                                                                    );
        void move_tail_to_front    (      ListType     & src          ,       ListType    &  dest    ,       ListLocation  dest_location);
        void replace_for_adapt     (                                                                                                    );
        void update_cache_map      (const key_t        & key          ,       ListLocation   location,       ListIter      list_it      );
        void move_to_dest_front    (      ListType     & src          ,       ListType    &  dest    , 
                                          ListIter       list_iterator, const ListLocation&  location, const key_t       & key          );
        void move_to_frequent      (      ListLocation   loc          , const key_t       &  key     , const ListIter    & list_iterator);
        void handle_ghost          (      ListLocation   location     , key_t                key     ,       ListIter      list_iterator);
        bool handle_existing_item  (const CacheMapIter & cache_map_it , const key_t       &  key                                        );
        void handle_cache_overflow (                                                                                                    );
        void add_new_item          (const key_t        & key          , const item_t      &  item                                       );

        ON_DEBUG(
        void print_header          (const std::string  & word                                                                           );
        void print_separator       (                                                                                                    );
        void print_location        (      ListLocation   loc                                                                            );
        void cache_map_dump        (const CacheMapType & cache_map                                                                      );
        void list_header           (const ListLocation   which_list                                                                     );
        void list_dump             (const ListType     & list         , const ListLocation   which_list                                 );
        )

    public:
        // class public methods

               ARCCache     (      size_t                capacity                          ); // ctor 1
        item_t get_item     (const key_t               & key                               );
        bool   add_cache    (const key_t               & key           , const item_t &item);
        size_t run_cache    (const std::vector<item_t> & requests                          );
        size_t get_hit_count(                                                              );
        ON_DEBUG(
        void   dump         (                                                              );
        )
};

//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------

// ctor
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
ARCCache<key_t, item_t>::not_empty_and_adaptive()
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
    CacheEntry element = *list_iterator;
    src.erase(list_iterator);

    dest.push_front(element);
    update_cache_map(key, location, dest.begin());
}

//---------------------------------------------------------------------------------------------------------------

template <typename key_t, typename item_t>
void
ARCCache<key_t, item_t>::move_to_frequent(ListLocation loc, const key_t &key, const ListIter &list_iterator)
{
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

        default: builtin_unreachable_wrapper("we must not be here"); // FIXME
    }

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

        default: break;
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
ARCCache<key_t, item_t>::get_item(const key_t &key)
{
    CacheMapIter cache_map_it = cache_map_.find(key);
    
    if (cache_map_it == cache_map_.end())
        return item_t();

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
ARCCache<key_t, item_t>::run_cache(const std::vector<item_t>& requests)
{
    const size_t requests_quant = requests.size();

    for (size_t i = 0; i < requests_quant; i++)
    {
        key_t key_tmp = requests[i];
        add_cache(key_tmp, requests[i]);
    }

    return hits_counter_;
}

//---------------------------------------------------------------------------------------------------------------

template <typename key_t, typename item_t>
size_t
ARCCache<key_t, item_t>::get_hit_count()
{
    return hits_counter_;
}

//---------------------------------------------------------------------------------------------------------------

ON_DEBUG(

//---------------------------------------------------------------------------------------------------------------

template <typename key_t, typename item_t>
void
ARCCache<key_t, item_t>::print_header(const std::string &word)
{
    std::cout << "============ " << word << " ============\n\n";
}

//---------------------------------------------------------------------------------------------------------------

template <typename key_t, typename item_t>
void
ARCCache<key_t, item_t>::print_separator()
{
    std::cout << "\n=======================================\n\n";
}

//---------------------------------------------------------------------------------------------------------------

template <typename key_t, typename item_t>
void
ARCCache<key_t, item_t>::print_location(ListLocation loc)
{
    switch(loc)
    {
        case ListLocation::FIRST_LIST:
            std::cout << "First occur list";
            break;

        case ListLocation::FREQUENT_LIST:
            std::cout << "Frequent occur list";
            break;

        case ListLocation::FIRST_LIST_GHOST:
            std::cout << "First occur ghost_list";
            break;

        case ListLocation::FREQUENT_LIST_GHOST:
            std::cout << "Frequent occur ghost_list";
            break;

        case ListLocation::NOT_FOUND:
            std::cout << "ELEMENT WASN`T FOUND IN ANY LIST";
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------

template <typename key_t, typename item_t>
void
ARCCache<key_t, item_t>::cache_map_dump(const CacheMapType &cache_map)
{

    print_header("cache_map DUMP");

    for (const auto &pair : cache_map) // pair = cache_map.begin -> cache_map.end() <=> pair - cache_map_it
    {
        const LocationInfo &loc_info = pair.second; // pair.second - iterator 
        const ListIter     &list_it  = loc_info.list_iter;

        std::cout << "key = " << pair.first << ": " << "item location: ";

        print_location(loc_info.location);

        if (list_it != ListIter())
        {
            std::cout << " item: " << list_it->item;
            std::cout << " key by cache info: " << list_it->key; 
        }
        else 
            std::cout << " INVALID ITERATOR";

        std::cout << std::endl;
    }
    print_separator();
}

//---------------------------------------------------------------------------------------------------------------

template <typename key_t, typename item_t>
void
ARCCache<key_t, item_t>::list_header(const ListLocation which_list)
{
    switch(which_list)
    {
        case ListLocation::FIRST_LIST:
            print_header("First occur list DUMP");
            break;

        case ListLocation::FREQUENT_LIST:
            print_header("Frequent occur list DUMP");
            break;

        case ListLocation::FIRST_LIST_GHOST:
            print_header("First occur list_ghost DUMP");
            break;

        case ListLocation::FREQUENT_LIST_GHOST:
            print_header("Frequent occur list_ghost DUMP");
            break;

        case ListLocation::NOT_FOUND:
            print_header("UNDEFINED LIST");
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------

template <typename key_t, typename item_t>
void
ARCCache<key_t, item_t>::list_dump(const ListType &list, const ListLocation which_list)
{
    list_header(which_list);


    std::size_t i = 0;
    std::size_t size = list.size();
    for (const auto &cache : list) // pair => list.begin() == ListIter
    {
        item_t item = cache.item;
        key_t  key  = cache.key;

        std::cout << "[ item" << i++ <<" = " << item <<"(cache_map key: " << key <<  ") ]";
        if (i < size) std::cout << "<==>";
    }

    std::cout.put('\n');
    print_separator();
}

//---------------------------------------------------------------------------------------------------------------

template <typename key_t, typename item_t>
void
ARCCache<key_t, item_t>::dump()
{
    print_header("Adaptive replacement cache DUMP");
    
    std::cout << "capacity: " << capacity_ << std::endl;
    std::cout << "hit count: " << hits_counter_ << std::endl;
    std::cout << "adadptive parametr:" << static_cast<double>(adapt_param_) << std::endl;

    cache_map_dump(cache_map_);

    list_dump(list_first_, ListLocation::FIRST_LIST);
    list_dump(list_frequent_, ListLocation::FREQUENT_LIST);
    list_dump(list_first_ghost_, ListLocation::FIRST_LIST_GHOST);
    list_dump(list_frequent_ghost_, ListLocation::FREQUENT_LIST_GHOST);

    print_separator();
}

//---------------------------------------------------------------------------------------------------------------

) // ON_DEBUG

//---------------------------------------------------------------------------------------------------------------


//---------------------------------------------------------------------------------------------------------------

#endif // ARC_CACHE_HPP
