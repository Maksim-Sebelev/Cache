#include <iostream>
#include <cstdlib>
#include <cassert>
#include <cstring>

#include "check_args.hpp"

//---------------------------------------------------------------------------------------------------------------

__attribute__ ((noreturn))
static void        bad_args_quant       (const std::string& exe_file);

__attribute__ ((noreturn))
static void        bad_test_extension   (const std::string& test_file);

__attribute__ ((noreturn))
static void        bad_answer_extension (const std::string& answer_file);

static std::string get_file_extension   (const std::string& file_name  );
static bool        is_file_test         (const std::string& test_file  );
static bool        is_file_answer       (const std::string& answer_file);

//---------------------------------------------------------------------------------------------------------------

void check_args(int argc, const char* argv[])
{
    assert(argv);

    static const int correct_argument_quantity = 3;
    // 3 = executable name + test file + answer file

    if (argc != correct_argument_quantity)
        bad_args_quant(argv[0]); // exit 1

    std::string test_file  (argv[1]);
    std::string answer_file(argv[2]);

    if (!is_file_test(test_file))
        bad_test_extension(test_file); // exit 1

    if (!is_file_answer(answer_file))
        bad_answer_extension(answer_file); // exit 1

    return;
}

//---------------------------------------------------------------------------------------------------------------

__attribute__ ((noreturn))
static void bad_args_quant(const std::string& exe_file)
{
    std::cerr << "Needed 2 argmunets - test and answer file name"
              << std::endl
              << "Usage: "
              << std::endl
              << exe_file << " <test_file>.dat <answer_file>.ans"
              << std::endl;

    exit(EXIT_FAILURE);
}

//---------------------------------------------------------------------------------------------------------------

__attribute__ ((noreturn))
static void bad_test_extension(const std::string& test_file)
{
    std::cerr << "\'" << test_file << "\'" << "is not a test file."
              << std::endl
              << "Correct is *.dat"
              << std::endl;

    exit(EXIT_FAILURE);
}

//---------------------------------------------------------------------------------------------------------------

__attribute__ ((noreturn))
static void bad_answer_extension(const std::string& answer_file)
{
    std::cerr << "\'" << answer_file << "\' is not a answer file."
              << std::endl
              << "Correct is *.ans"
              << std::endl;

    exit(EXIT_FAILURE);
}

//---------------------------------------------------------------------------------------------------------------

// return empty string if file didn`t have an extension
std::string get_file_extension(const std::string& filename)
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

static bool is_file_test(const std::string& test_file)
{
    check_extension(test_file, ".dat");
}

//---------------------------------------------------------------------------------------------------------------

static bool is_file_answer(const std::string& answer_file)
{
    check_extension(answer_file, ".ans");
}

//---------------------------------------------------------------------------------------------------------------

#undef check_extension

//---------------------------------------------------------------------------------------------------------------
