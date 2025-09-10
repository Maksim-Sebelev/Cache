#include <iostream>
#include <cstdlib>

#include "test/check_args.hpp"
#include "test/read_answer.hpp"
#include "test/test_files.hpp"
#include "test/read_test_and_answer.hpp"

//---------------------------------------------------------------------------------------------------------------

using input_t = int;

int main(int argc, char* argv[])
{
    check_args(argc, argv); // exit 1 if not correct

    test_files_t test_files(argv[1], argv[2]); // argv[1] and argv[2] exist. it check function 'check_args'

    test_data_t<input_t> test_data(test_files);

    return EXIT_SUCCESS;
}

//---------------------------------------------------------------------------------------------------------------
