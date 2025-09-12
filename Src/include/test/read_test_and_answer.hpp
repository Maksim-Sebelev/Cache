#ifndef TEST_TEST_AND_ANSWER_HPP
#define TEST_TEST_AND_ANSWER_HPP

//---------------------------------------------------------------------------------------------------------------

#include <cstdlib>
#include "read_test.hpp"
#include "read_answer.hpp"
#include "test_files.hpp"

//---------------------------------------------------------------------------------------------------------------

template <typename input_t>
struct test_data_t
{
    private:
        // private struct variables
        test_input_t<input_t> test_input_ ;
        test_answer_t         test_answer_;

    public:
        // public struct methods
        // ctor
        test_data_t(const test_files_t& test_files);

        // dtor
       ~test_data_t();
};

//---------------------------------------------------------------------------------------------------------------

#endif // TEST_TEST_AND_ANSWER_HPP