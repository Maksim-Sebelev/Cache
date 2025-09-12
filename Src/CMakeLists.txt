cmake_minimum_required(VERSION 3.5)

project(Cache
	LANGUAGES CXX
)

# =============================================================================================================
	
# creating main executable file of this projects
# name - 'cache'
# for reak work with our cache

set(CACHE_SRC_DIR ${PROJECT_SOURCE_DIR}/src/cache    )
set(CACHE_INC_DIR ${PROJECT_SOURCE_DIR}/include/cache)
set(CACHE_EXE     cache)

set(CACHE_SRC
	${CACHE_SRC_DIR}/arc_cache.cpp
	${CACHE_SRC_DIR}/main.cpp
)

add_executable(${CACHE_EXE} ${CACHE_SRC})

target_include_directories(${CACHE_EXE}
	PRIVATE
		${CACHE_INC_DIR}
)


# =============================================================================================================
# add targets 'run' and 'rebuild' to Makefile

set(EXE_TARGET     ${CACHE_EXE}) # useless, just for same code style
set(RUN_TARGET     run         )
set(CLEAN_TARGET   clean       ) # defaulted generate. this needed only for same code style
set(REBUILD_TARGET rebuild     )

add_custom_target(${RUN_TARGET}
    COMMAND $<TARGET_FILE:${EXE_TARGET}>
	DEPENDS ${EXE_TARGET}
	COMMENT "Running executable: $<TARGET_FILE:${CACHE_EXE}>"
)

add_custom_target(${REBUILD_TARGET}
	COMMAND ${CMAKE_COMMAND} --build . --target ${CLEAN_TARGET}
    COMMAND ${CMAKE_COMMAND} --build . --target ${EXE_TARGET}
	COMMENT "Rebuilding project"
)

# =============================================================================================================

# creating test cache executable
# name of executable file - 'cache_test'
# needed for testing of our cache
# for test didn`t need the main executable
# we are compiling the cache function
# for testing separately and with some another settings

set(CACHE_TEST_SRC_DIR ${PROJECT_SOURCE_DIR}/src/test    )
set(CACHE_TEST_INC_DIR ${PROJECT_SOURCE_DIR}/include/test)
set(CACHE_TEST_EXE     cache_test  )

set(CACHE_TEST_SRC
	# cache test files
	${CACHE_TEST_SRC_DIR}/cache_test.cpp
	${CACHE_TEST_SRC_DIR}/check_args.cpp
	${CACHE_TEST_SRC_DIR}/read_test.cpp
	${CACHE_TEST_SRC_DIR}/read_answer.cpp
	${CACHE_TEST_SRC_DIR}/read_test_and_answer.cpp
	${CACHE_TEST_SRC_DIR}/test_files.cpp

	# cache function
	${CACHE_SRC_DIR}/arc_cache.cpp
)

add_executable(${CACHE_TEST_EXE} 
				EXCLUDE_FROM_ALL # this executable file is depending on target 'test'
			   ${CACHE_TEST_SRC}
)

target_include_directories(${CACHE_TEST_EXE}
	PRIVATE
		${CACHE_TEST_INC_DIR}
		${CACHE_INC_DIR}
)

# =============================================================================================================
# creating tests

add_custom_target(ready_for_tests
	DEPENDS ${CACHE_TEST_EXE}
)

enable_testing()
include(CTest)

SET(TEST_DIR ${CMAKE_SOURCE_DIR}/../tests)

# check that exists test-directory
if(NOT EXISTS ${TEST_DIR})
    message(WARNING "Test directory not found: ${TEST_DIR}")
    return()
endif()

file(GLOB test_files   "${TEST_DIR}/*.dat") # save all tests -files in variable 'test_files'

# get quantity of tests
list(LENGTH test_files num_tests)

# check that at least 1 test was found
if(${num_tests} EQUAL 0)
	message(WARNING "No .dat files found in ${TEST_DIR}")
	return()
endif()


# add tests
foreach(test_file ${test_files})

	# get name of answer file
	get_filename_component(test_file_name      "${test_file}" NAME_WE)   # NAME_WE = без расширения
	get_filename_component(test_file_directory "${test_file}" DIRECTORY) # DIRECTTORY = только путь, без файла
	set(answer_file "${test_file_directory}/${test_file_name}.ans")

	# create test_name
	get_filename_component(test_name "test_${test_file}" NAME_WE)

	message(${test_file})
	message(${answer_file})

	# add test
	add_test(
		NAME ${test_name}
		# execute test
		COMMAND ./${CACHE_TEST_EXE} ${test_file} ${answer_file}

		# working directory - dircetory with executable file
		WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
	)

	# test will not execute, if didn`t exists executable file
	set_tests_properties(${test_name} PROPERTIES DEPENDS ${CACHE_TEST_EXE})
	

endforeach()
