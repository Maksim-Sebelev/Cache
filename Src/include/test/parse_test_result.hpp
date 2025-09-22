#ifndef PARSE_TEST_RESULT_HPP
#define PARSE_TEST_RESULT_HPP

//---------------------------------------------------------------------------------------------------------------

#include <cstdlib>
#include <iostream>

#include "get_test_data.hpp"


//---------------------------------------------------------------------------------------------------------------

enum class TestResult
{
    DONT_CHECK_RESULT = 0,
    TEST_PASSED          ,
    TEST_FAILED          ,
};


//---------------------------------------------------------------------------------------------------------------

template <typename input_t>
TestResult parse_test_result(size_t hits_count, test_data_t<input_t>* test_data)
{
    if (test_data->print_result_and_dont_check_answer())
    {
        std::cout << "cache hits quant = " << hits_count << std::endl;
        return TestResult::DONT_CHECK_RESULT;
    }

    const size_t test_answer = test_data->get_test_answer();

    if (test_answer == hits_count)
    {
        std::cout << "Test Passed" << std::endl;
        return TestResult::TEST_PASSED;
    }

    std::cout << "TEST FAILED:"                    << std::endl
              << "Cache hits     = "    << hits_count  << std::endl
              << "Correct answer = " << test_answer << std::endl;

    return TestResult::TEST_FAILED;
}

//---------------------------------------------------------------------------------------------------------------

#endif // PARSE_TEST_RESULT_HPP