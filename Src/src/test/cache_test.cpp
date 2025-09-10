#include <iostream>
#include <cstdlib>

#include "test/check_args.hpp"
#include "test/read_answer.hpp"
#include "test/test_struct.hpp"

//---------------------------------------------------------------------------------------------------------------


int main(int argc, char* argv[])
{
    check_args(argc, argv); // exit 1 if not correct

    test_files_t test_files(argv[1], argv[2]); // argv[1] and argv[2] exist. it check function 'check_args'
    



    return EXIT_SUCCESS;
}

//---------------------------------------------------------------------------------------------------------------
