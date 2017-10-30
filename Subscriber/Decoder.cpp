#include "Decoder.h"

size_t Decoder::decode_snapshot(SnapshotFullRefresh38 &refresh, char *buffer,
                                uint64_t offset, uint64_t block_length,
                                uint64_t version, uint64_t buffer_length) {

  refresh.wrapForDecode(buffer, offset, block_length, version, buffer_length);

  auto &entry = refresh.noMDEntries();

  while (entry.hasNext()) {
    entry.next();

    // validate securityID
    // validate seqnum

    int level = entry.mDPriceLevel();
    float price = entry.mDEntryPx().mantissa() *
                  std::pow(10, entry.mDEntryPx().exponent());
    int volume = entry.mDEntrySize();

    if (entry.mDEntryType() == MDEntryType::Bid) {
      book_.add_bid(level, price, volume);
    } else if (entry.mDEntryType() == MDEntryType::Offer) {
      book_.add_ask(level, price, volume);
    }
  }
}

void Decoder::ask_entry(MDUpdateAction::Value action, int level, float price,
                        int volume) {
  switch (action) {
  case MDUpdateAction::New:
    book_.add_ask(level, price, volume);
    break;
  case MDUpdateAction::Change:
    book_.update_ask(level, price, volume);
    break;
  case MDUpdateAction::Delete:
    book_.delete_ask(level, price);
    break;
  case MDUpdateAction::DeleteFrom:
    book_.delete_ask_from(level);
    break;
  case MDUpdateAction::DeleteThru:
    book_.delete_ask_thru(level);
    break;
  }
}

void Decoder::bid_entry(MDUpdateAction::Value action, int level, float price,
                        int volume) {
  switch (action) {
  case MDUpdateAction::New:
    book_.add_bid(level, price, volume);
    break;
  case MDUpdateAction::Change:
    book_.update_bid(level, price, volume);
    break;
  case MDUpdateAction::Delete:
    book_.delete_bid(level, price);
    break;
  case MDUpdateAction::DeleteFrom:
    book_.delete_bid_from(level);
    break;
  case MDUpdateAction::DeleteThru:
    book_.delete_bid_thru(level);
    break;
  }
}

size_t Decoder::decode_incremental_refresh_book(
    MDIncrementalRefreshBook32 &refresh, char *buffer, uint64_t offset,
    uint64_t block_length, uint64_t version, uint64_t buffer_length) {

  refresh.wrapForDecode(buffer, offset, block_length, version, buffer_length);

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
      bid_entry(entry.mDUpdateAction(), level, price, volume);
    } else if (entry.mDEntryType() == MDEntryTypeBook::Offer) {
      ask_entry(entry.mDUpdateAction(), level, price, volume);
    }
  }
}

size_t Decoder::decode_message_length(char *buffer, uint64_t offset) {
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
      decode_header(header_, buffer, offset + kMsgSize, sizeof(buffer));

  uint64_t msg_offset = offset + kMsgSize + header_length;

  switch (header_.templateId()) {

  case MDIncrementalRefreshBook32::sbeTemplateId():
    decode_incremental_refresh_book(incremental_refresh_book_, buffer,
                                    msg_offset, header_.blockLength(),
                                    header_.version(), sizeof(buffer));
    break;

  case SnapshotFullRefresh38::sbeTemplateId():
    decode_snapshot(snapshot_full_, buffer, msg_offset, header_.blockLength(),
                    header_.version(), sizeof(buffer);
    break;


  case MDIncrementalRefreshVolume37::sbeTemplateId():
      decode_incremental_refresh_volume();
      break;

  case MDIncrementalRefreshTradeSummary42::sbeTemplateId():
      decode_incremental_refresh_trade();
      break;

  case MDIncrementalRefreshOrderBook43::sbeTemplateId():
      decode_incremental_refresh_book();
      break;
  default:
      std::cout << "Unknown message type, templateId: " << header_.templateId() << '\n';
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
