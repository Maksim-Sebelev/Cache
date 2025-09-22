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

#endif // GLOBAL_HPP
