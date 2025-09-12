#include <iostream>
#include "arc_cache.hpp"
#include "optimal_cache.hpp"

//---------------------------------------------------------------------------------------------------------------

int main(int argc, char* argv[])
{
    std::size_t capacity = 0;
    std::size_t amount_numbers = 0;
    
    std::cin >> capacity >> amount_numbers;

    ARCCache<std::size_t, std::size_t> arc_cache(capacity);

    std::vector<std::pair<std::size_t, std::size_t>> input_key_item(amount_numbers); 

    for (std::size_t i = 0; i < amount_numbers; i++)
    {
        size_t key = 0;
        std::cin >> key;
        input_key_item[i] = {key, key};
    }

    std::size_t hits_counter = arc_cache.run_ARC_cache(input_key_item);
    std::cout << "hits: " << hits_counter << std::endl;

    std::cin >> capacity >> amount_numbers; 

    std::vector<std::pair<int, int>> input_items(amount_numbers);

    for (std::size_t i = 0; i < amount_numbers;i++)
    {
        std::cin >> input_items[i].first;
        input_items[i].second = input_items[i].first;
    }

    OPT_cache<int, int> optimal_cache(capacity, amount_numbers, input_items);

    optimal_cache.run_optimal_cache();

    std::cout << "hits: " << optimal_cache.get_hit_count() << std::endl;

    return 0;
}

//---------------------------------------------------------------------------------------------------------------
