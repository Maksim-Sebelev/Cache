#ifndef TEST_TEST_FILES_HPP
#define TEST_TEST_FILES_HPP

//---------------------------------------------------------------------------------------------------------------

struct test_files_t
{
    const char* test_file_  ;
    const char* answer_file_;

    test_files_t(const char* test_file, const char* answer_file);
};

//---------------------------------------------------------------------------------------------------------------

test_files_t::test_files_t(const char* test_file, const char* answer_file) :
test_file_(test_file), answer_file_(answer_file)
{}

//---------------------------------------------------------------------------------------------------------------

#endif // TEST_TEST_FILES_HPP
