#ifndef TEST_PARSE_ARGS_HPP
#define TEST_PARSE_ARGS_HPP

//---------------------------------------------------------------------------------------------------------------

#include <getopt.h>
#include <unistd.h>
#include <cstdlib>
#include <cassert>
#include <iostream>
#include <cstring>
#include <cstdlib>

#include "test_files.hpp"
#include "input_stream.hpp"

//---------------------------------------------------------------------------------------------------------------

class flags_parser
{
    public:
        // class public methods
        input_sream   get_input_stream     ();
        test_files_t  get_test_files       ();

        // ctor
        flags_parser(int argc, const char* argv[]);

    private:
        
        // class private variables
        input_sream  input_stream_;
        test_files_t test_files_  ;

        struct are_parametrs_already_defined
        {
            bool is_input_stream_defined;
            bool is_test_file_defined   ;
            bool is_answer_file_defined ;
        };

        are_parametrs_already_defined are_parametrs_already_defined_;
    
        void input_stream_defined();
        void test_file_defined   ();
        void answer_file_defined ();

        static const struct option                       long_options            [];
        static const std::pair<input_sream, std::string> input_stream_flag_values[];

        // class private methods

        void parse_flag_input_stream                         ();
        void parse_not_a_flag                                (); // maybe test file name, maybe invalid option
        void parse_flag_help                                 ();





        input_sream get_type_of_input_stream                 ();

        __attribute__ ((noreturn))
        void invalid_type_of_input_stream                    ();
        __attribute__ ((noreturn))
        void try_to_get_test_files_when_input_stream_is_stdin();

};

//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------

const struct option flags_parser::long_options[] =
{
    {"input_stream", required_argument, 0, 'i'},
    {"help"        , no_argument      , 0, 'h'},
    {nullptr       , 0                , 0,  0 }, // just for safety
};

//---------------------------------------------------------------------------------------------------------------

const std::pair<input_sream, std::string> flags_parser::input_stream_flag_values[] =
{
    {input_sream::standart_input           , "stdin"},
    {input_sream::dat_file_stream          , "file" },
    {input_sream::invalid_input_stream, ""     },
};

//---------------------------------------------------------------------------------------------------------------

// class public methods
//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------

// ctor

flags_parser::flags_parser(int argc, const char* argv[]) :
input_stream_                 (input_sream::standart_input), // default value of input_sream_
test_files_                   (nullptr, nullptr)           , // we don`t know files before parsing args
are_parametrs_already_defined_({false, false, false})        // nohing is defined before parsing args
{

}

//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------

input_sream flags_parser::get_input_stream()
{
    return input_stream_;
}

//---------------------------------------------------------------------------------------------------------------

test_files_t flags_parser::get_test_files()
{
    if (input_stream_ != input_sream::dat_file_stream)
        try_to_get_test_files_when_input_stream_is_stdin(); // exit 1
    
    return test_files_;
};

//---------------------------------------------------------------------------------------------------------------

// class private methods
//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------

void flags_parser::parse_flag_input_stream()
{
    assert(optarg);

    input_stream_ = get_type_of_input_stream();

    if (input_stream_ == input_sream::invalid_input_stream)
        invalid_type_of_input_stream(); // exit 1

    input_stream_defined();
}

//---------------------------------------------------------------------------------------------------------------

input_sream flags_parser::get_type_of_input_stream()
{
    assert(optarg);

    for (auto input_stream_type: input_stream_flag_values)
    {
        if (input_stream_type.second.compare(optarg) == 0)
            return input_stream_type.first;        
    }

    return input_sream::invalid_input_stream;
}

//---------------------------------------------------------------------------------------------------------------

void flags_parser::input_stream_defined()
{
    are_parametrs_already_defined_.is_input_stream_defined = true;
}

//---------------------------------------------------------------------------------------------------------------

void flags_parser::test_file_defined()
{
    are_parametrs_already_defined_.is_test_file_defined = true;
}

//---------------------------------------------------------------------------------------------------------------

void flags_parser::answer_file_defined()
{
    are_parametrs_already_defined_.is_answer_file_defined = true;
}

//---------------------------------------------------------------------------------------------------------------

__attribute__ ((noreturn))
void flags_parser::invalid_type_of_input_stream()
{
    assert(optarg);

    std::cerr << "undefined type of input stream '" << optarg << "'."    << std::endl
              << "Usage: '-input_stream=stdin' or '-input_stream=file'." << std::endl
              << "For more information use option '--help' or '-h',"     << std::endl;

    exit(EXIT_FAILURE);
}

//---------------------------------------------------------------------------------------------------------------

__attribute__ ((noreturn))
void flags_parser::try_to_get_test_files_when_input_stream_is_stdin()
{
    std::cerr << "Try to get test_files, but in they is nothing," << std::endl
              << "because test data was reading from stdin."      << std::endl;

    exit(EXIT_FAILURE);
}


//---------------------------------------------------------------------------------------------------------------

#endif // TEST_PARSE_ARGS_HPP
