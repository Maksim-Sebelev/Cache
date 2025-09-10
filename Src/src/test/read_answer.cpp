#include <iostream>
#include <cstdlib>
#include <fstream>
#include <cassert>

#include "test/read_answer.hpp"

//---------------------------------------------------------------------------------------------------------------

namespace
{
__attribute__ ((noreturn))
static void failed_open_answer_file(const char* answer_file);

__attribute__ ((noreturn))
static void failed_read_answer     (const char* answer_file);

} // namespace

//---------------------------------------------------------------------------------------------------------------

test_answer_t::test_answer_t (const char* answer_file)
{
    assert(answer_file);

    std::ifstream file(answer_file);
    
    if (!file.is_open())
        failed_open_answer_file(answer_file);
    
    file >> test_answer_;

    if (file.fail())
        failed_read_answer(answer_file);

    file.close();
}

//---------------------------------------------------------------------------------------------------------------

namespace // for only constructor needed funtions
{
//---------------------------------------------------------------------------------------------------------------

__attribute__ ((noreturn))
static void failed_open_answer_file(const char* answer_file)
{
    assert(answer_file);

    std::cerr << "Failed open '" << answer_file << "'"
              << std::endl;

    exit(EXIT_FAILURE);
}

//---------------------------------------------------------------------------------------------------------------

__attribute__ ((noreturn))
static void failed_read_answer(const char* answer_file)
{
    assert(answer_file);

    std::cerr << "Failed read answer from '" << answer_file
              << "'" << std::endl;

    exit(EXIT_FAILURE);
}

//---------------------------------------------------------------------------------------------------------------

} // namespace
