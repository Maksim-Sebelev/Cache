#ifndef TEST_TEST_AND_ANSWER_HPP
#define TEST_TEST_AND_ANSWER_HPP

//---------------------------------------------------------------------------------------------------------------

#include <cstdlib>
#include "global/global.hpp"
#include "test/read_test.hpp"
#include "test/read_answer.hpp"
#include "test/test_files.hpp"

//---------------------------------------------------------------------------------------------------------------

template <typename input_t>
struct test_data_t 
{
    test_input_t  test_input_ ;
    test_answer_t test_answer_;

    test_data_t(const test_files_t& test_files);
};

//---------------------------------------------------------------------------------------------------------------

#endif // TEST_TEST_AND_ANSWER_HPP