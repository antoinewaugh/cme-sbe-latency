#include "Decoder.h"
#include <iostream>

size_t Decoder::decode_incremental_refresh_volume(
    MDIncrementalRefreshVolume37 &refresh, Decoder::Message message) {}

size_t Decoder::decode_incremental_refresh_trade(
    MDIncrementalRefreshTradeSummary42 &refresh, Decoder::Message message) {}

size_t Decoder::decode_incremental_refresh_order_book(
    MDIncrementalRefreshOrderBook43 &refresh, Decoder::Message message) {}

size_t Decoder::decode_snapshot(SnapshotFullRefresh38 &refresh,
                                Decoder::Message message) {

  refresh.wrapForDecode(message.buffer, message.offset, message.block_length,
                        message.version, message.buffer_length);

  // symbolfeed.OnMDIncrementalRefreshBook32(refresh);
}

// void Decoder::handle_ask_entry(MDUpdateAction::Value action, int level,
//                               float price, int volume) {
//  switch (action) {
//  case MDUpdateAction::New:
//    book_.add_ask(level, price, volume);
//    break;
//  case MDUpdateAction::Change:
//    book_.update_ask(level, price, volume);
//    break;
//  case MDUpdateAction::Delete:
//    book_.delete_ask(level, price);
//    break;
//  case MDUpdateAction::DeleteFrom:
//    book_.delete_ask_from(level);
//    break;
//  case MDUpdateAction::DeleteThru:
//    book_.delete_ask_thru(level);
//    break;
//  }
//}
//
// void Decoder::handle_bid_entry(MDUpdateAction::Value action, int level,
//                               float price, int volume) {
//  switch (action) {
//  case MDUpdateAction::New:
//    book_.add_bid(level, price, volume);
//    break;
//  case MDUpdateAction::Change:
//    book_.update_bid(level, price, volume);
//    break;
//  case MDUpdateAction::Delete:
//    book_.delete_bid(level, price);
//    break;
//  case MDUpdateAction::DeleteFrom:
//    book_.delete_bid_from(level);
//    break;
//  case MDUpdateAction::DeleteThru:
//    book_.delete_bid_thru(level);
//    break;
//  }
//}

size_t
Decoder::decode_incremental_refresh_book(MDIncrementalRefreshBook32 &refresh,
                                         Decoder::Message message) {

  refresh.wrapForDecode(message.buffer, message.offset, message.block_length,
                        message.version, message.buffer_length);

  auto &entry = refresh.noMDEntries();

  while (entry.hasNext()) {
    entry.next();

    // validate securityID
    // validate seqnum

    int level = entry.mDPriceLevel();
    float price = entry.mDEntryPx().mantissa() *
                  std::pow(10, entry.mDEntryPx().exponent());
    int volume = entry.mDEntrySize();

    if (entry.mDEntryType() == MDEntryTypeBook::Bid) {
      //      handle_bid_entry(entry.mDUpdateAction(), level, price, volume);
    } else if (entry.mDEntryType() == MDEntryTypeBook::Offer) {
      //      handle_ask_entry(entry.mDUpdateAction(), level, price, volume);
    }
  }
}

size_t Decoder::decode_message_length(char *buffer, size_t offset) {
  return SBE_LITTLE_ENDIAN_ENCODE_16(*((std::uint16_t *)(buffer + offset)));
}

size_t Decoder::decode_header(MessageHeader &header, char *buffer,
                              std::uint64_t offset,
                              std::uint64_t buffer_length) {

  header.wrap(buffer, offset, kMsgHeaderVersion, buffer_length);
  return header.encodedLength();
}

size_t Decoder::decode_message(char *buffer, uint64_t offset) {

  size_t message_length = decode_message_length(buffer, offset);
  size_t header_length =
      decode_header(header_, buffer, offset + kMsgSize, 4096);

  uint64_t msg_offset = offset + kMsgSize + header_length;

  switch (header_.templateId()) {

  case MDIncrementalRefreshBook32::sbeTemplateId():
    decode_incremental_refresh_book(incremental_refresh_book_,
                                    Message{buffer, msg_offset,
                                            header_.blockLength(),
                                            header_.version(), 4096});
    break;

  case SnapshotFullRefresh38::sbeTemplateId():
    decode_snapshot(snapshot_full_,
                    Message{buffer, msg_offset, header_.blockLength(),
                            header_.version(), 4096});
    break;

  case MDIncrementalRefreshVolume37::sbeTemplateId():
    decode_incremental_refresh_volume(
        incremental_volume_, Message{buffer, msg_offset, header_.blockLength(),
                                     header_.version(), 4096});
    break;

  case MDIncrementalRefreshTradeSummary42::sbeTemplateId():
    decode_incremental_refresh_trade(
        incremental_trade_, Message{buffer, msg_offset, header_.blockLength(),
                                    header_.version(), 4096});
    break;

  case MDIncrementalRefreshOrderBook43::sbeTemplateId():
    decode_incremental_refresh_order_book(incremental_order_book_,
                                          Message{buffer, msg_offset,
                                                  header_.blockLength(),
                                                  header_.version(), 4096});
    break;
  default:
    std::cout << "Unknown message type, templateId: " << header_.templateId()
              << '\n';
    break;
  }
  return message_length;
}

size_t Decoder::decode_packet(char *buffer, size_t received) {
  size_t processed = kByteOffest;
  while (processed < received) {
    processed += decode_message(buffer, processed);
  }
}

Decoder::Decoder(SymbolFeed &feed) : feed_(feed) {}
