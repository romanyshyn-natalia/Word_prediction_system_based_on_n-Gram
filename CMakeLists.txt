cmake_minimum_required(VERSION 3.17)
project(n_gram)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_package(Qt5 COMPONENTS Widgets REQUIRED)

option(WARNINGS_AS_ERRORS   "Treat compiler warnings as errors."    OFF)
option(ENABLE_CONAN         "Use Conan as a package manager."       OFF)
option(ENABLE_PVS_STUDIO    "Use PVS-Studio static code analyzer."  OFF) # Option for the local usage only. PVS-Studio isn't installed on GitHub machines.
option(ENABLE_SANITIZERS    "Use sanitizers to detect errors."      OFF) # Option for the test builds. Do not use it in the production builds.

set(TARGET_NAME n_gram)

if (CMAKE_COMPILER_IS_GNUCC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
endif ()

# Build release version if not specified otherwise.
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif ()

find_package(ICU REQUIRED COMPONENTS uc i18n)
find_package(Boost REQUIRED COMPONENTS iostreams filesystem locale system)
find_package(LibArchive REQUIRED)

# Looks like it is not necessary:
# set(Boost_USE_STATIC_LIBS OFF) # enable dynamic linking
# set(Boost_USE_MULTITHREAD ON)  # enable multithreading

include_directories(inc)
include_directories(${LIBARCHIVE_INCLUDE_DIR})
include_directories(${Boost_INCLUDE_DIR} ${ICU_INCLUDE_DIRS})

qt5_add_resources(PROJECT_RESOURCE resources/img.qrc resources/style.qrc)


add_executable(${TARGET_NAME} ${PROJECT_RESOURCE} main.cpp ${SRC} src/main_window.cpp src/word_tokenizer.cpp
        inc/ngram_model.h inc/main_window.h inc/ngram.h inc/ngram_hasher.h inc/vector_hasher.h inc/word_tokenizer.h src/main_window.ui
        inc/files_runnable.h src/files_runnable.cpp inc/my_archive.h src/my_archive.cpp
        src/dialog.cpp src/dialog.ui inc/dialog.h)

target_link_libraries(n_gram LINK_PUBLIC ${Boost_LIBRARIES} ${ICU_LIBRARIES} )
target_link_libraries(n_gram LINK_PUBLIC ${LibArchive_LIBRARIES})
target_link_libraries(n_gram LINK_PUBLIC Qt5::Widgets)
