
#ifndef TEST_READ_ANSWER_HPP
#define TEST_READ_ANSWER_HPP

//---------------------------------------------------------------------------------------------------------------

#include <cstdlib>
#include <fstream>

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

#endif // TEST_READ_ANSWER_HPP
