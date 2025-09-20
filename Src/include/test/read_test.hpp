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
        test_input_t(); // ctor for reading from stdin
        test_input_t(const std::string& test_file); // ctor for reading from .dat file

        // dtor
       ~test_input_t();

    private:
        // struct private vairbales
        size_t        cache_size_ ;
        size_t        input_size_ ;
        input_t*      data_       ;
        std::ifstream file_       ;
        std::string   test_file_  ;

        // struct private methods

        bool          is_index_out_of_range                             (size_t index);
    
        __attribute__ ((noreturn))
        void          index_out_of_range                                (size_t index);
    
        // struct private methods for reading from stdin (read_from_stdin = true)

        void          read_cache_size_from_stdin                        ();
        void          read_input_size_from_stdin                        ();
        void          read_test_data_from_stdin                         ();


        __attribute__ ((noreturn))
        void          failed_read_cache_size_from_stdin                 ();
        __attribute__ ((noreturn))
        void          negative_or_to_big_cache_size_from_stdin          ();
        __attribute__ ((noreturn))
        void          negative_or_to_big_input_size_from_stdin          ();
        __attribute__ ((noreturn))
        void          failed_read_input_size_from_stdin                 ();
        __attribute__ ((noreturn))
        void          failed_allocate_memory_for_input_array_from_stdin ();
        __attribute__ ((noreturn))
        void          failed_read_i_page_from_stdin                     (size_t number_of_bad_page);
    
    
        // struct private methods for reading from .dat file (read_from_stdin = false)

        void          open_test_file                                    ();
        void          read_cache_size_from_dat                          ();
        void          read_input_size_from_dat                          ();
        void          read_test_data_from_dat                           ();
        void          close_test_file                                   ();

        bool          is_test_empty                                     ();

        __attribute__ ((noreturn))
        void          failed_open_test_file                             ();
        __attribute__ ((noreturn))
        void          failed_read_cache_size_from_dat                   ();
        __attribute__ ((noreturn))
        void          negative_or_to_big_cache_size_in_dat              ();
        __attribute__ ((noreturn))
        void          negative_or_to_big_input_size_from_dat            ();
        __attribute__ ((noreturn))
        void          failed_read_input_size_from_dat                   ();
        __attribute__ ((noreturn))
        void          failed_allocate_memory_for_input_array_from_dat   ();
        __attribute__ ((noreturn))
        void          failed_read_i_page_from_dat                       (size_t number_of_bad_page);
};


//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------

//ctor for reading from stdin
// no arguments
template <typename input_t>
test_input_t<input_t>::test_input_t()
{
    read_cache_size_from_stdin();
    read_input_size_from_stdin();
    read_test_data_from_stdin ();
}

//---------------------------------------------------------------------------------------------------------------

// ctor for reading from .dat
// arg = name of .dat file
template <typename input_t>
test_input_t<input_t>::test_input_t(const std::string& test_file) :
test_file_(test_file)
{
    open_test_file          ();
    read_cache_size_from_dat();
    read_input_size_from_dat();
    read_test_data_from_dat ();
    close_test_file         ();
}

//---------------------------------------------------------------------------------------------------------------

// dtor
template <typename input_t>
test_input_t<input_t>::~test_input_t()
{    
    if (!data_)
        return;

    delete[] data_;
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
bool test_input_t<input_t>::is_index_out_of_range(size_t index)
{
    return (index >= input_size_);
}

//---------------------------------------------------------------------------------------------------------------

template <typename input_t>
__attribute__ ((noreturn))
void test_input_t<input_t>::index_out_of_range(size_t index)
{
    std::cerr << "trying to get no exists element in input." << std::endl
    << "your index = " << index << std::endl
    << "data size  = " << input_size_ << std::endl;

    exit(EXIT_FAILURE);
}

//---------------------------------------------------------------------------------------------------------------


// private methods for reading from stdin
//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------

template <typename input_t>
void test_input_t<input_t>::read_cache_size_from_stdin()
{
    signed long long int signed_cache_size_ = 0;

    // будет ли оправдан такой статик ассерт? 
    // static_assert(sizeof(signed_cache_size_) == sizeof(cache_size_),
    // "if this sizes are not equal, we can`t report wrong about bad input"
    // "on your device you can ignore it, or made signed_cache_size equal to sizeof(size_t)"
    // );

    std::cin >> signed_cache_size_;

    if (signed_cache_size_ < 0)
        negative_or_to_big_cache_size_in_stdin();

    if (std::cin.fail())
        failed_read_cache_size_from_stdin(); // exit 1

    cache_size_ = static_cast<size_t> (signed_cache_size_); // here we are sure, that signed_cache_size_
}

//---------------------------------------------------------------------------------------------------------------

template <typename input_t>
void test_input_t<input_t>::read_input_size_from_stdin()
{
    signed long long int signed_input_size_ = 0;

    // будет ли оправдан такой статик ассерт? 
    // static_assert(sizeof(signed_input_size_) == sizeof(input_size_),
    // "if this sizes are not equal, we can`t report wrong about bad input"
    // "on your device you can ignore it, or made signed_cache_size_ equal to sizeof(size_t)"
    // );

    std::cin >> signed_input_size_;

    if (signed_input_size_ < 0)
        negative_or_to_big_input_size_from_stdin();

    if (std::cin.fail())
        failed_read_input_size_from_stdin(); // exit 1

    input_size_ = static_cast<size_t> (signed_input_size_);
}

//---------------------------------------------------------------------------------------------------------------

template <typename input_t>
void test_input_t<input_t>::read_test_data_from_stdin()
{
    data_ = new input_t[input_size_];

    if (!data_)
        failed_allocate_memory_for_input_array_from_stdin(); // exit 1

    for (size_t input_iter = 0; input_iter < input_size_; input_iter++)
    {
        std::cin >> data_[input_iter];
        
        if (!(std::cin.fail())) continue;

        failed_read_i_page_from_dat(input_iter); // exit 1
    }
}

//---------------------------------------------------------------------------------------------------------------

template <typename input_t>
__attribute__ ((noreturn))
void test_input_t<input_t>::negative_or_to_big_cache_size_from_stdin()
{
    std::cerr << "Cache size is negative or too big. We can`t parse it." << std::endl
    << "failed read cache size from stdin." << std::endl;

    exit(EXIT_FAILURE);
}

//---------------------------------------------------------------------------------------------------------------

template <typename input_t>
__attribute__ ((noreturn))
void test_input_t<input_t>::failed_read_cache_size_from_stdin()
{
    std::cerr << "Failed read cache size from stdin." << std::endl;

    exit(EXIT_FAILURE);
}

//---------------------------------------------------------------------------------------------------------------

template <typename input_t>
__attribute__ ((noreturn))
void test_input_t<input_t>::negative_or_to_big_input_size_from_stdin()
{
    std::cerr << "Input size is negative or too big. We can`t parse it." << std::endl
    << "failed read input size from stdin." << std::endl;

    exit(EXIT_FAILURE);
}

//---------------------------------------------------------------------------------------------------------------

template <typename input_t>
__attribute__ ((noreturn))
void test_input_t<input_t>::failed_read_input_size_from_stdin()
{
    std::cerr << "Failed read input size from stdin." << std::endl;

    exit(EXIT_FAILURE);
}

//---------------------------------------------------------------------------------------------------------------

template <typename input_t>
__attribute__ ((noreturn))
void test_input_t<input_t>::failed_allocate_memory_for_input_array_from_stdin()
{
    std::cerr << "Failed allocate for input from stdin." << std::endl;

    exit(EXIT_FAILURE);
}

//---------------------------------------------------------------------------------------------------------------

template <typename input_t>
__attribute__ ((noreturn))
void test_input_t<input_t>::failed_read_i_page_from_stdin(size_t number_of_bad_page)
{
    std::cerr << "failed read " << number_of_bad_page << " page from stdin."
              << std::endl;

    exit(EXIT_FAILURE);
}

//---------------------------------------------------------------------------------------------------------------

// private methods for reading from .dat file
//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------

template <typename input_t>
void test_input_t<input_t>::open_test_file()
{
    file_.open(test_file_);
    
    if (file_.fail())
        failed_open_test_file(); // exit 1
}

//---------------------------------------------------------------------------------------------------------------

template <typename input_t>
void test_input_t<input_t>::read_cache_size_from_dat()
{
    signed long long int signed_cache_size_ = 0;

    // будет ли оправдан такой статик ассерт? 
    // static_assert(sizeof(signed_cache_size_) == sizeof(cache_size_),
    // "if this sizes are not equal, we can`t report wrong about bad input"
    // "on your device you can ignore it, or made signed_cache_size equal to sizeof(size_t)"
    // );

    file_ >> signed_cache_size_;

    if (signed_cache_size_ < 0)
        negative_or_to_big_cache_size_in_dat();

    if (file_.fail())
        failed_read_cache_size_from_dat(); // exit 1

    cache_size_ = static_cast<size_t> (signed_cache_size_); // here we are sure, that signed_cache_size_
}

//---------------------------------------------------------------------------------------------------------------

template <typename input_t>
void test_input_t<input_t>::read_input_size_from_dat()
{

    signed long long int signed_input_size_ = 0;

    // будет ли оправдан такой статик ассерт? 
    // static_assert(sizeof(signed_input_size_) == sizeof(input_size_),
    // "if this sizes are not equal, we can`t report wrong about bad input"
    // "on your device you can ignore it, or made signed_cache_size_ equal to sizeof(size_t)"
    // );

    file_ >> signed_input_size_;

    if (signed_input_size_ < 0)
        negative_or_to_big_input_size_from_dat();

    if (file_.fail())
        failed_read_input_size_from_dat(); // exit 1

    input_size_ = static_cast<size_t> (signed_input_size_);
}

//---------------------------------------------------------------------------------------------------------------

template <typename input_t>
void test_input_t<input_t>::close_test_file()
{
    file_.close();
}

//---------------------------------------------------------------------------------------------------------------

template <typename input_t>
void test_input_t<input_t>::read_test_data_from_dat()
{
    if (is_test_empty()) return; // no allocation memory for empty test

    data_ = new input_t[input_size_];

    if (!data_)
        failed_allocate_memory_for_input_array_from_dat(); // exit 1

    for (size_t input_iter = 0; input_iter < input_size_; input_iter++)
    {
        file_ >> data_[input_iter];
        
        if (!file_.fail()) continue;

        failed_read_i_page_from_dat(input_iter); // exit 1
    }
}

//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------

template <typename input_t>
__attribute__ ((noreturn))
void test_input_t<input_t>::failed_open_test_file()
{
    std::cerr << "Failed open '" << test_file_ << "'"
              << std::endl;

    exit(EXIT_FAILURE);
}

//---------------------------------------------------------------------------------------------------------------

template <typename input_t>
__attribute__ ((noreturn))
void test_input_t<input_t>::negative_or_to_big_cache_size_in_dat()
{
    std::cerr << "Cache size is negative or too big. We can`t parse it." << std::endl
    << "failed read cache size from '" << test_file_ << "'" << std::endl;

    exit(EXIT_FAILURE);
}

//---------------------------------------------------------------------------------------------------------------

template <typename input_t>
__attribute__ ((noreturn))
void test_input_t<input_t>::failed_read_cache_size_from_dat()
{
    std::cerr << "Failed read cache size from '" << test_file_ << "'"
              << std::endl;

    exit(EXIT_FAILURE);
}

//---------------------------------------------------------------------------------------------------------------

template <typename input_t>
__attribute__ ((noreturn))
void test_input_t<input_t>::negative_or_to_big_input_size_from_dat()
{
    std::cerr << "Input size is negative or too big. We can`t parse it." << std::endl
    << "failed read input size from '" << test_file_ << "'" << std::endl;

    exit(EXIT_FAILURE);
}

//---------------------------------------------------------------------------------------------------------------

template <typename input_t>
__attribute__ ((noreturn))
void test_input_t<input_t>::failed_read_input_size_from_dat()
{
    std::cerr << "Failed read input size from '" << test_file_ << "'"
              << std::endl;

    exit(EXIT_FAILURE);
}

//---------------------------------------------------------------------------------------------------------------

template <typename input_t>
__attribute__ ((noreturn))
void test_input_t<input_t>::failed_allocate_memory_for_input_array_from_dat()
{
    std::cerr << "Failed allocate for input from '" << test_file_
              << "'" << std::endl;

    exit(EXIT_FAILURE);
}

//---------------------------------------------------------------------------------------------------------------

template <typename input_t>
__attribute__ ((noreturn))
void test_input_t<input_t>::failed_read_i_page_from_dat(size_t number_of_bad_page)
{
    std::cerr << "failed read " << number_of_bad_page << " page from '"
              << test_file_ << "'" << std::endl;

    exit(EXIT_FAILURE);
}

//---------------------------------------------------------------------------------------------------------------

template <typename input_t>
bool test_input_t<input_t>::is_test_empty()
{
    return (input_size_ == 0);
}

//---------------------------------------------------------------------------------------------------------------

#endif // TEST_READ_TEST_HPP