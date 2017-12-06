#include "Message.h"

#include <type_traits>

ChannelReset4& Message::GetChannelReset() {
  channel_reset.wrapForDecode(buffer, offset, block_length, version, buffer_length);
  return channel_reset;
}

AdminHeartbeat12& Message::GetAdminHeartbeat() {
  admin_heartbeat.wrapForDecode(buffer, offset, block_length, version, buffer_length);
  return admin_heartbeat;
}

MDInstrumentDefinitionFuture27& Message::GetInstrumentDefinitionFuture() {
  definition_future.wrapForDecode(buffer, offset, block_length, version, buffer_length);
  return definition_future;
}

MDInstrumentDefinitionOption41 & Message::GetInstrumentDefinitionOption() {
  definition_option.wrapForDecode(buffer, offset, block_length, version, buffer_length);
  return definition_option;
}

MDInstrumentDefinitionSpread29& Message::GetInstrumentDefinitionSpread() {
  definition_spread.wrapForDecode(buffer, offset, block_length, version, buffer_length);
  return definition_spread;
}

QuoteRequest39 &Message::GetQuoteRequest() {
  quote_request.wrapForDecode(buffer, offset, block_length, version, buffer_length);
  return quote_request;
}

SecurityStatus30 &Message::GetSecurityStatus() {
  security_status.wrapForDecode(buffer, offset, block_length, version, buffer_length);
  return security_status;
}

SnapshotFullRefresh38 &Message::GetSnapshotFullRefresh() {
  snapshot_full.wrapForDecode(buffer, offset, block_length, version, buffer_length);
  return snapshot_full;
}

SnapshotFullRefreshOrderBook44 &Message::GetSnapshotFullRefreshOrderBook() {
  snapshot_full_order_book.wrapForDecode(buffer, offset, block_length, version, buffer_length);
  return snapshot_full_order_book;
}

MDIncrementalRefreshBook32 &Message::GetIncrementalRefreshBook() {
  incremental_refresh_book.wrapForDecode(buffer, offset, block_length, version, buffer_length);
  return incremental_refresh_book;
}

MDIncrementalRefreshVolume37 &Message::GetIncrementalRefreshVolume() {
  incremental_volume.wrapForDecode(buffer, offset, block_length, version, buffer_length);
  return incremental_volume;
}

MDIncrementalRefreshTrade36 &Message::GetIncrementalRefreshTrade() {
  incremental_trade.wrapForDecode(buffer, offset, block_length, version, buffer_length);
  return incremental_trade;
}

MDIncrementalRefreshOrderBook43 &Message::GetIncrementalRefreshOrderBook() {
  incremental_order_book.wrapForDecode(buffer, offset, block_length, version, buffer_length);
  return incremental_order_book;
}

MDIncrementalRefreshSessionStatistics35 &Message::GetIncrementalRefreshSessionStatistics() {
  session_statistics.wrapForDecode(buffer, offset, block_length, version, buffer_length);
  return session_statistics;
}

MDIncrementalRefreshLimitsBanding34 &Message::GetIncrementalRefreshLimitsBanding() {
  limit_banding.wrapForDecode(buffer, offset, block_length, version, buffer_length);
  return limit_banding;
}

MDIncrementalRefreshDailyStatistics33 &Message::GetIncrementalRefreshDailyStatistics() {
  daily_statistics.wrapForDecode(buffer, offset, block_length, version, buffer_length);
  return daily_statistics;
};

template<typename T>
T Message::Get() {
  if(std::is_same<T, ChannelReset4>()) { return GetChannelReset(); };
  if(std::is_same<T, AdminHeartbeat12>()) { return GetAdminHeartbeat(); };
  if(std::is_same<T, MDInstrumentDefinitionFuture27>()) { return GetInstrumentDefinitionFuture(); };
  if(std::is_same<T, MDInstrumentDefinitionOption41>()) { return GetInstrumentDefinitionOption(); };
  if(std::is_same<T, MDInstrumentDefinitionSpread29>()) { return GetInstrumentDefinitionSpread(); };
  if(std::is_same<T, QuoteRequest39>()) { return GetQuoteRequest(); };
  if(std::is_same<T, SecurityStatus30>()) { return GetSecurityStatus(); };
  if(std::is_same<T, SnapshotFullRefresh38>()) { return GetSnapshotFullRefresh(); };
  if(std::is_same<T, SnapshotFullRefreshOrderBook44>()) { return GetSnapshotFullRefreshOrderBook(); };
  if(std::is_same<T, MDIncrementalRefreshBook32>()) { return GetIncrementalRefreshBook(); };
  if(std::is_same<T, MDIncrementalRefreshVolume37>()) { return GetIncrementalRefreshVolume(); };
  if(std::is_same<T, MDIncrementalRefreshTrade36>()) { return GetIncrementalRefreshTrade(); };
  if(std::is_same<T, MDIncrementalRefreshOrderBook43>()) { return GetIncrementalRefreshOrderBook(); };
  if(std::is_same<T, MDIncrementalRefreshSessionStatistics35>()) { return GetIncrementalRefreshSessionStatistics(); };
  if(std::is_same<T, MDIncrementalRefreshLimitsBanding34>()) { return GetIncrementalRefreshLimitsBanding(); };
  if(std::is_same<T, MDIncrementalRefreshDailyStatistics33>()) { return GetIncrementalRefreshDailyStatistics(); };
}

