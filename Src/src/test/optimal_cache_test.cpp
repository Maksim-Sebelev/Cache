#include <iostream>
#include <cstdlib>
#include <vector>

#include "optimal_cache.hpp"

//---------------------------------------------------------------------------------------------------------------

int main()
{
    size_t capacity        = 0;
    size_t amount_of_items = 0;
    
    std::cin >> capacity >> amount_of_items;

    std::vector<std::pair<int, int>> input_key_item(amount_of_items);
    for (size_t i = 0; i < amount_of_items; i++)
    {    
        int item = 0;
        std:: cin >> item; 

        int key = item;

        input_key_item[i] = {key, item};
    }

    OPT_cache<int, int> optimal_cache(capacity, amount_of_items);
    optimal_cache.run_optimal_cache(input_key_item);

    size_t hits_counter = optimal_cache.get_hit_count();
    
    std::cout << hits_counter << std::endl;

    return EXIT_SUCCESS;
}

//---------------------------------------------------------------------------------------------------------------
