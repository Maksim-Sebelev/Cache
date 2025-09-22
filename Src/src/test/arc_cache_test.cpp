#include <vector>
#include <cstdlib>

#include "arc_cache.hpp"
#include "get_test_data.hpp"
#include "parse_test_result.hpp"

#include "global.hpp"


using key_t  = int;
using item_t = int;


int main(int argc, char* argv[])
{
    test_data_t<item_t>*    test_data         = get_test_data            <item_t>(argc, argv                  ); ON_DEBUG(test_data->dump();)
    size_t                  cache_capacity    = test_data->get_cache_size        (                            );
    ARCCache<key_t, item_t> arc_cache                                            (cache_capacity              );
    std::vector<item_t>     requets           = test_data->get_requests          (                            );
    size_t                  cache_test_result = arc_cache.run_cache              (requets                     );
    TestResult              test_result_type  = parse_test_result                (cache_test_result, test_data);

    delete test_data;

    if (test_result_type == TestResult::TEST_FAILED)
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}
