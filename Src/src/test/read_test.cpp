#include <fstream>
#include <iostream>
#include <cassert>
#include <cstdlib>

#include "test/read_test.hpp"

//---------------------------------------------------------------------------------------------------------------

namespace // namespace for only ctor needed 
{

__attribute__ ((noreturn))
static void failed_open_test_file                  (const char* test_file);

__attribute__ ((noreturn))
static void failed_read_cache_size                 (const char* test_file);

__attribute__ ((noreturn))
static void failed_read_input_size                 (const char* test_file);

__attribute__ ((noreturn))
static void failed_allocate_memory_for_input_array (const char* test_file);

__attribute__ ((noreturn))
static void failed_read_i_page                     (const char* test_file, size_t number_of_bad_page);

static bool is_test_empty                          (size_t input_size);

} // namespace

//---------------------------------------------------------------------------------------------------------------

template <typename input_t>
test_input_t<input_t>::test_input_t(const char* test_file)
{
    assert(test_file);

    std::ifstream file(test_file);

    if (!file.is_open)
        failed_open_test_file(test_file); // exit 1

    file >> cache_size_;

    if (file.fail())
        failed_read_cache_size(test_file); // exit 1


    file >> input_size_;

    if (file.fail())
        failed_read_input_size(test_file); // exit 1

    if (is_test_empty(input_size_))
       return; // no allocation memory for empty test

    assert(input_size_ >= 1);

    input_array_ = new input_t(input_size);

    if (!input_array_)
        failed_allocate_memory_for_input_array(test_file); // exit 1

    for (size_t input_iter = 0; input_iter < input_size; input_iter++)
    {
        file >> input_array_[input_iter];
        
        if (file.fail())
            failed_read_i_page(test_file, input_iter); // exit 1
    }
}

//---------------------------------------------------------------------------------------------------------------

template <typename input_t>
test_input_t<input_t>::~test_input_t()
{
    if (!input_)
        return; // if nullptr => empty test and no allocation memory

    delete input_;
}

//---------------------------------------------------------------------------------------------------------------

namespace // namespace for only ctor needed functions
{
//---------------------------------------------------------------------------------------------------------------

__attribute__ ((noreturn))
static void failed_open_test_file(const char* test_file)
{
    assert(test_file);

    std::cerr << "Failed open '" << test_file << "'"
              << std::endl;

    exit(EXIT_FAILURE);
}


//---------------------------------------------------------------------------------------------------------------

__attribute__ ((noreturn))
static void failed_read_cache_size(const char* test_file)
{
    assert(test_file);

    std::cerr << "Failed read cache size from '" << test_file << "'"
              << std::endl;

    exit(EXIT_FAILURE);
}

//---------------------------------------------------------------------------------------------------------------

__attribute__ ((noreturn))
static void failed_read_input_size(const char* test_file)
{
    assert(test_file);

    std::cerr << "Failed read input size from '" << test_file << "'"
              << std::endl;

    exit(EXIT_FAILURE);
}

//---------------------------------------------------------------------------------------------------------------

__attribute__ ((noreturn))
static void failed_allocate_memory_for_input_array(const char* test_file)
{
    assert(test_file);

    std::cerr << "Failed allocate for input from '" << test_file
              << "'" << std::endl;

    exit(EXIT_FAILURE);
}

//---------------------------------------------------------------------------------------------------------------

__attribute__ ((noreturn))
static void failed_read_i_page(const char* test_file, size_t number_of_bad_page)
{
    assert(test_file);

    std::cerr << "failed read " << number_of_bad_page << " page from '"
              << test_file << "'" << std::endl;

    exit(EXIT_FAILURE);
}

//---------------------------------------------------------------------------------------------------------------

static bool is_test_empty(size_t input_size)
{
    return (input_size == 0);
}

//---------------------------------------------------------------------------------------------------------------

} // namespace
