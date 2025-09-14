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
		-Wsuggest-override -Wsync-nand -Wundef -Wunreachable-code -Wunused
		-Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing
		-Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector
		-pie -fPIE -Werror=vla
		# -Weffc++
		# -Wswitch-default
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

set(ARC_CACHE_INC_DIR     ${PROJECT_SOURCE_DIR}/include/cache/arc    )
set(OPTIMAL_CACHE_INC_DIR ${PROJECT_SOURCE_DIR}/include/cache/optimal)

# =============================================================================================================
# creating test for arc_cache
# name of executable file - 'arc_cache_test'
# needed for testing of our cache
# for test didn`t need the main executable
# we are compiling the cache function
# for testing separately and with some another settings


# arc 
set(ARC_CACHE_TEST_SRC_DIR ${PROJECT_SOURCE_DIR}/src/test)
set(ARC_CACHE_TEST_EXE     arc_cache_test                )

set(ARC_CACHE_TEST_SRC
	# cache test files
	${ARC_CACHE_TEST_SRC_DIR}/arc_cache_test.cpp
)

add_executable(${ARC_CACHE_TEST_EXE} 
			   ${ARC_CACHE_TEST_SRC}
)

target_include_directories(${ARC_CACHE_TEST_EXE}
	PRIVATE
		${ARC_CACHE_INC_DIR}
)


add_release_and_debug_options_to_executable(${ARC_CACHE_TEST_EXE})

# =============================================================================================================
# optimal
set(OPTIMAL_CACHE_TEST_SRC_DIR ${PROJECT_SOURCE_DIR}/src/test)
set(OPTIMAL_CACHE_TEST_EXE     optimal_cache_test            )

set(OPTIMAL_CACHE_TEST_SRC
	# cache test files
	${OPTIMAL_CACHE_TEST_SRC_DIR}/optimal_cache_test.cpp
)

add_executable(${OPTIMAL_CACHE_TEST_EXE} 
			   ${OPTIMAL_CACHE_TEST_SRC}
)

target_include_directories(${OPTIMAL_CACHE_TEST_EXE}
	PRIVATE
		${OPTIMAL_CACHE_INC_DIR}
)

add_release_and_debug_options_to_executable(${OPTIMAL_CACHE_TEST_EXE})

# =============================================================================================================