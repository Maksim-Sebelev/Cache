#include <fstream>
#include <iostream>
#include <cassert>
#include <cstdlib>

#include "test/read_test.hpp"

namespace
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


    input_array_ = new input_t(input_size);

    if (!input_array_)
        failed_allocate_memory_for_input_array(test_file); // exit 1

    for (size_t i = 0; i < input_size; i++)
    {
        file >> input_array_[i];
        
        if (file.fail())
            failed_read_i_page(test_file); // exit 1
    }
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

} // namespace
