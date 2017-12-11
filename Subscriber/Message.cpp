#include "Message.h"

#include <type_traits>


ChannelReset4& Message::GetChannelReset() {
  channel_reset.wrapForDecode(buffer_, offset_, header.blockLength(), header.version(), header.bufferLength());
  return channel_reset;
}

AdminHeartbeat12& Message::GetAdminHeartbeat() {
  admin_heartbeat.wrapForDecode(buffer_, offset_, header.blockLength(), header.version(), header.bufferLength());
  return admin_heartbeat;
}

MDInstrumentDefinitionFuture27& Message::GetInstrumentDefinitionFuture() {
  definition_future.wrapForDecode(buffer_, offset_, header.blockLength(), header.version(), header.bufferLength());
  return definition_future;
}

MDInstrumentDefinitionOption41& Message::GetInstrumentDefinitionOption() {
  definition_option.wrapForDecode(buffer_, offset_, header.blockLength(), header.version(), header.bufferLength());
  return definition_option;
}

MDInstrumentDefinitionSpread29& Message::GetInstrumentDefinitionSpread() {
  definition_spread.wrapForDecode(buffer_, offset_, header.blockLength(), header.version(), header.bufferLength());
  return definition_spread;
}

QuoteRequest39 &Message::GetQuoteRequest() {
  quote_request.wrapForDecode(buffer_, offset_, header.blockLength(), header.version(), header.bufferLength());
  return quote_request;
}

SecurityStatus30 &Message::GetSecurityStatus() {
  security_status.wrapForDecode(buffer_, offset_, header.blockLength(), header.version(), header.bufferLength());
  return security_status;
}

SnapshotFullRefresh38 &Message::GetSnapshotFullRefresh() {
  snapshot_full.wrapForDecode(buffer_, offset_, header.blockLength(), header.version(), header.bufferLength());
  return snapshot_full;
}

SnapshotFullRefreshOrderBook44 &Message::GetSnapshotFullRefreshOrderBook() {
  snapshot_full_order_book.wrapForDecode(buffer_, offset_, header.blockLength(), header.version(), header.bufferLength());
  return snapshot_full_order_book;
}

MDIncrementalRefreshBook32 &Message::GetIncrementalRefreshBook() {
  incremental_refresh_book.wrapForDecode(buffer_, offset_, header.blockLength(), header.version(), header.bufferLength());
  return incremental_refresh_book;
}

MDIncrementalRefreshVolume37 &Message::GetIncrementalRefreshVolume() {
  incremental_volume.wrapForDecode(buffer_, offset_, header.blockLength(), header.version(), header.bufferLength());
  return incremental_volume;
}

MDIncrementalRefreshTrade36 &Message::GetIncrementalRefreshTrade() {
  incremental_trade.wrapForDecode(buffer_, offset_, header.blockLength(), header.version(), header.bufferLength());
  return incremental_trade;
}

MDIncrementalRefreshOrderBook43 &Message::GetIncrementalRefreshOrderBook() {
  incremental_order_book.wrapForDecode(buffer_, offset_, header.blockLength(), header.version(), header.bufferLength());
  return incremental_order_book;
}

MDIncrementalRefreshSessionStatistics35 &Message::GetIncrementalRefreshSessionStatistics() {
  session_statistics.wrapForDecode(buffer_, offset_, header.blockLength(), header.version(), header.bufferLength());
  return session_statistics;
}

MDIncrementalRefreshLimitsBanding34 &Message::GetIncrementalRefreshLimitsBanding() {
  limit_banding.wrapForDecode(buffer_, offset_, header.blockLength(), header.version(), header.bufferLength());
  return limit_banding;
}

MDIncrementalRefreshDailyStatistics33 &Message::GetIncrementalRefreshDailyStatistics() {
  daily_statistics.wrapForDecode(buffer_, offset_, header.blockLength(), header.version(), header.bufferLength());
  return daily_statistics;
};

//template<typename T>
//T Message::Get() {
//  if(std::is_same<T, ChannelReset4>()) { return GetChannelReset(); };
//  if(std::is_same<T, AdminHeartbeat12>()) { return GetAdminHeartbeat(); };
//  if(std::is_same<T, MDInstrumentDefinitionFuture27>()) { return GetInstrumentDefinitionFuture(); };
//  if(std::is_same<T, MDInstrumentDefinitionOption41>()) { return GetInstrumentDefinitionOption(); };
//  if(std::is_same<T, MDInstrumentDefinitionSpread29>()) { return GetInstrumentDefinitionSpread(); };
//  if(std::is_same<T, QuoteRequest39>()) { return GetQuoteRequest(); };
//  if(std::is_same<T, SecurityStatus30>()) { return GetSecurityStatus(); };
//  if(std::is_same<T, SnapshotFullRefresh38>()) { return GetSnapshotFullRefresh(); };
//  if(std::is_same<T, SnapshotFullRefreshOrderBook44>()) { return GetSnapshotFullRefreshOrderBook(); };
//  if(std::is_same<T, MDIncrementalRefreshBook32>()) { return GetIncrementalRefreshBook(); };
//  if(std::is_same<T, MDIncrementalRefreshVolume37>()) { return GetIncrementalRefreshVolume(); };
//  if(std::is_same<T, MDIncrementalRefreshTrade36>()) { return GetIncrementalRefreshTrade(); };
//  if(std::is_same<T, MDIncrementalRefreshOrderBook43>()) { return GetIncrementalRefreshOrderBook(); };
//  if(std::is_same<T, MDIncrementalRefreshSessionStatistics35>()) { return GetIncrementalRefreshSessionStatistics(); };
//  if(std::is_same<T, MDIncrementalRefreshLimitsBanding34>()) { return GetIncrementalRefreshLimitsBanding(); };
//  if(std::is_same<T, MDIncrementalRefreshDailyStatistics33>()) { return GetIncrementalRefreshDailyStatistics(); };
//}

uint16_t Message::GetMsgSize() {
  return msg_size_;
}

static uint16_t ExtractMsgSize(char* buffer, size_t offset) {
  return SBE_LITTLE_ENDIAN_ENCODE_16(*((std::uint16_t *)(buffer + offset)));
}
size_t Message::Reset(char* buffer, size_t offset, int buffer_length) {
  msg_size_ = ExtractMsgSize(buffer_, offset_);
  header.wrap(buffer, offset + kMsgSize, kMsgHeaderVersion, buffer_length);
  buffer_ = buffer;
  offset_ = offset + kMsgSize + header.encodedLength();
  return msg_size_;
}

uint16_t Message::GetTemplateId() {
  return header.templateId();
}

