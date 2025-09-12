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

        bool          is_test_empty                          (size_t input_size);
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

#endif // TEST_READ_TEST_HPP