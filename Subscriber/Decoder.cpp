#include "Decoder.h"

Decoder::Decoder(uint64_t securityid, const Handler &handler,
                 std::function<void(SeqNumStatus)> OnSeqNumStatus)
    : securityid_(securityid), handler_(handler),
      cb_seqnumstatus_(OnSeqNumStatus) {}

bool Decoder::DecodeIncrementalRefreshVolume(
    MDIncrementalRefreshVolume37 &refresh, Decoder::Message message) {

  refresh.wrapForDecode(message.buffer, message.offset, message.block_length,
                        message.version, message.buffer_length);

  auto &entry = refresh.noMDEntries();

  while (entry.hasNext()) {
    entry.next();

    if (CheckStream(entry)) {
    }
  }
}

bool Decoder::DecodeIncrementalRefreshTrade(
    MDIncrementalRefreshTradeSummary42 &refresh, Decoder::Message message) {

  refresh.wrapForDecode(message.buffer, message.offset, message.block_length,
                        message.version, message.buffer_length);

  auto &entry = refresh.noMDEntries();
  while (entry.hasNext()) {
    entry.next();

    if (CheckStream(entry)) {
    }
  }
}

bool Decoder::DecodeIncrementalRefreshOrderBook(
    MDIncrementalRefreshOrderBook43 &refresh, Decoder::Message message) {

  refresh.wrapForDecode(message.buffer, message.offset, message.block_length,
                        message.version, message.buffer_length);

  // do nothing
}

bool Decoder::DecodeSnapshot(SnapshotFullRefresh38 &refresh,
                             Decoder::Message message) {

  refresh.wrapForDecode(message.buffer, message.offset, message.block_length,
                        message.version, message.buffer_length);

  if (!recoverymode_)
    return false;

  if (refresh.securityID() != securityid_)
    return false;

  auto &entry = refresh.noMDEntries();

  book_.Clear();

  seqnum_ = refresh.rptSeq();

  while (entry.hasNext()) {
    entry.next();

    int level = entry.mDPriceLevel();
    float price = entry.mDEntryPx().mantissa() *
                  std::pow(10, entry.mDEntryPx().exponent());
    int volume = entry.mDEntrySize();

    if (entry.mDEntryType() == MDEntryType::Bid) {
      book_.AddBid(level, price, volume);
    } else if (entry.mDEntryType() == MDEntryType::Offer) {
      book_.AddAsk(level, price, volume);
    }
  }

  handler_.OnQuote(book_, recoverymode_, securityid_, seqnum_);

  return true;
}

bool Decoder::DecodeIncrementalRefreshBook(MDIncrementalRefreshBook32 &refresh,
                                           Decoder::Message message) {

  refresh.wrapForDecode(message.buffer, message.offset, message.block_length,
                        message.version, message.buffer_length);

  auto &entry = refresh.noMDEntries();

  while (entry.hasNext()) {

    entry.next();

    if (CheckStream(entry)) {

      int level = entry.mDPriceLevel();
      float price = entry.mDEntryPx().mantissa() *
                    std::pow(10, entry.mDEntryPx().exponent());
      int volume = entry.mDEntrySize();

      if (entry.mDEntryType() == MDEntryTypeBook::Bid) {
        HandleBidEntry(entry.mDUpdateAction(), level, price, volume);
      } else if (entry.mDEntryType() == MDEntryTypeBook::Offer) {
        HandleAskEntry(entry.mDUpdateAction(), level, price, volume);
      }

      handler_.OnQuote(book_, recoverymode_, securityid_, seqnum_);
    }
  }
}

size_t Decoder::DecodeMessageLength(char *buffer, size_t offset) {
  return SBE_LITTLE_ENDIAN_ENCODE_16(*((std::uint16_t *)(buffer + offset)));
}

size_t Decoder::DecodeHeader(MessageHeader &header, char *buffer,
                             std::uint64_t offset,
                             std::uint64_t buffer_length) {

  header.wrap(buffer, offset, kMsgHeaderVersion, buffer_length);
  return header.encodedLength();
}

size_t Decoder::DecodeMessage(char *buffer, uint64_t offset) {

  auto message_length = DecodeMessageLength(buffer, offset);
  auto header_length = DecodeHeader(header_, buffer, offset + kMsgSize, 4096);

  uint64_t msg_offset = offset + kMsgSize + header_length;

  auto message = Message{buffer, msg_offset, header_.blockLength(),
                         header_.version(), 4096};

  switch (header_.templateId()) {

  case MDIncrementalRefreshBook32::sbeTemplateId():
    DecodeIncrementalRefreshBook(incremental_refresh_book_, message);
    break;

  case SnapshotFullRefresh38::sbeTemplateId():
    DecodeSnapshot(snapshot_full_, message);
    break;

  case MDIncrementalRefreshVolume37::sbeTemplateId():
    DecodeIncrementalRefreshVolume(incremental_volume_, message);
    break;

  case MDIncrementalRefreshTradeSummary42::sbeTemplateId():
    DecodeIncrementalRefreshTrade(incremental_trade_, message);
    break;

  case MDIncrementalRefreshOrderBook43::sbeTemplateId():
    DecodeIncrementalRefreshOrderBook(incremental_order_book_, message);
    break;

  case MDIncrementalRefreshDailyStatistics33::sbeTemplateId():
    DecodeIncrementalRefreshOrderBook(incremental_order_book_, message);
    break;

  default:
    std::cout << "Unknown message type, templateId: " << header_.templateId()
              << '\n';
    break;
  }
  return message_length;
}

size_t Decoder::DecodePacket(char *buffer, size_t received) {

  size_t processed = kByteOffest;
  while (processed < received) {
    processed += DecodeMessage(buffer, processed);
  }
}

void Decoder::StartRecovery() {

  if (!recoverymode_) {

    std::cout << "\nStarting Recovery";

    recoverymode_ = true;
    seqnum_ = 0; // ensures subsequent incrementals are ignored until snapshot
                 // alignment

    cb_seqnumstatus_(SeqNumStatus(SeqNumStatus::Unsynchronised));

    // call back with cleared book
    book_.Clear();
    handler_.OnQuote(book_, recoverymode_, securityid_, seqnum_);
  }
}

void Decoder::StopRecovery() {

  if (recoverymode_) {
    std::cout << "\nStopping Recovery";
    recoverymode_ = false;
    cb_seqnumstatus_(SeqNumStatus(SeqNumStatus::Unsynchronised));
  }
}

void Decoder::HandleAskEntry(MDUpdateAction::Value action, int level,

                             float price, int volume) {
  switch (action) {
  case MDUpdateAction::New:
    book_.AddAsk(level, price, volume);
    break;
  case MDUpdateAction::Change:
    book_.UpdateAsk(level, price, volume);
    break;
  case MDUpdateAction::Delete:
    book_.DeleteAsk(level, price);
    break;
  case MDUpdateAction::DeleteFrom:
    book_.DeleteAskFrom(level);
    break;
  case MDUpdateAction::DeleteThru:
    book_.DeleteAskThru();
    break;
  }
}
void Decoder::HandleBidEntry(MDUpdateAction::Value action, int level,
                             float price, int volume) {
  switch (action) {
  case MDUpdateAction::New:
    book_.AddBid(level, price, volume);
    break;
  case MDUpdateAction::Change:
    book_.UpdateBid(level, price, volume);
    break;
  case MDUpdateAction::Delete:
    book_.DeleteBid(level, price);
    break;
  case MDUpdateAction::DeleteFrom:
    book_.DeleteBidFrom(level);
    break;
  case MDUpdateAction::DeleteThru:
    book_.DeleteBidThru(level);
    break;
  }
}

template <class T> bool Decoder::CheckStream(T entry) {

  if (entry.securityID() != securityid_) {
    return false;
  }

  if (entry.rptSeq() < seqnum_ + 1) {
    return false; // redundant packet
  }

  if (entry.rptSeq() > seqnum_ + 1) {
    std::cout << "Missing SeqNum identified, recovering. RptSeq: "
              << entry.rptSeq() << ", ExRptSeq: " << seqnum_ << '\n';
    StartRecovery();
    return false;
  }

  if (recoverymode_)
    StopRecovery();

  ++seqnum_;

  return true;
}

bool Decoder::DecodeIncrementalRefreshDailyStatistics(
    MDIncrementalRefreshDailyStatistics33 &refresh, Decoder::Message message) {

  refresh.wrapForDecode(message.buffer, message.offset, message.block_length,
                        message.version, message.buffer_length);

  auto &entry = refresh.noMDEntries();
  while (entry.hasNext()) {
    entry.next();

    if (CheckStream(entry)) {
    }
  }
}

bool Decoder::DecodeIncrementalRefreshLimitsBanding(
    MDIncrementalRefreshLimitsBanding34 &refresh, Decoder::Message message) {

  refresh.wrapForDecode(message.buffer, message.offset, message.block_length,
                        message.version, message.buffer_length);

  auto &entry = refresh.noMDEntries();
  while (entry.hasNext()) {
    entry.next();

    if (CheckStream(entry)) {
    }
  }
}

bool Decoder::DecodeIncrementalRefreshSessionStatistics(
    MDIncrementalRefreshSessionStatistics35 &refresh,
    Decoder::Message message) {

  refresh.wrapForDecode(message.buffer, message.offset, message.block_length,
                        message.version, message.buffer_length);

  auto &entry = refresh.noMDEntries();
  while (entry.hasNext()) {
    entry.next();

    if (CheckStream(entry)) {
    }
  }
}
