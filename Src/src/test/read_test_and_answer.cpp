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

template <typename input_t>
size_t test_data_t<input_t>::get_test_answer()
{
    return test_answer_.get_test_answer();
}

//---------------------------------------------------------------------------------------------------------------

template <typename input_t>
size_t test_data_t<input_t>::get_cache_size()
{
    return test_input_.get_cache_size();
}

//---------------------------------------------------------------------------------------------------------------

template <typename input_t>
size_t test_data_t<input_t>::get_input_size()
{
    return test_input_.get_input_size();
}

//---------------------------------------------------------------------------------------------------------------

template <typename input_t>
input_t test_data_t<input_t>::get_i_element_of_data(size_t i)
{
    return test_input_.get_i_element_of_data(i);
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
