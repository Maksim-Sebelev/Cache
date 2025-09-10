cmake_minimum_required(VERSION 3.5)

project(some_c++_project
	LANGUAGES CXX)

set(SRC_DIR src)
set(INC_DIR include)
set(EXE cache)

set(SRC
	${SRC_DIR}/cache_test.cpp
	${SRC_DIR}/arc_cache.cpp
)

add_executable(${EXE} ${SRC})

target_include_directories(${EXE}
	PRIVATE
		${PROJECT_SOURCE_DIR}/${INC_DIR}
)

# add targets 'run' and 'rebuild' to Makefile

set(EXE_TARGET     ${EXE})
set(RUN_TARGET     run   )
set(CLEAN_TARGET   clean  ) # defaulted generate. this needed only for same code style
set(REBUILD_TARGET rebuild)

add_custom_target(${RUN_TARGET}
    COMMAND $<TARGET_FILE:${EXE_TARGET}>
	DEPENDS ${EXE_TARGET}
	COMMENT "Running executable: $<TARGET_FILE:${EXE}>"
)

add_custom_target(${REBUILD_TARGET}
	COMMAND ${CMAKE_COMMAND} --build . --target ${CLEAN_TARGET}
    COMMAND ${CMAKE_COMMAND} --build . --target ${EXE_TARGET}
	COMMENT "Rebuilding project"
)

# create tests

enable_testing()
include(CTest)

SET(TEST_DIR ${CMAKE_SOURCE_DIR}/../tests)

file(GLOB tests_files "${TEST_DIR}/*.dat") # save all tests-files in variable 'testfiles'

if(NOT EXISTS ${TEST_DIR}) # check that exists test-directory
    message(WARNING "Test directory not found: ${TEST_DIR}")
    return()
endif()

list(LENGTH tests_files num_tests) # get quantity of tests

if(${num_tests} EQUAL 0) # check that at least 1 test was found
	message(WARNING "No .dat files found in ${TEST_DIR}")
	return()
endif()

if(NOT EXISTS ${TEST_DIR}/testrun.sh)
    message(WARNING "testrun.sh not found in ${TEST_DIR}")
    return()
endif()

foreach(file ${tests_files})
	get_filename_component(test_name ${file} NAME_WE)

	add_test(
		NAME test_${test_name}
		COMMAND bash -c ${TEST_DIR}/testrun2.sh ${file}
		# WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
	)

	set_tests_properties(test_${test_name} PROPERTIES DEPENDS ${EXE})

endforeach()
