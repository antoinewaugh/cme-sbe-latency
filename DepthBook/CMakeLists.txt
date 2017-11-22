project(DepthBook)

add_definitions(-std=c++14)

set(HEADER_FILES DepthList.h DepthBook.h)
set(SOURCE_FILES DepthList.cpp DepthBook.cpp)

add_library(DepthBook STATIC ${SOURCE_FILES} ${HEADER_FILES})