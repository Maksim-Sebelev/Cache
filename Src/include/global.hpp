#ifndef GLOBAL_HPP
#define GLOBAL_HPP

//---------------------------------------------------------------------------------------------------------------

#ifdef _DEBUG
#define ON_DEBUG(...) __VA_ARGS__
#include <iostream>
#else // _DEBUG
#define ON_DEBUG(...)
#endif // _DEBUG

//---------------------------------------------------------------------------------------------------------------

#define builtin_unreachable_wrapper(debug_message) do \
{                                                      \
    assert(0 && debug_message);                         \
    __builtin_unreachable();                             \
}                                                         \
while (0)                                                  \


//---------------------------------------------------------------------------------------------------------------

#endif // GLOBAL_HPP
