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
        flags_parser(int argc, char* argv[]);

    private:
        
        // class private variables
        input_sream  input_stream_;
        test_files_t test_files_  ;

        struct are_parametrs_already_defined
        {
            bool is_define_input_stream;
            bool is_define_test_file   ;
            bool is_define_answer_file ;
        };

        are_parametrs_already_defined are_parametrs_already_defined_;
    
        void define_input_stream();
        void define_test_file   ();
        void define_answer_file ();

        static const struct option                       long_options            [];
        static const std::pair<input_sream, std::string> input_stream_flag_values[];

        // class private methods

        void parse_flag_input_stream                         ();
        void parse_flag_help                                 ();
        void parse_not_a_flag                                (const char* argument); // maybe test file name, maybe invalid option


        input_sream get_type_of_input_stream                 ();

        std::string get_file_extension                       (const std::string& filename);
        bool        is_string_test_file_name  (const std::string& test_file);
        bool        is_string_answer_file_name(const std::string& test_file);

        __attribute__ ((noreturn))
        void         invalid_type_of_input_stream                    ();
        __attribute__ ((noreturn))
        void         try_to_get_test_files_when_input_stream_is_stdin();
        __attribute__ ((noreturn))
        void         redefine_input_stream                           ();
        __attribute__ ((noreturn))
        void         redefine_test_file                              ();
        __attribute__ ((noreturn))
        void         redefine_answer_file                            ();
        __attribute__ ((noreturn))
        void         undefined_option                                ();
        __attribute__ ((noreturn))
        void         define_test_file_before_input_stream            ();
        __attribute__ ((noreturn))
        void         define_answer_file_before_input_stream          ();
};

//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------

const struct option flags_parser::long_options[] =
{
    {"input_stream", required_argument, 0, 'i'},
    {"help"        , no_argument      , 0, 'h'},
    {""            , 0                , 0,  0 }, // just for safety
};

//---------------------------------------------------------------------------------------------------------------

const std::pair<input_sream, std::string> flags_parser::input_stream_flag_values[] =
{
    {input_sream::standart_input      , "stdin"},
    {input_sream::dat_file_stream     , "files"},
    {input_sream::invalid_input_stream, ""      },
};

//---------------------------------------------------------------------------------------------------------------

// class public methods
//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------

// ctor

flags_parser::flags_parser(int argc, char* argv[]) :
input_stream_                 (input_sream::standart_input), // default value of input_sream_
test_files_                   ("", "")                     , // we don`t know files before parsing args
are_parametrs_already_defined_({false, false, false})        // nohing is defined before parsing args
{
    
    printf("i = %d\nh = %d\n\n", 'i', 'h');
    // int option_index = 0;

    int options_iterator = 1; // int because argc, = 1 because argc >= 1

    for (int options_iterator = 1; options_iterator < argc; options_iterator++)
    {
        int opt = getopt_long(argc, argv, "i:h", long_options, nullptr);
    
        switch (opt)
        {
            case 'i': parse_flag_input_stream();                       break;
            case 'h': parse_flag_help        ();                       break;
            case -1 : parse_not_a_flag       (argv[options_iterator]); break;
        }
    }
        // std::cout << "opt after = " << opt << std::endl;    
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
    input_stream_ = get_type_of_input_stream();

    if (input_stream_ == input_sream::invalid_input_stream)
        invalid_type_of_input_stream(); // exit 1

    define_input_stream();
}

//---------------------------------------------------------------------------------------------------------------

void flags_parser::parse_not_a_flag(const char* argument)
{
    assert(argument);

    if (is_string_test_file_name(argument))
    {
        if (!are_parametrs_already_defined_.is_define_input_stream)
            define_test_file_before_input_stream(); // exit 1

        test_files_.test_file_ = argument;
        define_test_file();
        return;
    }

    if (is_string_answer_file_name(argument))
    {
        if (!are_parametrs_already_defined_.is_define_input_stream)
            define_answer_file_before_input_stream(); // exit 1

        test_files_.answer_file_ = argument;
        define_answer_file();
        return;
    }

    undefined_option(); // exit 1
}

//---------------------------------------------------------------------------------------------------------------

void flags_parser::parse_flag_help()
{
    std::cout <<
    "There are all flags and parametrs:\n"
    "-h, --help\n"
    "-i=<some_value>, --input_stream=<some_value>\n"
    "*.dat, *.ans\n"
    "\n"
    "What does it mean?\n"
    "-h, --help\n"
    "these flags show information about all flags and their functions.\n"
    "\n"
    "-i=<some_value>, --input_stream=<some_value>\n"
    "these options change format of test.\n"
    "<some_value> has onlu 2 values:\n"
    "'stdin' and 'file'\n"
    "if youre using\n"
    "--input_stream=stdin\n"
    "program will read test data from stdin\n"
    "if youre using\n"
    "--input_stream=file\n"
    "program will read test data from file\n"
    "what is that file?\n"
    "any arg, that look like '*.dat' will be input stream for a programm\n"
    "also '*.ans' will be input stream for answer at the test\n"
    "\n"
    "ATTENTION:\n"
    "You can`t choose a .dat and .ans file if your input stream is file\n"
    "Default value of input sream is stdin\n"
    "(you can don`t indicate explicitly stdin), but if you need files - use -i=file\n"
    "You can`t choose more than 1 .dat and 1 .ans file\n"
    "You can`t choose input stream more 1 time\n"
    "\n"
    "some examples:\n"
    "correct:\n"
    "./program --input_stream=file test.dat test.ans\n"
    "\n"
    "INCORRECT (no files needed for stdin):\n"
    "./program --input_stream=stdin test.dat test.ans\n"
    "\n"
    "correct (no options => stdin):\n"
    "./program\n"
    "\n"
    "INCORRECT (no -i=file => program wait stdin, not files):\n"
    "./program test.dat test.ans\n"
    "\n"
    "\n"
    "So, that was all, what I know about flags in this program.\n"
    "Good luck, I love you :)\n";

    exit(EXIT_SUCCESS); // good exit :)
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

void flags_parser::define_input_stream()
{
    if (are_parametrs_already_defined_.is_define_input_stream)
        redefine_input_stream(); // exit 1

    are_parametrs_already_defined_.is_define_input_stream = true;
}

//---------------------------------------------------------------------------------------------------------------

void flags_parser::define_test_file()
{
    if (are_parametrs_already_defined_.is_define_test_file)
        redefine_test_file();

    are_parametrs_already_defined_.is_define_test_file = true;
}

//---------------------------------------------------------------------------------------------------------------

void flags_parser::define_answer_file()
{
    if (are_parametrs_already_defined_.is_define_answer_file)
        redefine_answer_file();

    are_parametrs_already_defined_.is_define_answer_file = true;
}

//---------------------------------------------------------------------------------------------------------------

__attribute__ ((noreturn))
void flags_parser::invalid_type_of_input_stream()
{
    assert(optarg);

    std::cerr << "undefined type of input stream '" << optarg << "'."     << std::endl
              << "Usage: '-input_stream=stdin' or '-input_stream=files'." << std::endl
              << "For more information use option '--help' or '-h'."      << std::endl;

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

__attribute__ ((noreturn))
void flags_parser::redefine_input_stream()
{
    std::cerr << "redefinition of input stream."     << std::endl
             << "I don`t know, what i must to do :(" << std::endl;

    exit(EXIT_FAILURE);
}

//---------------------------------------------------------------------------------------------------------------

__attribute__ ((noreturn))
void flags_parser::redefine_test_file()
{
    std::cerr << "redefinition of test file (.dat)."  << std::endl
              << "I don`t know, what i need to do :(" << std::endl;

    exit(EXIT_FAILURE);
}

//---------------------------------------------------------------------------------------------------------------

__attribute__ ((noreturn))
void flags_parser::redefine_answer_file()
{
    std::cerr << "redefinition of answer file (.ans)." << std::endl
              << "I don`t know, what i need to do :("  << std::endl;

    exit(EXIT_FAILURE);
}

//---------------------------------------------------------------------------------------------------------------

__attribute__ ((noreturn))
void flags_parser::undefined_option()
{
    std::cerr << "Undefined option '" << optarg << "'" << std::endl
              << "I don`t know, what i need to do :("  << std::endl;

    exit(EXIT_FAILURE);
}

//---------------------------------------------------------------------------------------------------------------

__attribute__ ((noreturn))
void flags_parser::define_test_file_before_input_stream()
{
    std::cerr << "Test file was defined before input stream." << std::endl
              << "logical error."                             << std::endl;

    exit(EXIT_FAILURE);
}

//---------------------------------------------------------------------------------------------------------------

__attribute__ ((noreturn))
void flags_parser::define_answer_file_before_input_stream()
{
    std::cerr << "Answer file was defined before input stream." << std::endl
              << "logical error."                               << std::endl;

    exit(EXIT_FAILURE);
}

//---------------------------------------------------------------------------------------------------------------

// return empty string if file didn`t have an extension
std::string flags_parser::get_file_extension(const std::string& filename)
{
    const size_t dot_pos = filename.rfind('.');

    if ((dot_pos != std::string::npos) && // dot is found
        (dot_pos + 1 < filename.size()))  // dot isn`t last symbol
    
        return filename.substr(dot_pos); // return ".govno"

    return "";
}

//---------------------------------------------------------------------------------------------------------------

#define check_extension(file, needed_extension)  \
std::string extension = get_file_extension(file); \
return  (                                          \
        (extension != "") &&                        \
        (extension == needed_extension)              \
        );                                            \


//---------------------------------------------------------------------------------------------------------------

bool flags_parser::is_string_test_file_name(const std::string& test_file)
{
    check_extension(test_file, ".dat");
}

//---------------------------------------------------------------------------------------------------------------

bool flags_parser::is_string_answer_file_name(const std::string& answer_file)
{
    check_extension(answer_file, ".ans");
}

//---------------------------------------------------------------------------------------------------------------

#undef check_extension

//---------------------------------------------------------------------------------------------------------------

#endif // TEST_PARSE_ARGS_HPP
