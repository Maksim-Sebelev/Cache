#include <iostream>
#include <cstdlib>
#include <vector>


#include "optimal_cache.hpp"
#include "check_args.hpp"
#include "test_files.hpp"
#include "read_test_and_answer.hpp"

//---------------------------------------------------------------------------------------------------------------

int main(int argc, const char* argv[])
{
    check_args(argc, argv); // exit 1 if not correct

    // argv[1] = test file
    // argv[2] = answer file
    test_files_t     test_files(argv[1], argv[2]); // argv[1] and argv[2] exist. it check function 'check_args'
    test_data_t<int> test_data (test_files);

    ON_DEBUG(
    test_data.dump();
    )

    const size_t capacity        = test_data.get_cache_size();
    const size_t amount_of_items = test_data.get_input_size();
    
    std::vector<std::pair<int, int>> input_key_item(amount_of_items);
    for (size_t i = 0; i < amount_of_items; i++)
    {    
        int item = test_data.get_i_element_of_data(i);
        int key  = item;

        input_key_item[i] = {key, item};
    } 
    
    OPT_cache<int, int> optimal_cache(capacity, amount_of_items, input_key_item);
    optimal_cache.run_optimal_cache();

    size_t hits_counter = optimal_cache.get_hit_count();
    size_t answer = test_data.get_test_answer();

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
