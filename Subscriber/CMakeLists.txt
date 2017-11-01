
project(Subscriber)

add_definitions(-std=c++11)

# imported library
add_library(sbe STATIC IMPORTED)
set_property(TARGET sbe PROPERTY IMPORTED_LOCATION /usr/local/lib/libsbe.a)
set_property(TARGET sbe PROPERTY INTERFACE_INCLUDE_DIRECTORIES /usr/local/include/sbe)

set(HEADER_FILES Receiver.h Decoder.h SymbolFeed.h Subscriber.h Handler.h)
set(SOURCE_FILES Decoder.cpp Receiver.cpp SymbolFeed.cpp Subscriber.cpp Handler.cpp )

add_library(Subscriber STATIC ${SOURCE_FILES} ${HEADER_FILES})
target_link_libraries(Subscriber sbe)
