#include "Decoder.h"

#include <cmath>

//using IncrementalTrade = MDIncrementalRefreshTrade36;
//using IncrementalTradeSummary = MDIncrementalRefreshTradeSummary42;
//using IncrementalVolume = MDIncrementalRefreshVolume37;

Decoder::Decoder(uint64_t securityid, const Handler &handler,
                 std::function<void(ChannelStatus)> OnChannelStatus)
    : securityid_(securityid), handler_(handler),
      cb_channel_(OnChannelStatus) {

  StartRecovery(); // need to start in a state to sync

}

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
    MDIncrementalRefreshTrade36 &refresh, Decoder::Message message) {

  refresh.wrapForDecode(message.buffer, message.offset, message.block_length,
                        message.version, message.buffer_length);

  auto &entry = refresh.noMDEntries();
  while (entry.hasNext()) {
    entry.next();

    if (CheckStream(entry)) {
    }
  }
}

bool Decoder::DecodeIncrementalRefreshTradeSummary(

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

  if (!market_recovery_)
    return false;

  if (refresh.securityID() != securityid_)
    return false;

  auto &entry = refresh.noMDEntries();

  book_.Clear();

  rpt_seq_ = refresh.rptSeq();

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

  handler_.OnQuote(book_, market_recovery_, securityid_, rpt_seq_);

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

      handler_.OnQuote(book_, market_recovery_, securityid_, rpt_seq_);
    }
  }
}

uint32_t Decoder::GetSeqNum(char *buffer, size_t received) {
  if(received > 4) {
    return SBE_LITTLE_ENDIAN_ENCODE_32(*((std::uint32_t *) (buffer)));
  } return 0;
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
  auto header_length =
      DecodeHeader(messages_.header, buffer, offset + kMsgSize, 4096);

  uint64_t msg_offset = offset + kMsgSize + header_length;

  auto message = Message{buffer, msg_offset, messages_.header.blockLength(),
                         messages_.header.version(), 4096};

  // TODO: order by historical occurrence
  switch (messages_.header.templateId()) {

  case MDIncrementalRefreshBook32::sbeTemplateId():
    DecodeIncrementalRefreshBook(messages_.incremental_refresh_book, message);
    break;

  case SnapshotFullRefresh38::sbeTemplateId():
    DecodeSnapshot(messages_.snapshot_full, message);
    break;

  case SnapshotFullRefreshOrderBook44::sbeTemplateId():
    DecodeSnapshotFullRefreshOrderbook(messages_.snapshot_full_order_book,
                                       message);
    break;

  case MDIncrementalRefreshVolume37::sbeTemplateId():
    DecodeIncrementalRefreshVolume(messages_.incremental_volume, message);
    break;

  case MDIncrementalRefreshTrade36::sbeTemplateId():
    DecodeIncrementalRefreshTrade(messages_.incremental_trade, message);
    break;

  case MDIncrementalRefreshTradeSummary42::sbeTemplateId():
    DecodeIncrementalRefreshTradeSummary(messages_.incremental_trade_summary,
                                         message);
    break;

  case MDIncrementalRefreshOrderBook43::sbeTemplateId():
    DecodeIncrementalRefreshOrderBook(messages_.incremental_order_book,
                                      message);
    break;

  case MDIncrementalRefreshLimitsBanding34::sbeTemplateId():
    DecodeIncrementalRefreshLimitsBanding(messages_.limit_banding, message);
    break;

  case MDIncrementalRefreshDailyStatistics33::sbeTemplateId():
    DecodeIncrementalRefreshDailyStatistics(messages_.daily_statistics,
                                            message);
    break;

  case MDIncrementalRefreshSessionStatistics35::sbeTemplateId():
    DecodeIncrementalRefreshSessionStatistics(messages_.session_statistics,
                                              message);
    break;

  // less common messages

  case ChannelReset4::sbeTemplateId():
    DecodeChannelReset(messages_.channel_reset, message);
    break;

  case AdminHeartbeat12::sbeTemplateId():
    DecodeAdminHeartbeat(messages_.admin_heartbeat, message);
    break;

  case MDInstrumentDefinitionFuture27::sbeTemplateId():
    DecodeInstrumentDefinitionFuture(messages_.definition_future, message);
    break;

  case MDInstrumentDefinitionSpread29::sbeTemplateId():
    DecodeInstrumentDefinitionSpread(messages_.definition_spread, message);
    break;

  case MDInstrumentDefinitionOption41::sbeTemplateId():
    DecodeInstrumentDefinitionOption(messages_.definition_option, message);
    break;

  case QuoteRequest39::sbeTemplateId():
    DecodeQuoteRequest(messages_.quote_request, message);
    break;

  case SecurityStatus30::sbeTemplateId():
    DecodeSecurityStatus(messages_.security_status, message);
    break;

  default:
    std::cout << "Unknown message type, templateId: "
              << messages_.header.templateId() << '\n';
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

  if (!market_recovery_) {

    std::cout << "\nStarting Recovery";

    market_recovery_ = true;
    rpt_seq_ = 0; // ensures subsequent incrementals are ignored until snapshot
                 // alignment


    book_.Clear();
    handler_.OnQuote(book_, market_recovery_, securityid_, rpt_seq_);
  }
}

void Decoder::StopRecovery() {

  if (market_recovery_) {
    std::cout << "\nStopping Recovery";
    market_recovery_ = false;
    cb_channel_(ChannelStatus::MarketRecovered);
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
    case MDUpdateAction::New: book_.AddBid(level, price, volume); break;
    case MDUpdateAction::Change: book_.UpdateBid(level, price, volume);break;
    case MDUpdateAction::Delete: book_.DeleteBid(level, price); break;
    case MDUpdateAction::DeleteFrom: book_.DeleteBidFrom(level);break;
    case MDUpdateAction::DeleteThru: book_.DeleteBidThru(level);break;
  }
}

template <class T> bool Decoder::CheckStream(T entry) {

  if (entry.securityID() != securityid_) {
    return false;
  }

  if (entry.rptSeq() < rpt_seq_ + 1) {
    return false; // redundant packet
  }

  if (entry.rptSeq() > rpt_seq_ + 1) {
    //std::cout << "Missing SeqNum identified, recovering. RptSeq: "
    //          << entry.rptSeq() << ", ExRptSeq: " << rpt_seq_ << '\n';
    //StartRecovery();
    return false;
  }

  if (market_recovery_)
    StopRecovery();

  ++rpt_seq_;

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

bool Decoder::DecodeChannelReset(ChannelReset4 &refresh,
                                 Decoder::Message message) {

  refresh.wrapForDecode(message.buffer, message.offset, message.block_length,
                        message.version, message.buffer_length);

  // no need to check seqnum

  //ClearAllStates();
}

void Decoder::ClearAllStates() {

  book_.Clear();
  //  trades_.Clear();
  //  status_.Clear();

  // inform user of state clearing
  // handler_.OnQuote()
  // confirm we only call back after an event complete
}

bool Decoder::DecodeAdminHeartbeat(AdminHeartbeat12 &refresh,
                                   Decoder::Message message) {

  refresh.wrapForDecode(message.buffer, message.offset, message.block_length,
                        message.version, message.buffer_length);

  // reset heartbeat timeout
  // on timeout ClearAllStates();
}

bool Decoder::DecodeInstrumentDefinitionFuture(
    MDInstrumentDefinitionFuture27 &refresh, Decoder::Message message) {

  refresh.wrapForDecode(message.buffer, message.offset, message.block_length,
                        message.version, message.buffer_length);
  return false;
}

bool Decoder::DecodeInstrumentDefinitionOption(
    MDInstrumentDefinitionOption41 &refresh, Decoder::Message message) {

  refresh.wrapForDecode(message.buffer, message.offset, message.block_length,
                        message.version, message.buffer_length);

  return false;
}

bool Decoder::DecodeInstrumentDefinitionSpread(
    MDInstrumentDefinitionSpread29 &refresh, Decoder::Message message) {

  refresh.wrapForDecode(message.buffer, message.offset, message.block_length,
                        message.version, message.buffer_length);

  return false;
}

bool Decoder::DecodeQuoteRequest(QuoteRequest39 &refresh,
                                 Decoder::Message message) {

  refresh.wrapForDecode(message.buffer, message.offset, message.block_length,
                        message.version, message.buffer_length);

  //
  return false;
}

bool Decoder::DecodeSecurityStatus(SecurityStatus30 &refresh,
                                   Decoder::Message message) {

  refresh.wrapForDecode(message.buffer, message.offset, message.block_length,
                        message.version, message.buffer_length);

  return false;
}

bool Decoder::DecodeSnapshotFullRefreshOrderbook(
    SnapshotFullRefreshOrderBook44 &refresh, Decoder::Message message) {
  refresh.wrapForDecode(message.buffer, message.offset, message.block_length,
                        message.version, message.buffer_length);
  return false;
}

void Decoder::Clear() {
  rpt_seq_ = 0; // prepare for recovery
  book_.Clear();
}
