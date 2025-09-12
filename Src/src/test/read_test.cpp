#include <fstream>
#include <iostream>
#include <cassert>
#include <cstdlib>

#include "read_test.hpp"

//---------------------------------------------------------------------------------------------------------------

// ctor
template <typename input_t>
test_input_t<input_t>::test_input_t(const char* test_file)
{
    assert(test_file);

    test_file_ = test_file;

    open_test_file ();
    read_cache_size();
    read_input_size();

    if (is_test_empty(input_size_)) return; // no allocation memory for empty test

    read_test_data();
}

//---------------------------------------------------------------------------------------------------------------

// dtor

template <typename input_t>
test_input_t<input_t>::~test_input_t()
{
    if (!data_)
        return; // if nullptr => empty test and no allocation memory

    delete data_;
}

//---------------------------------------------------------------------------------------------------------------

// public methods
//---------------------------------------------------------------------------------------------------------------

template <typename input_t>
size_t test_input_t<input_t>::get_cache_size()
{
    return cache_size_;
}

//---------------------------------------------------------------------------------------------------------------

template <typename input_t>
size_t test_input_t<input_t>::get_input_size()
{
    return input_size_;
}

//---------------------------------------------------------------------------------------------------------------

template <typename input_t>
input_t test_input_t<input_t>::get_i_element_of_data(size_t i)
{
    if (is_index_out_of_range(i))
        index_out_of_range(i);

    return data_[i];
}

//---------------------------------------------------------------------------------------------------------------

// private methods
//---------------------------------------------------------------------------------------------------------------

template <typename input_t>
void test_input_t<input_t>::open_test_file()
{
    file_.open(test_file_);
    
    if (file_.fail())
        failed_open_test_file();
}
//---------------------------------------------------------------------------------------------------------------

template <typename input_t>
void test_input_t<input_t>::read_cache_size()
{
    file_ >> cache_size_;

    if (file_.fail())
        failed_read_cache_size(); // exit 1
}
//---------------------------------------------------------------------------------------------------------------

template <typename input_t>
void test_input_t<input_t>::read_input_size()
{
    file_ >> input_size_;

    if (file_.fail())
        failed_read_input_size(); // exit 1
}

//---------------------------------------------------------------------------------------------------------------

template <typename input_t>
void test_input_t<input_t>::close_test_file()
{
    file_.close();
}

//---------------------------------------------------------------------------------------------------------------

template <typename input_t>
void test_input_t<input_t>::read_test_data()
{
    data_ = new input_t(input_size_);

    if (!data_)
        failed_allocate_memory_for_input_array(); // exit 1

    for (size_t input_iter = 0; input_iter < input_size_; input_iter++)
    {
        file_ >> data_[input_iter];
        
        if (!file_.fail()) break;
    
        failed_read_i_page(input_iter); // exit 1
    }
}

//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------

template <typename input_t>
__attribute__ ((noreturn))
void test_input_t<input_t>::failed_open_test_file()
{
    assert(test_file_);

    std::cerr << "Failed open '" << test_file_ << "'"
              << std::endl;

    exit(EXIT_FAILURE);
}

//---------------------------------------------------------------------------------------------------------------

template <typename input_t>
__attribute__ ((noreturn))
void test_input_t<input_t>::failed_read_cache_size()
{
    assert(test_file_);

    std::cerr << "Failed read cache size from '" << test_file_ << "'"
              << std::endl;

    exit(EXIT_FAILURE);
}

//---------------------------------------------------------------------------------------------------------------

template <typename input_t>
__attribute__ ((noreturn))
void test_input_t<input_t>::failed_read_input_size()
{
    assert(test_file_);

    std::cerr << "Failed read input size from '" << test_file_ << "'"
              << std::endl;

    exit(EXIT_FAILURE);
}

//---------------------------------------------------------------------------------------------------------------

template <typename input_t>
__attribute__ ((noreturn))
void test_input_t<input_t>::failed_allocate_memory_for_input_array()
{
    assert(test_file_);

    std::cerr << "Failed allocate for input from '" << test_file_
              << "'" << std::endl;

    exit(EXIT_FAILURE);
}

//---------------------------------------------------------------------------------------------------------------

template <typename input_t>
__attribute__ ((noreturn))
void test_input_t<input_t>::failed_read_i_page(size_t number_of_bad_page)
{
    assert(test_file_);

    std::cerr << "failed read " << number_of_bad_page << " page from '"
              << test_file_ << "'" << std::endl;

    exit(EXIT_FAILURE);
}

//---------------------------------------------------------------------------------------------------------------

template <typename input_t>
__attribute__ ((noreturn))
void test_input_t<input_t>::index_out_of_range(size_t index)
{
    assert(test_file_);

    std::cerr << "truing to get no exists element in input." << std::endl
    << "your index = " << index << std::endl
    << "data size  = " << input_size_ << std::endl;

    exit(EXIT_FAILURE);
}

//---------------------------------------------------------------------------------------------------------------

template <typename input_t>
bool test_input_t<input_t>::is_test_empty(size_t input_size)
{
    return (input_size == 0);
}

//---------------------------------------------------------------------------------------------------------------

template <typename input_t>
bool test_input_t<input_t>::is_index_out_of_range(size_t index)
{
    return (index >= input_size_);
}

//---------------------------------------------------------------------------------------------------------------

/*
    ЭТО ДИЧАЙШИЙ КРИНЖ
    НО Я ЭТО СДЕЛАЛ ДО ТОГО
    КАК УЗНАЛ ЧТО РЕЛИЗОВЫВАТЬ
    МЕТОДЫ В .cpp БЫЛО ПЛОХОЙ ИДЕЕЙ
    Я РАСКАИВАЮСЬ И
    БОЛЬШЕ НИКОГДА ТАК ДЕЛАТЬ
    НЕ БУДУ!!! ОБЕЩАЮ
    НЕ БЕЙТЕ МЕНЯ  
*/

template struct test_input_t<int>;

//---------------------------------------------------------------------------------------------------------------
