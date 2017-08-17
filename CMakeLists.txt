cmake_minimum_required(VERSION 3.5)
project(cme_sbe_latency)

set(CMAKE_CXX_STANDARD 11)

# imported library
add_library(sbe STATIC IMPORTED)
set_property(TARGET sbe PROPERTY IMPORTED_LOCATION /usr/local/lib/libsbe.a)
set_property(TARGET sbe PROPERTY INTERFACE_INCLUDE_DIRECTORIES /usr/local/include/sbe)

set(BOOST_ROOT /usr/local/boost_1_63_0)
set(BOOST_INCLUDEDIR /usr/include)

set(BOOST_LIBRARYDIR /usr/lib/x86_64-linux-gnu)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)

find_package(Boost COMPONENTS system thread)
include_directories( ${Boost_INCLUDE_DIRS})

set(SOURCE_FILES main.cpp)
add_executable(cme_sbe_latency ${SOURCE_FILES})
target_link_libraries(cme_sbe_latency ${Boost_LIBRARIES} sbe)
