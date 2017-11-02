#include "Subscriber.h"

Subscriber::Subscriber(boost::asio::io_service &io_service,
                       const boost::asio::ip::address &listen_address,
                       const boost::asio::ip::address &multicast_address_ia,
                       const short multicast_port_ia,
                       const boost::asio::ip::address &multicast_address_ib,
                       const short multicast_port_ib,
                       const boost::asio::ip::address &multicast_address_sa,
                       const short multicast_port_sa,
                       const boost::asio::ip::address &multicast_address_sb,
                       const short multicast_port_sb
)
    : feed_(23936, handler_, decoder_, io_service, listen_address, multicast_address_ia, multicast_port_ia, multicast_address_ib, multicast_port_ib, multicast_address_sa, multicast_port_sa, multicast_address_sb, multicast_port_sb
){}
