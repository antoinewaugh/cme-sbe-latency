cmake_minimum_required(VERSION 3.5)
project(cme_sbe_latency)

set(CMAKE_CXX_STANDARD 14)


set(BOOST_ROOT /usr/local/boost_1_63_0)
set(BOOST_INCLUDEDIR /usr/include)

set(BOOST_LIBRARYDIR /usr/lib/x86_64-linux-gnu)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)

find_package(Boost COMPONENTS system thread)
include_directories( ${Boost_INCLUDE_DIRS})

set(SOURCE_FILES main.cpp)
add_executable(cme_sbe_latency ${SOURCE_FILES})

include_directories(DepthBook)
add_subdirectory(DepthBook)

include_directories(Subscriber)
add_subdirectory(Subscriber)

target_link_libraries(cme_sbe_latency ${Boost_LIBRARIES} sbe Subscriber DepthBook)
