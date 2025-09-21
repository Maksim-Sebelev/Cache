
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

        // ctor
        test_answer_t();                        // ctor for reading from stdin
        test_answer_t(std::string answer_file); // reading from .dat file
    
    private:
        // struct private vairable
        size_t        test_answer_ ;
        std::ifstream file_        ;
        std::string   answer_file_ ;

        // struct private methods for reading from .ans file

        void open_answer_file           ();
        void read_test_answer_from_ans  ();
        void close_answer_file          ();
    
        __attribute__ ((noreturn))
        void failed_open_answer_file    ();

        __attribute__ ((noreturn))
        void failed_read_answer_from_ans();
};

//---------------------------------------------------------------------------------------------------------------


// ctor
//---------------------------------------------------------------------------------------------------------------
// ctor for reading from stdin
// made nothing, but needed for 
// initialization test_data_t

// made nothing, because when input from stdin
// you check test result yourself.
// in that case answer is in stdout

test_answer_t::test_answer_t() :
test_answer_(0), file_(""), answer_file_("")
{}

//---------------------------------------------------------------------------------------------------------------

// ctor for reading from .ans file
test_answer_t::test_answer_t(std::string answer_file) :
answer_file_(answer_file)
{
    open_answer_file         ();
    read_test_answer_from_ans();
    close_answer_file        ();
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
    file_.open(answer_file_);
    
    if (!file_.is_open())
        failed_open_answer_file();
}

//---------------------------------------------------------------------------------------------------------------

void test_answer_t::read_test_answer_from_ans()
{
    /*
    здесь нет проверки на отрицательсность значения
    (тип test_answer_ есть size_t),
    так как если у вас некоректные ответы
    в тестах, то что вы ожидаете???
    */

    file_ >> test_answer_;

    if (file_.fail())
        failed_read_answer_from_ans();
}

//---------------------------------------------------------------------------------------------------------------

void test_answer_t::close_answer_file()
{
    file_.close();
}

//---------------------------------------------------------------------------------------------------------------

__attribute__ ((noreturn))
void test_answer_t::failed_open_answer_file()
{
    std::cerr << "Failed open '" << answer_file_ << "'"
              << std::endl;

    exit(EXIT_FAILURE);
}

//---------------------------------------------------------------------------------------------------------------

__attribute__ ((noreturn))
void test_answer_t::failed_read_answer_from_ans()
{
    std::cerr << "Failed read answer from '" << answer_file_
              << "'" << std::endl;

    exit(EXIT_FAILURE);
}

//---------------------------------------------------------------------------------------------------------------


#endif // TEST_READ_ANSWER_HPP
