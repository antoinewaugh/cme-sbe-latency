
project(Subscriber)

add_definitions(-std=c++17)

# imported library
add_library(sbe STATIC IMPORTED)
set_property(TARGET sbe PROPERTY IMPORTED_LOCATION /usr/local/lib/libsbe.a)
set_property(TARGET sbe PROPERTY INTERFACE_INCLUDE_DIRECTORIES /usr/local/include/sbe)

set(HEADER_FILES
        DataSource.h
        Decoder.h
        MulticastReceiver.h
        Subscriber.h
        Handler.h
        Message.h
        Packet.h
        Channel.h
        ChannelAccessor.h
        ChannelInstrumentBookHandler.h
        InstrumentController.h
        ChannelInstrumentSynchroniser.h
        InstrumentMdHandler.h
        ChannelDepthBookManager.h
        )

set(SOURCE_FILES
        DataSource.cpp
        Decoder.cpp
        MulticastReceiver.cpp
        Subscriber.cpp
        Handler.cpp
        Message.cpp
        Packet.cpp
        Channel.cpp
        ChannelAccessor.cpp
        ChannelInstrumentBookHandler.cpp
        InstrumentController.cpp
        ChannelInstrumentSynchroniser.cpp
        InstrumentMdHandler.cpp
        ChannelDepthBookManager.cpp
        )

add_library(Subscriber STATIC ${SOURCE_FILES} ${HEADER_FILES})
target_link_libraries(Subscriber sbe)
