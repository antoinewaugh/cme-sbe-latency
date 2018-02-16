#pragma once

#include <cstdint>

// sortino partners / low latency trading platform / cme
namespace sp {
namespace lltp {
namespace cme {

enum SecurityStatus {
  TradingHalt = (std::uint8_t)2,
  Close = (std::uint8_t)4,
  NewPriceIndication = (std::uint8_t)15,
  ReadyToTrade = (std::uint8_t)17,
  NotAvailableForTrading = (std::uint8_t)18,
  UnknownorInvalid = (std::uint8_t)20,
  PreOpen = (std::uint8_t)21,
  PreCross = (std::uint8_t)24,
  Cross = (std::uint8_t)25,
  PostClose = (std::uint8_t)26,
  NoChange = (std::uint8_t)103,
  NULL_STATUS = (std::uint8_t)255
};

enum SecurityEvent
{
  NoEvent = (std::uint8_t)0,
  NoCancel = (std::uint8_t)1,
  ResetStatistics = (std::uint8_t)4,
  ImpliedMatchingON = (std::uint8_t)5,
  ImpliedMatchingOFF = (std::uint8_t)6,
  NULL_EVENT = (std::uint8_t)255
};

}}} // sp.llts.cme

