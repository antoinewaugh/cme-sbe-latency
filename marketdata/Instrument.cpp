
#include "Instrument.h"

#include <iomanip>
#include <chrono>

namespace sp {
namespace lltp {
namespace cme {

  std::ostream &operator<<(std::ostream &os, const Instrument &instrument) {
    std::time_t activation_t = instrument.activationdatetime / 1000000000;
    std::time_t expiry_t = instrument.expirationdatetime / 1000000000;

    os << "securityid: " << instrument.securityid << " symbol: " << instrument.symbol << " securitygroup: "
       << instrument.securitygroup << " type: " << instrument.type << " marketsegmentid: " << instrument.marketsegmentid
       << " activation: " << std::put_time(std::localtime(&activation_t), "%Y-%m-%d %X") << " expiry: "
       << std::put_time(std::localtime(&expiry_t), "%Y-%m-%d %X");
    return os;
  }

}}}  // namespace
