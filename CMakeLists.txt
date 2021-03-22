cmake_minimum_required(VERSION 3.17)
project(n_gram)

set(CMAKE_CXX_STANDARD 14)

if (CMAKE_COMPILER_IS_GNUCC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
endif ()

set(CMAKE_BUILD_TYPE Debug)
#set(CMAKE_BUILD_TYPE Release)

find_package(Threads REQUIRED)
find_package(ICU REQUIRED COMPONENTS uc i18n)
find_package(Boost REQUIRED COMPONENTS iostreams filesystem locale system)

set(Boost_USE_STATIC_LIBS OFF) # enable dynamic linking
set(Boost_USE_MULTITHREAD ON)  # enable multithreading

INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR}, ${ICU_LIBRARIES}, ${ICU_INCLUDE_DIRS})

#file(GLOB SRC "inc/*.h" "src/*.cpp")

add_executable(n_gram main.cpp ${SRC} src/NgramModel.cpp inc/NgramModel.h src/Ngram.cpp inc/Ngram.h
        src/word_tokenizer.cpp inc/word_tokenizer.h inc/ngram_hasher.h inc/vector_hasher.h)
target_link_libraries(n_gram LINK_PUBLIC ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(n_gram LINK_PUBLIC ${Boost_LIBRARIES})
target_link_libraries(n_gram LINK_PUBLIC Threads::Threads)