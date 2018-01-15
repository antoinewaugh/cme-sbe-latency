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
  NULL_VALUE = (std::uint8_t)255
};

}}} // sp.llts.cme

