
project(Subscriber)

add_definitions(-std=c++17)

# imported library
add_library(sbe STATIC IMPORTED)
set_property(TARGET sbe PROPERTY IMPORTED_LOCATION /usr/local/lib/libsbe.a)
set_property(TARGET sbe PROPERTY INTERFACE_INCLUDE_DIRECTORIES /usr/local/include/sbe)

set(HEADER_FILES
        MulticastReceiver.h
        Handler.h
        Message.h
        Packet.h
        ChannelAccessor.h
        InstrumentController.h
        ChannelController.h
        InstrumentMdHandler.h
        Channel.h
        )

set(SOURCE_FILES
        MulticastReceiver.cpp
        Handler.cpp
        Message.cpp
        Packet.cpp
        ChannelAccessor.cpp
        InstrumentController.cpp
        ChannelController.cpp
        InstrumentMdHandler.cpp
        Channel.cpp
        )

add_library(Subscriber STATIC ${SOURCE_FILES} ${HEADER_FILES})
target_link_libraries(Subscriber sbe)
