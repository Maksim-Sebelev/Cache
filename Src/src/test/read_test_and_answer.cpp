#include <cassert>
#include "read_test_and_answer.hpp"
#include "test_files.hpp"

//---------------------------------------------------------------------------------------------------------------

template <typename input_t>
test_data_t<input_t>::test_data_t(const test_files_t& test_files) :
test_input_(test_files.test_file_), test_answer_(test_files.answer_file_)
{}

//---------------------------------------------------------------------------------------------------------------

template <typename input_t>
test_data_t<input_t>::~test_data_t()
{
    test_input_ .~test_input_t ();
    test_answer_.~test_answer_t();
}

//---------------------------------------------------------------------------------------------------------------

/*
    ЭТО ДИЧАЙШИЙ КРИНЖ
    НО Я ЭТО СДЕЛАЛ ДО ТОГО
    КАК УЗНАЛ ЧТО РЕЛИЗОВЫВАТЬ
    МЕТОДЫ В .cpp БЫЛО ПЛОХОЙ ИДЕЕЙ
    Я РАСКАИВАЮСЬ И
    БОЛЬШЕ НИКОГДА ТАК ДЕЛАТЬ
    НЕ БУДУ!!! ОБЕЩАЮ
    НЕ БЕЙТЕ МЕНЯ  
*/

template struct test_data_t<int>;

//---------------------------------------------------------------------------------------------------------------
