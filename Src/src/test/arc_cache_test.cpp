#include <iostream>
#include <cstdlib>
#include <vector>
#include <memory>

#include "arc_cache.hpp"
#include "check_args.hpp"
#include "test_files.hpp"
#include "read_test_and_answer.hpp"
#include "parse_args.hpp"


//---------------------------------------------------------------------------------------------------------------

using input_t = int;

//---------------------------------------------------------------------------------------------------------------

int main(int argc, char* argv[])
{
    // check_args(argc, argv); // exit 1 if not correct
    
    flags_parser parsing_flags_result(argc, argv);

    input_sream  type_of_input_sream = parsing_flags_result.get_input_stream();

    std::unique_ptr<test_data_t<input_t>> test_data;

    switch (type_of_input_sream)
    {
        case input_sream::standart_input:
        {
            test_data = std::make_unique<test_data_t<input_t>>();
            break;
        }

        case input_sream::dat_file_stream:
        {
            test_files_t test_files = parsing_flags_result.get_test_files();
            test_data = std::make_unique<test_data_t<input_t>>(test_files);
        }
    }

    test_data->dump();
    // argv[1] = test file
    // argv[2] = answer file
    // test_files_t     test_files(argv[1], argv[2]); // argv[1] and argv[2] exist. it check function 'check_args'
    // test_data_t<int> test_data (test_files);

    // ON_DEBUG(
    // test_data.dump();
    // )



    // const size_t capacity        = test_data.get_cache_size();
    // const size_t amount_of_items = test_data.get_input_size();
    

    // std::vector<std::pair<int, int>> input_key_item(amount_of_items);
    // for (size_t i = 0; i < amount_of_items; i++)
    // {    
    //     int item = test_data.get_i_element_of_data(i);
    //     int key  = item;

    //     input_key_item[i] = {key, item};
    // }
    
    // ARCCache<int, int> arc_cache(capacity);
    // arc_cache.run_ARC_cache(input_key_item);

    // size_t hits_counter = arc_cache.get_hit_count();
    // size_t answer = test_data.get_test_answer();

    // if (answer != hits_counter) 
    // {
    //     std::cout << "EXIT FAILURE: ANSWERS WRONG!!!\n";
    //     std::cout << "hit counter: " << hits_counter << "\nanswer:      " << answer;
    //     std::cout << std::endl;
        
    //     return EXIT_FAILURE;
    // }

    return EXIT_SUCCESS;
}

//---------------------------------------------------------------------------------------------------------------
