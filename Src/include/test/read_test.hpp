#ifndef TEST_READ_TEST_HPP
#define TEST_READ_TEST_HPP

//---------------------------------------------------------------------------------------------------------------

#include <cstdlib>

//---------------------------------------------------------------------------------------------------------------

template <typename input_t>
struct test_input_t
{
    size_t   cache_size_;
    size_t   input_size_;
    input_t* input_     ;

     test_input_t(const char* test_file);
    ~test_input_t();
};

//---------------------------------------------------------------------------------------------------------------

#endif // TEST_READ_TEST_HPP