#include <cassert>
#include "test/read_test_and_answer.hpp"
#include "test/test_files.hpp"

//---------------------------------------------------------------------------------------------------------------

template <typename input_t>
test_data_t<input_t>::test_data_t(const test_files_t& test_files) :
test_input_(test_files.test_file_), test_answer_(test_files.answer_file_)
{
    assert(test_file);
    assert(answer_file);
}

//---------------------------------------------------------------------------------------------------------------
