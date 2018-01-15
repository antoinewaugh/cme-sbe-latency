
#include "SessionStatistics.h"

namespace sp {
namespace lltp {
namespace cme {

  std::ostream &operator<<(std::ostream &os, const SessionStatistics &statistics) {
    os << "openingprice: " << statistics.openingprice << " fixingprice: " << statistics.fixingprice
       << " indictiveopeningprice: " << statistics.indictiveopeningprice << " sessionhighbid: "
       << statistics.sessionhighbid << " sessionlowask: " << statistics.sessionlowask << " sessionhighprice: "
       << statistics.sessionhighprice << " sessionlowprice: " << statistics.sessionlowprice << " settlement: "
       << statistics.settlement << " openinterest: " << statistics.openinterest << " clearedvolume: "
       << statistics.clearedvolume;
    return os;
  }

}}} // sp.lltp.cme
