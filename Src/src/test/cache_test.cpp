#include <iostream>
#include <cstdlib>
#include <cassert>
#include <cstring>
#include "arc_cache.hpp"

//---------------------------------------------------------------------------------------------------------------

int main(int argc, char* argv[])
{
        



    return 0;
}

//---------------------------------------------------------------------------------------------------------------

static void CheckArgs(int argc, char* argv[])
{
    if (argc != 2)
        bad_args_quant(argv[0]);

    const char* test_file   = argv[1];
    const char* answer_file = argv[2];   

    if (!is_file_test(test_file))
        bad_test_extension(test_file); // exit 1

    if (!is_file_answer(answer_file))
        bad_answer_extension(answer_file); // exit 1

    return;
}

//---------------------------------------------------------------------------------------------------------------

__attribute__ ((noreturn))
static void bad_args_quant(const char* exe_file)
{
    assert(exe_file);

    std::cerr << "Needed 2 argmunets - test and answer file name"
              << std::endl
              << "Usage: ./" << exe_file << " <test_file>.dat <answer_file>.ans"
              << std::endl;

    exit(EXIT_FAILURE);
}

//---------------------------------------------------------------------------------------------------------------

__attribute__ ((noreturn))
static void bad_test_extension(const char* test_file)
{
    assert(test_file);

    std::cerr << "\'" << test_file << "\'" << "is not a test file."
              << std::endl
              << "Correct is *.dat"
              << std::endl;

    exit(EXIT_FAILURE);
}

//---------------------------------------------------------------------------------------------------------------

__attribute__ ((noreturn))
static void bad_answer_extension(const char* answer_file)
{
    assert(answer_file);

    std::cerr << "\'" << answer_file << "\'" << "is not a answer file."
              << std::endl
              << "Correct is *.ans"
              << std::endl;

    exit(EXIT_FAILURE);
}

//---------------------------------------------------------------------------------------------------------------

const char* get_file_extension(const char* file_name)
{
    assert(file_name);

    const char* dot = strrchr(file_name, '.');

    if ((!dot) || (dot == file_name))
        return nullptr;

    return dot + 1;
}

//---------------------------------------------------------------------------------------------------------------

#define check_extension(file, needed_extension) \
assert(file);                                    \
const char* extension = get_file_extension(file); \
return  (extension &&                              \
        (strcmp(extension, needed_extension) == 0)  \
        );                                           \


//---------------------------------------------------------------------------------------------------------------

static bool is_file_test(const char* test_file)
{
    check_extension(test_file, ".dat");
}

//---------------------------------------------------------------------------------------------------------------

static bool is_file_answer(const char* answer_file)
{
    check_extension(answer_file, ".ans");
}

//---------------------------------------------------------------------------------------------------------------

#undef check_extension

//---------------------------------------------------------------------------------------------------------------
