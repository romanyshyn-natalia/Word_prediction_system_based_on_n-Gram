cmake_minimum_required(VERSION 3.17)
project(n_gram)

set(CMAKE_CXX_STANDARD 14)

if (CMAKE_COMPILER_IS_GNUCC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
endif ()

set(CMAKE_BUILD_TYPE Debug)
#set(CMAKE_BUILD_TYPE Release)

find_package(Boost COMPONENTS filesystem REQUIRED)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})

file(GLOB SRC "include/*.hpp" "src/*.cpp")

add_executable(n_gram main.cpp ${SRC} src/NgramModel.cpp inc/NgramModel.h)
TARGET_LINK_LIBRARIES(n_gram LINK_PUBLIC ${Boost_LIBRARIES})