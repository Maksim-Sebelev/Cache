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

       size_t  get_test_answer      ();
       size_t  get_cache_size       ();
       size_t  get_input_size       ();
       input_t get_i_element_of_data(size_t i);
};

//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------

template <typename input_t>
test_data_t<input_t>::test_data_t(const test_files_t& test_files) :
test_input_(test_files.test_file_), test_answer_(test_files.answer_file_)
{}

//---------------------------------------------------------------------------------------------------------------

template <typename input_t>
test_data_t<input_t>::~test_data_t()
{
    test_input_ .~test_input_t ();
    test_answer_.~test_answer_t();
}

//---------------------------------------------------------------------------------------------------------------

template <typename input_t>
size_t test_data_t<input_t>::get_test_answer()
{
    return test_answer_.get_test_answer();
}

//---------------------------------------------------------------------------------------------------------------

template <typename input_t>
size_t test_data_t<input_t>::get_cache_size()
{
    return test_input_.get_cache_size();
}

//---------------------------------------------------------------------------------------------------------------

template <typename input_t>
size_t test_data_t<input_t>::get_input_size()
{
    return test_input_.get_input_size();
}

//---------------------------------------------------------------------------------------------------------------

template <typename input_t>
input_t test_data_t<input_t>::get_i_element_of_data(size_t i)
{
    return test_input_.get_i_element_of_data(i);
}

//---------------------------------------------------------------------------------------------------------------

#endif // TEST_TEST_AND_ANSWER_HPP