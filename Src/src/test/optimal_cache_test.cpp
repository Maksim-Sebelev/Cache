#include <cstdlib>

#include "optimal_cache.hpp"
#include "get_test_data.hpp"
#include "parse_test_result.hpp"

#include "global.hpp"

using item_t = int;
using key_t  = int;

int main(int argc, char* argv[])
{
    test_data_t<item_t>*     test_data         = get_test_data            <item_t>(argc, argv                  ); ON_DEBUG(test_data->dump();)
    size_t                   cache_capacity    = test_data->get_cache_size        (                            );
    OPT_cache<key_t, item_t> opt_cache                                            (cache_capacity              );
    std::vector<item_t>      requests          = test_data->get_requests          (                            );

    // читай README чтобы понять, что тут происходит
    std::vector<std::pair<key_t, item_t>> requets_correct_format(test_data->get_input_size());

    for (size_t i = 0; i < test_data->get_input_size(); i++)
        requets_correct_format[i] = {requests[i], requests[i]};
    // дальше все нормально

    size_t                   cache_test_result = opt_cache.run_cache              (requets_correct_format      );
    TestResult               test_result_type  = parse_test_result                (cache_test_result, test_data);

    delete test_data;

    if (test_result_type == TestResult::TEST_FAILED)
        return EXIT_FAILURE;


    return EXIT_SUCCESS;
 }
