#ifndef DRIVER_HPP
#define DRIVER_HPP

//---------------------------------------------------------------------------------------------------------------

#include <iostream>
#include <vector>
#include <memory>

#include "CacheInterface.hpp"
#include "get_test_data.hpp"

//---------------------------------------------------------------------------------------------------------------

template <typename key_t, typename item_t>
class CacheDriver
{
    private:
        // class typedefs
        using request_t = typename std::pair<key_t, item_t>;

        // class private variables
        std::vector<request_t> requests_;

    public:
        // ctor
        CacheDriver(const std::unique_ptr<test_data_t<key_t>>& test_data);

        // class public methods

        ssize_t                   run_cache        (CacheInterface<key_t, item_t>& cache);
        std::pair<size_t, size_t> compare_caches   (CacheInterface<key_t, item_t>& cache1, 
                                                    CacheInterface<key_t, item_t>& cache2);
};

// class public methods
//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------

//ctor
template <typename key_t, typename item_t>
CacheDriver<key_t, item_t>::CacheDriver(const std::unique_ptr<test_data_t<key_t>>& test_data)
{
    size_t requests_quant = test_data->get_input_size();
    requests_.resize(requests_quant);

    for (size_t i = 0; i < requests_quant; i++)
    {
        key_t  key  = test_data.get_i_element_of_data(i);
        item_t item = (item_t) key; // ДА ЭТО КРИНЖ, НО МЫ ХОТЕЛИ СДЕЛАТЬ ПО НАСТОЯЩЕМУ, И ИМИТИРОВАТЬ РАБОТУ С РЕАЛЬНЫМИ pages, НО У НАС ИХ НЕТ, ПОЭТОМУ БУДЕТ ТАК.
        requests_[i] = {key, item};
    }
}

//---------------------------------------------------------------------------------------------------------------

template <typename key_t, typename item_t>
ssize_t // returning type
CacheDriver<key_t, item_t>::run_cache(CacheInterface<key_t, item_t>& cache)
{
    cache.run_cache();
    return cache.get_hit_count();
}

//---------------------------------------------------------------------------------------------------------------

template <typename key_t, typename item_t>
std::pair<size_t, size_t>
CacheDriver<key_t, item_t>::compare_caches(CacheInterface<key_t, item_t>& cache1, 
                                           CacheInterface<key_t, item_t>& cache2)
{
    size_t first_cache_result  = run_cache(cache1);
    size_t second_cache_result = run_cache(cache2);

    std::pair<size_t, size_t> compare_results(first_cache_result,
                                              second_cache_result);
    return compare_results;
}
    
//---------------------------------------------------------------------------------------------------------------

#endif // DRIVER_HPP
