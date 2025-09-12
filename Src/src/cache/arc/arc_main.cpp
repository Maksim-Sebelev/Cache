#include <iostream>
#include "arc_cache.hpp"

//---------------------------------------------------------------------------------------------------------------

int main(int argc, char* argv[])
{
    std::cout << std::endl;

    std::cout << "argc = " << argc << std::endl;

    for (int i = 0; i < argc; i++)
        std::cout << "argv[" << i << "] = '" << argv[i] << "'" << std::endl;

    std::cout << std::endl;

    return 0;
}

//---------------------------------------------------------------------------------------------------------------
