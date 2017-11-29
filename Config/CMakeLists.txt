project(Config)

add_definitions(-std=c++17)

set(HEADER_FILES Connection.h Config.h ConfigGenerator.h )
set(SOURCE_FILES Connection.cpp Config.cpp ConfigGenerator.cpp)

add_library(Config STATIC ${SOURCE_FILES} ${HEADER_FILES})