#pragma once

#include <string>

namespace sp {
namespace lltp {
namespace cme {

  struct Instrument {
    int securityid;
    std::string symbol;
    std::string securitygroup;
    std::string type;
    uint64_t activationdatetime;
    uint64_t expirationdatetime;
    int marketsegmentid;
    int marketdepth;
    int implmarketdepth;

    friend std::ostream &operator<<(std::ostream &os, const Instrument &instrument);
  };

}}}  // namespace
