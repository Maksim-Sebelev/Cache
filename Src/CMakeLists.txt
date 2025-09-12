cmake_minimum_required(VERSION 3.5)

project(Cache
	LANGUAGES CXX
)

# =============================================================================================================
	
# creating main executable file of this projects
# name - 'arc_cache'
# for reak work with our cache

set(ARC_CACHE_SRC_DIR ${PROJECT_SOURCE_DIR}/src/cache/arc    )
set(ARC_CACHE_INC_DIR ${PROJECT_SOURCE_DIR}/include/cache/arc)
set(ARC_CACHE_EXE     arc_cache                              )

set(ARC_CACHE_SRC
	${ARC_CACHE_SRC_DIR}/arc_cache.cpp
	${ARC_CACHE_SRC_DIR}/arc_main.cpp
)

add_executable(${ARC_CACHE_EXE} ${ARC_CACHE_SRC})

target_include_directories(${ARC_CACHE_EXE}
	PRIVATE
		${ARC_CACHE_INC_DIR}
)

# =============================================================================================================

# creating executable file of optimal cache (belady)
# name - 'optimal_cache'

set(OPTIMAL_CACHE_SRC_DIR ${PROJECT_SOURCE_DIR}/src/cache/optimal    )
set(OPTIMAL_CACHE_INC_DIR ${PROJECT_SOURCE_DIR}/include/cache/optimal)
set(OPTIMAL_CACHE_EXE     optimal_cache								 )

set(OPTIMAL_CACHE_SRC
	${OPTIMAL_CACHE_SRC_DIR}/optimal_cache.cpp
	${OPTIMAL_CACHE_SRC_DIR}/optimal_main.cpp
)

add_executable(${OPTIMAL_CACHE_EXE}
				EXCLUDE_FROM_ALL	
			   ${OPTIMAL_CACHE_SRC}
)

target_include_directories(${OPTIMAL_CACHE_EXE}
	PRIVATE
		${OPTIMAL_CACHE_INC_DIR}
)

# =============================================================================================================
# add targets
# 'run', 'rebuild'
# 'run_optimlal', 'rebuild_optimal'

set(EXE_TARGET     ${ARC_CACHE_EXE}) # useless, just for same code style
set(RUN_TARGET     run         )
set(CLEAN_TARGET   clean       ) # defaulted generate. this needed only for same code style
set(REBUILD_TARGET rebuild     )

add_custom_target(${RUN_TARGET}
    COMMAND $<TARGET_FILE:${EXE_TARGET}>
	DEPENDS ${EXE_TARGET}
	COMMENT "Running executable: $<TARGET_FILE:${ARC_CACHE_EXE}>"
)

add_custom_target(${REBUILD_TARGET}
	COMMAND make clean
    COMMAND make
	COMMENT "Rebuilding project"
)

# =============================================================================================================

# creating test for arc_cache executable
# name of executable file - 'cache_test'
# needed for testing of our cache
# for test didn`t need the main executable
# we are compiling the cache function
# for testing separately and with some another settings

set(ARC_CACHE_TEST_SRC_DIR ${PROJECT_SOURCE_DIR}/src/test    )
set(ARC_CACHE_TEST_INC_DIR ${PROJECT_SOURCE_DIR}/include/test)
set(ARC_CACHE_TEST_EXE     cache_test  )

set(ARC_CACHE_TEST_SRC
	# cache test files
	${ARC_CACHE_TEST_SRC_DIR}/cache_test.cpp
	${ARC_CACHE_TEST_SRC_DIR}/check_args.cpp

	# cache function
	${ARC_CACHE_SRC_DIR}/arc_cache.cpp
)

add_executable(${ARC_CACHE_TEST_EXE} 
				EXCLUDE_FROM_ALL # this executable file is depending on target 'ready_for_test'
			   ${ARC_CACHE_TEST_SRC}
)

target_include_directories(${ARC_CACHE_TEST_EXE}
	PRIVATE
		${ARC_CACHE_TEST_INC_DIR}
		${ARC_CACHE_INC_DIR}
)

# =============================================================================================================
# creating tests

enable_testing()
include(CTest)



# function for adding tests to some executable files
function(add_testing test_executable test_dir)

	if(NOT EXISTS ${test_dir})
		message(WARNING "Test directory not found: ${test_dir}")
		return()
	endif()

	file(GLOB test_files "${test_dir}/dat/*.dat") # save all tests -files in variable 'test_files'

	# get quantity of tests
	list(LENGTH test_files num_tests)

	# check that at least 1 test was found
	if(${num_tests} EQUAL 0)
		message(WARNING "No .dat files found in ${test_dir}")
		return()
	endif()

	# add tests
	foreach(test_file ${test_files})

		# get name of answer file
		get_filename_component(test_file_name      "${test_file}" NAME_WE)   # NAME_WE = без расширения
		get_filename_component(test_file_directory "${test_file}" DIRECTORY) # DIRECTTORY = только путь, без файла
		set(answer_file "${test_file_directory}/../ans/${test_file_name}.ans")

		# create test_name
		
		get_filename_component(test_num "${test_file}" NAME_WE)
		set(test_name ${test_executable}_${test_num})

		message(${test_name})

		# add test
		add_test(
			NAME ${test_name}
			# execute test
			COMMAND ./${test_executable} ${test_file} ${answer_file}

			# working directory - dircetory with executable file
			WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
		)

	endforeach()

endfunction(add_testing)

# run this target, to compile executable for testing
add_custom_target(ready_for_tests
	DEPENDS ${ARC_CACHE_EXE} ${OPTIMAL_CACHE_EXE}
)

# get directory with all tests
set(GLOBAL_TEST_DIR  ${CMAKE_SOURCE_DIR}/../tests)

set(ARC_TEST_DIR     ${GLOBAL_TEST_DIR}/arc    )
set(OPTIMAL_TEST_DIR ${GLOBAL_TEST_DIR}/optimal)

add_testing(${ARC_CACHE_EXE}     ${ARC_TEST_DIR}    ) # testing arc     cache
add_testing(${OPTIMAL_CACHE_EXE} ${OPTIMAL_TEST_DIR}) # testing optimal cache
