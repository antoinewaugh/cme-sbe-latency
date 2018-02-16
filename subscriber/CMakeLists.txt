
project(subscriber)

add_definitions(-std=c++17)
add_definitions(-fPIC)

# imported library
add_library(sbe STATIC IMPORTED)
set_property(TARGET sbe PROPERTY IMPORTED_LOCATION /usr/local/lib/libsbe.a)
set_property(TARGET sbe PROPERTY INTERFACE_INCLUDE_DIRECTORIES /usr/local/include/sbe)

set(HEADER_FILES
        MulticastReceiver.h
        MarketDataListener.h
        Message.h
        Packet.h
        ChannelContext.h
        InstrumentController.h
        ChannelController.h
        SymbolController.h
        InstrumentMdHandler.h
        Channel.h
        ChannelImpl.h
        )

set(SOURCE_FILES
        MulticastReceiver.cpp
        MarketDataListener.cpp
        Message.cpp
        Packet.cpp
        ChannelContext.cpp
        InstrumentController.cpp
        SymbolController.cpp
        ChannelController.cpp
        InstrumentMdHandler.cpp
        Channel.cpp
        ChannelImpl.cpp)

add_library(subscriber STATIC ${SOURCE_FILES} ${HEADER_FILES})
target_link_libraries(subscriber sbe)
