cmake_minimum_required(VERSION 3.6)
project(cme_sbe_latency)

#set(CMAKE_CXX_STANDARD 17)

# Check Apama Environment
if(DEFINED APAMA_HOME)
    message("APAMA_HOME found : $ENV{APAMA_HOME}")
    set(APAMA_INCLUDE_DIRS "${APAMA_HOME}/include")
    set(APAMA_LIBRARIES "${APAMA_HOME}/lib/libapclient.so")
else()
    message(FATAL_ERROR "Please specify APAMA_HOME using -DAPAMA_HOME=/path/to/Apama")
endif()

add_definitions(-std=c++17)

set(BOOST_ROOT /usr/local/boost_1_63_0)
set(BOOST_INCLUDEDIR /usr/include)
set(BOOST_LIBRARYDIR /usr/local/lib)
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)

find_package(Boost COMPONENTS system thread)
include_directories(${Boost_INCLUDE_DIRS})

set(SOURCE_FILES connectivity_plugin.cpp)
add_library(cme_sbe_latency ${SOURCE_FILES})

include_directories(${PROJECT_INCLUDE_DIRS})
include_directories(${APAMA_INCLUDE_DIRS})
include_directories(Config)
include_directories(MarketData)
include_directories(Subscriber)

add_subdirectory(Config)
add_subdirectory(MarketData)
add_subdirectory(Subscriber)

#####################################################################
#### GTEST
#####################################################################
# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
if(result)
    message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
if(result)
    message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_BINARY_DIR}/googletest-src
        ${CMAKE_BINARY_DIR}/googletest-build
        EXCLUDE_FROM_ALL)

# The gtest/gtest_main targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
    include_directories("${gtest_SOURCE_DIR}/include")
endif()

add_subdirectory(test)

#####################################################################
#### // GTEST
#####################################################################

target_link_libraries(cme_sbe_latency ${Boost_LIBRARIES} sbe Config Subscriber MarketData ${APAMA_LIBRARIRES})

