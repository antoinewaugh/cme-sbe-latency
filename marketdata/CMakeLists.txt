project(marketdata)

add_definitions(-std=c++17)
add_definitions(-fPIC)

set(HEADER_FILES
        Instrument.h
        DepthList.h
        DepthBook.h
        Trade.h
        SessionStatistics.h
        SecurityStatus.h)

set(SOURCE_FILES
        Instrument.cpp
        DepthList.cpp
        DepthBook.cpp
        Trade.cpp
        SessionStatistics.cpp
        SecurityStatus.cpp)

add_library(marketdata STATIC ${SOURCE_FILES} ${HEADER_FILES})
