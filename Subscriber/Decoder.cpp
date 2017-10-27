#include "Decoder.h"

// Decoder::Decoder() {
//  boost::asio::io_service io_service;
//  receiver r(io_service, boost::asio::ip::address::from_string(argv[1]),
//             boost::asio::ip::address::from_string(argv[2]), atoi(argv[3]));
//  io_service.run();
//}

size_t Decoder::decode_snapshot(SnapshotFullRefresh38 &refresh, char *buffer,
                                uint64_t offset, uint64_t block_length,
                                uint64_t version, uint64_t bufferLength) {

  refresh.wrapForDecode(buffer, offset, block_length, version, bufferLength);

  auto &entry = refresh.noMDEntries();

  while (entry.hasNext()) {
    entry.next();

    // validate securityID
    // validate seqnum

    int level = entry.mDPriceLevel();
    float price =
        entry.mDEntryPx().mantissa() * 10 ^ entry.mDEntryPx().exponent();
    int volume = entry.mDEntrySize();

    if (entry.mDEntryType() == MDEntryType::Bid) {
      book_.add_bid(level, price, volume);
    } else if (entry.mDEntryType() == MDEntryType::Offer) {
      book_.add_ask(level, price, volume);
    }
  }
}

size_t Decoder::decode_incremental_refresh_book(
    MDIncrementalRefreshBook32 &refresh, char *buffer, uint64_t offset,
    uint64_t block_length, uint64_t version, uint64_t bufferLength) {
  refresh.wrapForDecode(buffer, offset, block_length, version, bufferLength);

  auto &entry = refresh.noMDEntries();

  while (entry.hasNext()) {
    entry.next();

    // validate securityID
    // validate seqnum

    int level = entry.mDPriceLevel();
    float price =
        entry.mDEntryPx().mantissa() * 10 ^ entry.mDEntryPx().exponent();
    int volume = entry.mDEntrySize();

    if (entry.mDEntryType() == MDEntryTypeBook::Bid) {
      if (entry.mDUpdateAction() == MDUpdateAction::New) {
        book_.add_bid(level, price, volume);
      } else if (entry.mDUpdateAction() == MDUpdateAction::Change) {
        book_.update_bid(level, price, volume);
      } else if (entry.mDUpdateAction() == MDUpdateAction::Delete) {
        book_.delete_bid(level, price);
      } else if (entry.mDUpdateAction() == MDUpdateAction::DeleteFrom) {
        book_.delete_bid_from(level);
      } else if (entry.mDUpdateAction() == MDUpdateAction::DeleteThru) {
        book_.delete_bid_thru(level);
      }
    } else if (entry.mDEntryType() == MDEntryTypeBook::Offer) {
      if (entry.mDUpdateAction() == MDUpdateAction::New) {
        book_.add_ask(level, price, volume);
      } else if (entry.mDUpdateAction() == MDUpdateAction::Change) {
        book_.update_ask(level, price, volume);
      } else if (entry.mDUpdateAction() == MDUpdateAction::Delete) {
        book_.delete_ask(level, price);
      } else if (entry.mDUpdateAction() == MDUpdateAction::DeleteFrom) {
        book_.delete_ask_from(level);
      } else if (entry.mDUpdateAction() == MDUpdateAction::DeleteThru) {
        book_.delete_ask_thru(level);
      }
    }
  }
}

size_t Decoder::decode_header(MessageHeader &header, char *buffer,
                              std::uint64_t offset,
                              std::uint64_t buffer_length) {
  header.wrap(buffer, offset, kMsgHeaderVersion, buffer_length);
  return header.encodedLength();
}

size_t Decoder::decode_message(uint64_t offset) {
  int16_t message_length =
      SBE_LITTLE_ENDIAN_ENCODE_16(*((std::uint16_t *)(data_ + offset)));

  size_t header_length =
      decode_header(header_, data_, offset + kMsgSize, sizeof(data_));

  uint64_t msg_offset = offset + kMsgSize + header_length;

  if (header_.templateId() == MDIncrementalRefreshBook32::sbeTemplateId()) {
    decode_incremental_refresh_book(incremental_refresh_book_, data_,
                                    msg_offset, header_.blockLength(),
                                    header_.version(), sizeof(data_));
  } else if (header_.templateId() == SnapshotFullRefresh38::sbeTemplateId()) {
    decode_snapshot(snapshot_full_, data_, msg_offset, header_.blockLength(),
                    header_.version(), sizeof(data_));
  }
  //  sbe::MDIncrementalRefreshBook32::sbeTemplateId()) {
  //    std::size_t d =
  //        decodeMDIncremental(incr_, data_, offset + kMsgSize +
  //        header_length,
  //                            header_.blockLength(), header_.version(),
  //                            sizeof(data_));
  //
  //  } else if (header_.templateId() ==
  //             sbe::MDIncrementalRefreshVolume37::sbeTemplateId()) {
  //    std::size_t d =
  //    decodeMDIncremental<sbe::MDIncrementalRefreshVolume37>(
  //        incrVol_, data_, offset + kMsgSize + header_length,
  //        header_.blockLength(),
  //        header_.version(), sizeof(data_));
  //  } else if (header_.templateId() ==
  //             sbe::MDIncrementalRefreshTradeSummary42::sbeTemplateId()) {
  //    std::size_t d =
  //        decodeMDIncremental<sbe::MDIncrementalRefreshTradeSummary42>(
  //            incrTrade_, data_, offset + kMsgSize + header_length,
  //            header_.blockLength(), header_.version(), sizeof(data_));
  //  } else if (header_.templateId() ==
  //             sbe::MDIncrementalRefreshOrderBook43::sbeTemplateId()) {
  //    std::size_t d =
  //        decodeMDIncrementalNoEntries<sbe::MDIncrementalRefreshOrderBook43>(
  //            incrOB_, data_, offset + kMsgSize + header_length,
  //            header_.blockLength(), header_.version(), sizeof(data_));
  //  } else {
  //    std::cout << "Unknown message type, templateId: " <<
  //    header_.templateId()
  //              << std::endl;
  //    break;
  //  }
  //
  return message_length;
}

size_t Decoder::decode_packet(size_t received_) {
  size_t processed_ = kByteOffest;
  while (processed_ < received_) {
    processed_ += decode_message(processed_);
  }
}

void Decoder::handle_packet(const boost::system::error_code &error,
                            size_t received_) {
  if (error)
    return;

  decode_packet(received_);

  socket_.async_receive_from(
      boost::asio::buffer(data_, max_length), sender_endpoint_,
      boost::bind(&Decoder::handle_packet, this,
                  boost::asio::placeholders::error,
                  boost::asio::placeholders::bytes_transferred));
}
