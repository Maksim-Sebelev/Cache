#ifndef TEST_READ_TEST_HPP
#define TEST_READ_TEST_HPP

//---------------------------------------------------------------------------------------------------------------

#include <cstdlib>
#include <fstream>
#include <cassert>

//---------------------------------------------------------------------------------------------------------------

template <typename input_t>
struct test_input_t
{
    public:
        // struct private variables
        size_t  get_cache_size       ();
        size_t  get_input_size       ();
        input_t get_i_element_of_data(size_t i);

        // ctor
        test_input_t(const char* test_file);

        // dtor
       ~test_input_t();

    private:
        // struct private vairbales
        size_t        cache_size_;
        size_t        input_size_;
        input_t*      data_      ;
        std::ifstream file_      ;
        const char*   test_file_ ;

        // struct private methods
        void          open_test_file                         ();
        void          read_cache_size                        ();
        void          read_input_size                        ();
        void          read_test_data                         ();
        void          close_test_file                        ();

        bool          is_test_empty                          ();
        bool          is_index_out_of_range                  (size_t index);

        __attribute__ ((noreturn))
        void          failed_open_test_file                  ();
        __attribute__ ((noreturn))
        void          failed_read_cache_size                 ();
        __attribute__ ((noreturn))
        void          failed_read_input_size                 ();
        __attribute__ ((noreturn))
        void          failed_allocate_memory_for_input_array ();
        __attribute__ ((noreturn))
        void          failed_read_i_page                     (size_t number_of_bad_page);
        __attribute__ ((noreturn))
        void          index_out_of_range                     (size_t index);
};


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
    read_test_data ();
    close_test_file();

}

//---------------------------------------------------------------------------------------------------------------

// dtor
template <typename input_t>
test_input_t<input_t>::~test_input_t()
{    
    if (!data_)
        return;

    delete data_;
}
//---------------------------------------------------------------------------------------------------------------

// public methods
//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------
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
//---------------------------------------------------------------------------------------------------------------
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
    if (is_test_empty()) return; // no allocation memory for empty test

    data_ = new input_t[input_size_];

    if (!data_)
        failed_allocate_memory_for_input_array(); // exit 1

    for (size_t input_iter = 0; input_iter < input_size_; input_iter++)
    {
        file_ >> data_[input_iter];
        
        if (!file_.fail()) continue;;
    
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
bool test_input_t<input_t>::is_test_empty()
{
    return (input_size_ == 0);
}

//---------------------------------------------------------------------------------------------------------------

template <typename input_t>
bool test_input_t<input_t>::is_index_out_of_range(size_t index)
{
    return (index >= input_size_);
}

//---------------------------------------------------------------------------------------------------------------


#endif // TEST_READ_TEST_HPP