project(config)

add_definitions(-std=c++17)
add_definitions(-fPIC)


set(HEADER_FILES Connection.h config.h)
set(SOURCE_FILES Connection.cpp config.cpp)

add_library(config STATIC ${SOURCE_FILES} ${HEADER_FILES})
