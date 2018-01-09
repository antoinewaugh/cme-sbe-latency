cmake_minimum_required(VERSION 3.6)
project(cme_sbe_latency)

#set(CMAKE_CXX_STANDARD 17)

add_definitions(-std=c++17)
set(BOOST_ROOT /usr/local/boost_1_63_0)
set(BOOST_INCLUDEDIR /usr/include)

set(BOOST_LIBRARYDIR /usr/local/boost_1_63_0/lib)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)

find_package(Boost COMPONENTS system thread)
include_directories(${Boost_INCLUDE_DIRS})

set(SOURCE_FILES main.cpp)
add_executable(cme_sbe_latency ${SOURCE_FILES})

include_directories(Config)
add_subdirectory(Config)

include_directories(DepthBook)
add_subdirectory(DepthBook)

include_directories(Subscriber)
add_subdirectory(Subscriber)

target_link_libraries(cme_sbe_latency ${Boost_LIBRARIES} sbe Config Subscriber DepthBook)
