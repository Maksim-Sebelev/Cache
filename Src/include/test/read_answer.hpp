
#ifndef TEST_READ_ANSWER_HPP
#define TEST_READ_ANSWER_HPP

//---------------------------------------------------------------------------------------------------------------

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <cassert>

//---------------------------------------------------------------------------------------------------------------

struct test_answer_t
{
    public:
        // struct public methods    
        size_t get_test_answer();
        test_answer_t         (const char* answer_file); // ctor
    
    private:
        // struct private vairable
        size_t        test_answer_ ;
        std::ifstream file_        ;
        const char*   answer_file_ ;
        
        // struct private methods
        void open_answer_file       ();
        __attribute__ ((noreturn))
        void failed_open_answer_file();
        __attribute__ ((noreturn))
        void failed_read_answer     ();
};

//---------------------------------------------------------------------------------------------------------------


// ctor
//---------------------------------------------------------------------------------------------------------------

test_answer_t::test_answer_t(const char* answer_file)
{
    assert(answer_file_);

    answer_file_ = answer_file;

    std::ifstream file(answer_file_);
    
    if (!file.is_open())
        failed_open_answer_file();
    
    file >> test_answer_;

    if (file.fail())
        failed_read_answer();

    file.close();
}

//---------------------------------------------------------------------------------------------------------------

// public methodes
//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------

size_t test_answer_t::get_test_answer()
{
    return test_answer_;
}

//---------------------------------------------------------------------------------------------------------------

// private methods
//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------

void test_answer_t::open_answer_file()
{
    assert(answer_file_);
 
    file_.open(answer_file_);
    
    if (!file_.is_open())
        failed_open_answer_file();
}


//---------------------------------------------------------------------------------------------------------------

__attribute__ ((noreturn))
void test_answer_t::failed_open_answer_file()
{
    assert(answer_file_);

    std::cerr << "Failed open '" << answer_file_ << "'"
              << std::endl;

    exit(EXIT_FAILURE);
}

//---------------------------------------------------------------------------------------------------------------

__attribute__ ((noreturn))
void test_answer_t::failed_read_answer()
{
    assert(answer_file_);

    std::cerr << "Failed read answer from '" << answer_file_
              << "'" << std::endl;

    exit(EXIT_FAILURE);
}

//---------------------------------------------------------------------------------------------------------------


#endif // TEST_READ_ANSWER_HPP
