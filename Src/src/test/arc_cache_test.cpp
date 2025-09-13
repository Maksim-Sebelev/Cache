#include <iostream>
#include <cstdlib>


#include "arc_cache.hpp"
#include "check_args.hpp"
#include "test_files.hpp"
#include "read_test_and_answer.hpp"

//---------------------------------------------------------------------------------------------------------------

int main(int argc, char* argv[])
{
    check_args(argc, argv); // exit 1 if not correct

    // argv[1] = test file
    // argv[2] = answer file
    test_files_t     test_files(argv[1], argv[2]); // argv[1] and argv[2] exist. it check function 'check_args'
    test_data_t<int> test_data (test_files);

    ON_DEBUG(
    test_data.dump();
    )

    test_data.get_cache_size       ();
    test_data.get_input_size       ();
    test_data.get_i_element_of_data(10);
    test_data.get_test_answer      ();

    // тема, вот здесь делай функцию для теста оптимального хеша

    return EXIT_SUCCESS;
}

//---------------------------------------------------------------------------------------------------------------
