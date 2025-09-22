#include <vector>
#include <cstdlib>
#include <memory>

#include "arc_cache.hpp"
#include "utils/driver/driver.hpp"
#include "get_test_data.hpp"

#include "global.hpp"

using request_t = int;


int main(int argc, char* argv[])
{

    // std::unique_ptr<test_data_t<request_t>> test_data = get_test_data<request_t>(argc, argv);
    std::unique_ptr<test_data_t<request_t>> test_data = get_test_data<request_t>(argc, argv);
    bool print_result_and_dont_check_answer = test_data->print_result_and_dont_check_answer();

    ON_DEBUG(
    test_data->dump(); // in _DEBUG we want to know, is our test data correct
    )
    
    CacheDriver<int, int> driver(test_data);
    
    const size_t cache_size = test_data->get_cache_size();
    ARCCache<int, int> arc_cache(cache_size);

    const size_t hits_count = driver.run_cache(arc_cache);

    if (print_result_and_dont_check_answer)
    {
        std::cout << "cache hits for this test = " << hits_count << std::endl;
        return EXIT_SUCCESS;
    }

    const size_t correct_hits_count = test_data->get_test_answer();

    if (hits_count != correct_hits_count)
    {
        std::cout << "TEST FAILED!" << std::endl
                  << "your cache hits quant = " << hits_count         << std::endl
                  << "correct    hits quant = " << correct_hits_count << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
