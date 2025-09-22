#ifndef TEST_GET_TEST_DATA_HPP
#define TEST_GET_TEST_DATA_HPP

//---------------------------------------------------------------------------------------------------------------

#include <cstdlib>
#include <cassert>
#include <memory>

#include "read_test.hpp"
#include "read_answer.hpp"
#include "test_files.hpp"
#include "input_stream.hpp"
#include "parse_args.hpp"

#include "global.hpp"

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
        // ctor`s
        test_data_t();                               // ctor for stdin
        test_data_t(const test_files_t& test_files); // ctor for files 

        size_t  get_test_answer      ();
        size_t  get_cache_size       ();
        size_t  get_input_size       ();
        input_t get_i_element_of_data(size_t i);

        ON_DEBUG(
        void dump();
        )
};

//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------

// function to get test data
// memory allocation needed because we 
// don`t know how we become a test data:
// from stdin or from files
// !!! in README is more information about this

template <typename input_t>
std::unique_ptr<test_data_t<input_t>> get_test_data(int argc, char* argv[])
{
    flags_parser  parsing_flags_result(argc, argv);
    input_stream  type_of_input_sream = parsing_flags_result.get_input_stream();

    assert(type_of_input_sream != input_stream::invalid_input_stream);

    std::unique_ptr<test_data_t<input_t>> test_data;

    switch (type_of_input_sream)
    {
        case input_stream::standart_input:
        {
            test_data = std::make_unique<test_data_t<input_t>>();
            break;
        }

        case input_stream::dat_file_stream:
        {
            test_files_t test_files = parsing_flags_result.get_test_files();
            test_data = std::make_unique<test_data_t<input_t>>(test_files);
            break;
        }

        case input_stream::invalid_input_stream:
        default: assert(0 && "wtf?"); __builtin_unreachable();

    }

    return test_data;
}

//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------

ON_DEBUG(
template <typename input_t>
void test_data_t<input_t>::dump()
{
    std::cout << "=========== TEST DATA DUMP ===============" << std::endl << std::endl;

    std::cout << "ans        = " << get_test_answer() << std::endl;
    std::cout << "cache size = " << get_cache_size () << std::endl;
    std::cout << "input size = " << get_input_size () << std::endl;
    std::cout << std::endl << std::endl;

    for (size_t i = 0; i < get_input_size(); i++)
        std::cout << "input[" << i << "] = " << get_i_element_of_data(i) << std::endl;
    
    std::cout << std::endl << "=========== TEST DATA DUMP END ==========" << std::endl << std::endl;
}
)

//---------------------------------------------------------------------------------------------------------------

// ctor for reading from files
template <typename input_t>
test_data_t<input_t>::test_data_t(const test_files_t& test_files) :
test_input_(test_files.test_file_), test_answer_(test_files.answer_file_)
{}

//---------------------------------------------------------------------------------------------------------------

// ctor for reading from stdin
template <typename input_t>
test_data_t<input_t>::test_data_t() :
test_input_(), test_answer_()
{}

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

#endif // TEST_GET_TEST_DATA_HPP
