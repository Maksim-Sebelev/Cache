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

    ON_DEBUG(
    test_data->dump();
    )

    test_data->get_i_element_of_data(0);


    CacheDriver<int, int> driver(test_data);

    ARCCache<int, int> arc_cache();

    driver.run_cache(arc_cache);

    return EXIT_SUCCESS;
}
