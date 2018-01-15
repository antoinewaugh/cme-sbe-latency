project(DepthBook)

add_definitions(-std=c++17)

set(HEADER_FILES
        DepthList.h
        DepthBook.h
        Trade.h
        SessionStatistics.h
        SecurityStatus.h)

set(SOURCE_FILES
        DepthList.cpp
        DepthBook.cpp
        Trade.cpp
        SessionStatistics.cpp
        SecurityStatus.cpp)

add_library(DepthBook STATIC ${SOURCE_FILES} ${HEADER_FILES})
