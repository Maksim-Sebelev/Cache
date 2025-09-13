cmake_minimum_required(VERSION 3.10)

project(Cache
	LANGUAGES CXX
)

# =============================================================================================================

# function to set some sanitezers and flags in debug and release versions

function(add_release_and_debug_options_to_executable executable)

	# some saitizers for debug version
	set(DEBUG_SANITIZERS
		-fsanitize=address
		-fsanitize=alignment
		-fsanitize=bool
		-fsanitize=bounds
		-fsanitize=enum
		-fsanitize=float-cast-overflow
		-fsanitize=undefined
		-fsanitize=unreachable
		-fsanitize=vla-bound
		-fsanitize=vptr
		-fsanitize=float-divide-by-zero
		-fsanitize=integer-divide-by-zero
		-fsanitize=leak
		-fsanitize=nonnull-attribute
		-fsanitize=null
		-fsanitize=object-size
		-fsanitize=return
		-fsanitize=returns-nonnull-attribute
		-fsanitize=shift
		-fsanitize=signed-integer-overflow
		# fsanitize=address,alignment,bool
		# fsanitize=bounds,enum,float-cast-overflow
		# fsanitize=undefined,unreachable,vla-bound,vptr
		# fsanitize=float-divide-by-zero,integer-divide-by-zero
		# fsanitize=leak,nonnull-attribute,null,object-size,return
		# fsanitize=returns-nonnull-attribute,shift,signed-integer-overflow
	)

	set(DEBUG_FLAGS
		-g -D _DEBUG -ggdb3 -std=c++17 -O0 -Wall
		-Wextra -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported
		-Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations
		-Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op
		-Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith
		-Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral
		-fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer -Wstack-usage=8192
		-Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel
		-Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types
		-Wsuggest-override -Wswitch-default -Wsync-nand -Wundef -Wunreachable-code -Wunused
		-Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing
		-Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector
		-pie -fPIE -Werror=vla
		# -Weffc++
	)

	target_compile_options(${executable} PRIVATE

		# some santizers and DED32 debug flags for debug version
		$<$<CONFIG:Debug>:
			${DEBUG_SANITIZERS}
			${DEBUG_FLAGS}
		>

		# some release flags
		$<$<CONFIG:Release>:
			-march=native
		>
	)

	# linking sanitizers
	target_link_options(${executable} PRIVATE
		$<$<CONFIG:Debug>:
			${DEBUG_SANITIZERS}
		>
	)

endfunction(add_release_and_debug_options_to_executable)

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

target_include_directories(${ARC_CACHE_EXE} PRIVATE
		${ARC_CACHE_INC_DIR}
)

add_release_and_debug_options_to_executable(${ARC_CACHE_EXE})

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

add_release_and_debug_options_to_executable(${OPTIMAL_CACHE_EXE})

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


# arc 
set(ARC_CACHE_TEST_SRC_DIR ${PROJECT_SOURCE_DIR}/src/test    )
set(ARC_CACHE_TEST_INC_DIR ${PROJECT_SOURCE_DIR}/include/test)
set(ARC_CACHE_TEST_EXE     arc_cache_test                    )

set(ARC_CACHE_TEST_SRC
	# cache test files
	${ARC_CACHE_TEST_SRC_DIR}/arc_cache_test.cpp
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


add_release_and_debug_options_to_executable(${ARC_CACHE_TEST_EXE})

# ==========
# optimal
set(OPTIMAL_CACHE_TEST_SRC_DIR ${PROJECT_SOURCE_DIR}/src/test    )
set(OPTIMAL_CACHE_TEST_INC_DIR ${PROJECT_SOURCE_DIR}/include/test)
set(OPTIMAL_CACHE_TEST_EXE     optimal_cache_test                )

set(OPTIMAL_CACHE_TEST_SRC
	# cache test files
	${OPTIMAL_CACHE_TEST_SRC_DIR}/optimal_cache_test.cpp
	${OPTIMAL_CACHE_TEST_SRC_DIR}/check_args.cpp

	# cache function
	${OPTIMAL_CACHE_SRC_DIR}/optimal_cache.cpp
)

add_executable(${OPTIMAL_CACHE_TEST_EXE} 
				EXCLUDE_FROM_ALL # this executable file is depending on target 'ready_for_test'
			   ${OPTIMAL_CACHE_TEST_SRC}
)

target_include_directories(${OPTIMAL_CACHE_TEST_EXE}
	PRIVATE
		${OPTIMAL_CACHE_TEST_INC_DIR}
		${OPTIMAL_CACHE_INC_DIR}
)

add_release_and_debug_options_to_executable(${OPTIMAL_CACHE_TEST_EXE})

# =============================================================================================================
# creating tests

enable_testing()
include(CTest)


# function for adding tests to some executable files
function(add_testing test_executable test_dir ans_dir)

	file(GLOB test_files "${test_dir}/*.dat") # save all tests -files in variable 'test_files'

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
		get_filename_component(test_file_name "${test_file}" NAME_WE)   # NAME_WE = без расширения
		set(answer_file "${ans_dir}/${test_file_name}.ans")

		# create test_name
		set(test_name ${test_executable}_${test_file_name})

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
	DEPENDS ${ARC_CACHE_TEST_EXE} ${OPTIMAL_CACHE_TEST_EXE}
)

# get directory with all tests

set(ARC_TEST_DIR   ${CMAKE_SOURCE_DIR}/../tests/arc/dat)
set(ARC_ANSWER_DIR ${CMAKE_SOURCE_DIR}/../tests/arc/ans)

set(OPT_TEST_DIR   ${CMAKE_SOURCE_DIR}/../tests/optimal/dat)
set(OPT_ANSWER_DIR ${CMAKE_SOURCE_DIR}/../tests/optimal/ans)

set(OPTIMAL_TEST_ANSWER_DIR ${GLOBAL_TEST_ANSWER_DIR}/optimal)

add_testing(${ARC_CACHE_TEST_EXE}      ${ARC_TEST_DIR} ${ARC_ANSWER_DIR}) # testing arc     cache
add_testing(${OPTIMAL_CACHE_TEST_EXE}  ${OPT_TEST_DIR} ${OPT_ANSWER_DIR}) # testing optimal cache

# =============================================================================================================
