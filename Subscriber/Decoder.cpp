#include "Decoder.h"
#include "SymbolFeed.h"
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

  cb_snapshotfull_(refresh);
}

size_t
Decoder::decode_incremental_refresh_book(MDIncrementalRefreshBook32 &refresh,
                                         Decoder::Message message) {

  refresh.wrapForDecode(message.buffer, message.offset, message.block_length,
                        message.version, message.buffer_length);

  cb_book_(refresh);
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

int Decoder::RegisterCallbacks(

    std::function<void(MDIncrementalRefreshBook32&)> cb_book,
    std::function<void(MDIncrementalRefreshDailyStatistics33&)> cb_dailystatistics,
    std::function<void(MDIncrementalRefreshLimitsBanding34&)> cb_limitsbanding,
    std::function<void(MDIncrementalRefreshSessionStatistics35&)> cb_sessionstatistics,
    std::function<void(MDIncrementalRefreshTrade36&)> cb_trade,
    std::function<void(MDIncrementalRefreshVolume37&)> cb_volume,
    std::function<void(MDIncrementalRefreshTradeSummary42&)> cb_tradesummary,
    std::function<void(MDIncrementalRefreshOrderBook43&)> cb_orderbook,
    std::function<void(SnapshotFullRefresh38&)> cb_snapshotfull,
    std::function<void(SnapshotFullRefreshOrderBook44&)> cb_snapshotorderbook
) {
  cb_book_ = cb_book;
  cb_dailystatistics_ = cb_dailystatistics;
  cb_limitsbanding_ = cb_limitsbanding;
  cb_sessionstatistics_ = cb_sessionstatistics;
  cb_trade_ = cb_trade;
  cb_volume_ = cb_volume;
  cb_tradesummary_ = cb_tradesummary;
  cb_orderbook_ = cb_orderbook;
  cb_snapshotfull_ = cb_snapshotfull;
  cb_snapshotorderbook_ = cb_snapshotorderbook;

}

size_t Decoder::DecodePacket(char *buffer, size_t received) {
  size_t processed = kByteOffest;
  while (processed < received) {
    processed += decode_message(buffer, processed);
  }
}

