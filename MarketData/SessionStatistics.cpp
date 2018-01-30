
#include "SessionStatistics.h"

namespace sp {
namespace lltp {
namespace cme {

  std::ostream &operator<<(std::ostream &os, const SessionStatistics &statistics) {
    os << "lowlimitprice: " << statistics.lowlimitprice << " highlimitprice: " << statistics.highlimitprice
       << " openingprice: " << statistics.openingprice << " fixingprice: " << statistics.fixingprice
       << " indictiveopeningprice: " << statistics.indictiveopeningprice << " sessionhighbid: "
       << statistics.sessionhighbid << " sessionlowask: " << statistics.sessionlowask << " sessionhighprice: "
       << statistics.sessionhighprice << " sessionlowprice: " << statistics.sessionlowprice << " settlement: "
       << statistics.settlement << " openinterest: " << statistics.openinterest << " clearedvolume: "
       << statistics.clearedvolume << " electronicvolume: " << statistics.electronicvolume << " thresholdprice: "
       << statistics.thresholdprice << " thresholdvolume: " << statistics.thresholdvolume;
    return os;
  }
}}} // sp.lltp.cme
