#include <iostream>
#include <cstdlib>
#include <memory>

#include "optimal_cache.hpp"
#include "get_test_data.hpp"

//---------------------------------------------------------------------------------------------------------------

using input_t = int;

//---------------------------------------------------------------------------------------------------------------

int main(int argc, char* argv[])
{
    std::unique_ptr<test_data_t<input_t>> test_data = get_test_data<input_t>(argc, argv);

    ON_DEBUG(
    test_data->dump();
    )

    const size_t capacity        = test_data->get_cache_size();
    const size_t amount_of_items = test_data->get_input_size();
    
    std::vector<std::pair<int, int>> input_key_item(amount_of_items);
    for (size_t i = 0; i < amount_of_items; i++)
    {    
        int item = test_data->get_i_element_of_data(i);
        int key  = item;

        input_key_item[i] = {key, item};
    } 
    
    OPT_cache<int, int> optimal_cache(capacity, amount_of_items, input_key_item);
    optimal_cache.run_optimal_cache();

    size_t hits_counter = optimal_cache.get_hit_count();
    size_t answer = test_data->get_test_answer();

    if (answer != hits_counter)  
    {
        std::cout << "EXIT FAILURE: ANSWERS WRONG!!!\n";
        std::cout << "hit counter: " << hits_counter << "\nanswer:      " << answer;
        std::cout << std::endl;
        
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

//---------------------------------------------------------------------------------------------------------------
