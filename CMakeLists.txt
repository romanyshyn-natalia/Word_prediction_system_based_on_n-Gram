cmake_minimum_required(VERSION 3.17)
project(n_gram)

set(CMAKE_CXX_STANDARD 17)

option(WARNINGS_AS_ERRORS   "Treat compiler warnings as errors."    OFF)
option(ENABLE_CONAN         "Use Conan as a package manager."       OFF)
option(ENABLE_PVS_STUDIO    "Use PVS-Studio static code analyzer."  OFF) # Option for the local usage only. PVS-Studio isn't installed on GitHub machines.
option(ENABLE_SANITIZERS    "Use sanitizers to detect errors."      ON) # Option for the test builds. Do not use it in the production builds.

SET(TARGET_NAME n_gram)

if (CMAKE_COMPILER_IS_GNUCC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
endif ()

# Build release version if not specified otherwise.
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif ()

find_package(Threads REQUIRED)
find_package(ICU REQUIRED COMPONENTS uc i18n)
find_package(Boost REQUIRED COMPONENTS iostreams filesystem locale system)

find_package(OpenMP REQUIRED)

# Looks like it is not necessary:
# set(Boost_USE_STATIC_LIBS OFF) # enable dynamic linking
# set(Boost_USE_MULTITHREAD ON)  # enable multithreading

INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR} ${ICU_INCLUDE_DIRS})

#file(GLOB SRC "inc/*.h" "src/*.cpp")

add_executable(${TARGET_NAME} main.cpp src/NgramModel.cpp inc/NgramModel.h src/Ngram.cpp inc/Ngram.h
        src/word_tokenizer.cpp inc/word_tokenizer.h inc/ngram_hasher.h inc/vector_hasher.h inc/time.h)

target_link_libraries(n_gram LINK_PUBLIC ${Boost_LIBRARIES} ${ICU_LIBRARIES} )
target_link_libraries(n_gram LINK_PUBLIC Threads::Threads)
target_link_libraries(n_gram PRIVATE OpenMP::OpenMP_CXX)

include(cmake/Config.cmake)