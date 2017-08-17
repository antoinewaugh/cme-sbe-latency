//
// receiver.cpp
// ~~~~~~~~~~~~
//
// Copyright (c) 2003-2008 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include "MDIncrementalRefreshBook32.h"
#include "MDIncrementalRefreshOrderBook43.h"
#include "MDIncrementalRefreshTradeSummary42.h"
#include "MDIncrementalRefreshVolume37.h"
#include "MessageHeader.h"
#include "boost/bind.hpp"
#include <boost/asio.hpp>
#include <chrono>
#include <fstream>
#include <ios>
#include <iostream>
#include <string>

// 14 bytes for:
// MsgSeqNum = 4
// https://www.cmegroup.com/confluence/display/EPICSANDBOX/MDP+3.0+-+Binary+Packet+Header
// SendingTime  = 8
// https://www.cmegroup.com/confluence/display/EPICSANDBOX/MDP+3.0+-+Binary+Packet+Header
// MsgSize=2
// https://www.cmegroup.com/confluence/display/EPICSANDBOX/MDP+3.0+-+Message+Header

bool hasWrittenToFile = false;

constexpr int BYTE_OFFSET = 12;

class receiver {
public:
    receiver(boost::asio::io_service& io_service,
        const boost::asio::ip::address& listen_address,
        const boost::asio::ip::address& multicast_address,
        const short multicast_port)
        : socket_(io_service)
    {

        out = std::ofstream("sample.dat", std::ios::out | std::ios::binary);

        boost::asio::ip::udp::endpoint listen_endpoint(boost::asio::ip::udp::v4(),
            multicast_port);

        socket_.open(listen_endpoint.protocol());
        socket_.set_option(boost::asio::ip::udp::socket::reuse_address(true));
        socket_.set_option(boost::asio::ip::multicast::enable_loopback(true));

        socket_.bind(listen_endpoint);
        socket_.set_option(
            boost::asio::ip::multicast::join_group(multicast_address.to_v4()));

        socket_.async_receive_from(
            boost::asio::buffer(data_, max_length), sender_endpoint_,
            boost::bind(&receiver::handle_receive_from, this,
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred));
    }

    template <typename T>
    std::size_t decodeMDIncremental(T& incr, char* buffer, std::uint64_t offset,
        std::uint64_t actingBlockLength,
        std::uint64_t actingVersion,
        std::uint64_t bufferLength)
    {

        incr.wrapForDecode(buffer, offset, actingBlockLength, actingVersion,
            bufferLength);

        unsigned long ns_timestamp = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::system_clock::now().time_since_epoch())
                                         .count();

        auto& matchEventIndicator = incr.matchEventIndicator();

        if (matchEventIndicator.endOfEvent()) {
            auto &entry = incr.noMDEntries();
            while (entry.hasNext()) {
                entry.next();
                std::cout << "SecurityId" << entry.securityID()
                          << ", MDEntryType: " << entry.mDEntryType() // needs to be converted to CME type
                          << ", RptSeq: " << entry.rptSeq()
                          << ", Server TS: " << ns_timestamp
                          << ", CME TS: " << incr.transactTime() << '\n';
            }
        }
        return incr.encodedLength();
    }

    template <typename T>
    std::size_t decodeMDIncrementalNoEntries(T& incr, char* buffer,
        std::uint64_t offset,
        std::uint64_t actingBlockLength,
        std::uint64_t actingVersion,
        std::uint64_t bufferLength)
    {

        incr.wrapForDecode(buffer, offset, actingBlockLength, actingVersion,
            bufferLength);

        return incr.encodedLength();
    }

    void handle_receive_from(const boost::system::error_code& error,
        size_t bytes_recvd)
    {
        if (!error) {
            std::size_t bytesProcessed = BYTE_OFFSET;

            int MSG_SIZE = 2;

            while (bytesProcessed < bytes_recvd) {

                // read next two bytes for message length
                int16_t totalMessageLength = SBE_LITTLE_ENDIAN_ENCODE_16(
                    *((std::uint16_t*)(data_ + bytesProcessed)));

                std::size_t decodeMessageLength = decodeHdr(hdr_, data_, bytesProcessed + MSG_SIZE, sizeof(data_));

                if (hdr_.templateId() == sbe::MDIncrementalRefreshBook32::sbeTemplateId()) {

                    std::size_t d = decodeMDIncremental(
                        incr_, data_, bytesProcessed + MSG_SIZE + decodeMessageLength,
                        hdr_.blockLength(), hdr_.version(), sizeof(data_));

                } else if (hdr_.templateId() == sbe::MDIncrementalRefreshVolume37::sbeTemplateId()) {

                    std::size_t d = decodeMDIncremental<sbe::MDIncrementalRefreshVolume37>(
                        incrVol_, data_,
                        bytesProcessed + MSG_SIZE + decodeMessageLength,
                        hdr_.blockLength(), hdr_.version(), sizeof(data_));
                } else if (hdr_.templateId() == sbe::MDIncrementalRefreshTradeSummary42::sbeTemplateId()) {

                    std::size_t d = decodeMDIncremental<sbe::MDIncrementalRefreshTradeSummary42>(
                        incrTrade_, data_,
                        bytesProcessed + MSG_SIZE + decodeMessageLength,
                        hdr_.blockLength(), hdr_.version(), sizeof(data_));
                } else if (hdr_.templateId() == sbe::MDIncrementalRefreshOrderBook43::sbeTemplateId()) {

                    std::size_t d = decodeMDIncrementalNoEntries<sbe::MDIncrementalRefreshOrderBook43>(
                        incrOB_, data_, bytesProcessed + MSG_SIZE + decodeMessageLength,
                        hdr_.blockLength(), hdr_.version(), sizeof(data_));
                } else {
                    std::cout << "Unknown message type, templateId: " << hdr_.templateId()
                              << std::endl;
                    break;
                }
                // as encodeLength() uses position() which changes depending on what has been accessed,
                // totalMessageLength is much more reliable for pushing offset forwards
                bytesProcessed += totalMessageLength;
            }

            socket_.async_receive_from(
                boost::asio::buffer(data_, max_length), sender_endpoint_,
                boost::bind(&receiver::handle_receive_from, this,
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
        }
    }

private:
    boost::asio::ip::udp::socket socket_;
    boost::asio::ip::udp::endpoint sender_endpoint_;
    enum { max_length = 4096 };
    char data_[max_length];
    const int messageHeaderVersion_ = 0;
    MessageHeader hdr_;
    MDIncrementalRefreshBook32 incr_;
    MDIncrementalRefreshVolume37 incrVol_;
    MDIncrementalRefreshTradeSummary42 incrTrade_;
    MDIncrementalRefreshOrderBook43 incrOB_;

    std::ofstream out;

    std::size_t decodeHdr(MessageHeader& hdr, char* buffer, std::uint64_t offset,
        std::uint64_t bufferLength)
    {
        hdr.wrap(buffer, offset, messageHeaderVersion_, bufferLength);
        return hdr.encodedLength();
    }
};

int main(int argc, char* argv[])
{
    try {
        if (argc != 4) {
            std::cerr << "Usage: receiver <listen_address> <multicast_address> "
                         "<multicast_port>\n";
            std::cerr << "  For IPv4, try:\n";
            std::cerr << "    receiver 0.0.0.0 224.0.32.1 15310\n";
            return 1;
        }

        boost::asio::io_service io_service;
        receiver r(io_service, boost::asio::ip::address::from_string(argv[1]),
            boost::asio::ip::address::from_string(argv[2]), atoi(argv[3]));
        io_service.run();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }


    return 0;
}
