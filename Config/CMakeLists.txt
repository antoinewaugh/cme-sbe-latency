project(Config)

add_definitions(-std=c++17)
add_definitions(-fPIC)


set(HEADER_FILES Connection.h Config.h)
set(SOURCE_FILES Connection.cpp Config.cpp)

add_library(Config STATIC ${SOURCE_FILES} ${HEADER_FILES})
