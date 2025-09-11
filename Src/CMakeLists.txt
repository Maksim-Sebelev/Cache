cmake_minimum_required(VERSION 3.5)

project(Cache
	LANGUAGES CXX
)

# =============================================================================================================
	
set(CACHE_SRC_DIR src/cache    )
set(CACHE_INC_DIR include/cache)
set(CACHE_EXE     cache        )

set(CACHE_SRC
	${CACHE_SRC_DIR}/arc_cache.cpp
	${CACHE_SRC_DIR}/main.cpp
)

add_executable(${CACHE_EXE} ${CACHE_SRC})

target_include_directories(${CACHE_EXE}
	PRIVATE
		${PROJECT_SOURCE_DIR}/${CACHE_INC_DIR}
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

# create test cache executable

set(CACHE_TEST_SRC_DIR src/test    )
set(CACHE_TEST_INC_DIR include/test)
set(CACHE_TEST_EXE     cache_test  )

set(TEST_SRC
	${CACHE_TEST_SCR_DIR}/cache_test.cpp
	${CACHE_TEST_SCR_DIR}/check_args.cpp
	${CACHE_TEST_SCR_DIR}/read_test.cpp
	${CACHE_TEST_SCR_DIR}/read_answer.cpp
	${CACHE_TEST_SCR_DIR}/read_test_ans_answer.cpp
	${CACHE_TEST_SCR_DIR}/test_files.cpp
)

add_executable(${CACHE_TEST_EXE}
				EXCLUDE_FROM_ALL
				
)

# create tests

enable_testing()
include(CTest)

SET(TEST_DIR ${CMAKE_SOURCE_DIR}/../tests)

file(GLOB tests_files "${TEST_DIR}/*.dat") # save all tests-files in variable 'testfiles'

# check that exists test-directory
if(NOT EXISTS ${TEST_DIR})
    message(WARNING "Test directory not found: ${TEST_DIR}")
    return()
endif()

# get quantity of tests
list(LENGTH tests_files num_tests)

# check that at least 1 test was found
if(${num_tests} EQUAL 0)
	message(WARNING "No .dat files found in ${TEST_DIR}")
	return()
endif()

# check that script for running tests exists
if(NOT EXISTS ${TEST_DIR}/testrun.sh)
    message(WARNING "testrun.sh not found in ${TEST_DIR}")
    return()
endif()

# add tests
foreach(file ${tests_files})
	# create test_name
	get_filename_component(test_name test_${file} NAME_WE)

	# add test
	add_test(
		NAME ${test_name}
		# execute test command (-c need if .sh is not executable)
		COMMAND bash -c ${TEST_DIR}/testrun2.sh ${file}

		# working directory - dircetory with executable file
		WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
	)
	# test will not execute, if didn`t exists executable file
	set_tests_properties(${test_name} PROPERTIES DEPENDS ${CACHE_EXE})

endforeach()
