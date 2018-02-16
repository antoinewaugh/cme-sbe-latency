#pragma once

#include <ostream>

namespace sp {
namespace lltp {
namespace cme {

  struct SessionStatistics {

    double lowlimitprice = 0;
    double highlimitprice = 0;
    double openingprice = 0;
    double fixingprice = 0;
    double indictiveopeningprice = 0;
    double sessionhighbid = 0;
    double sessionlowask = 0;
    double sessionhighprice = 0;
    double sessionlowprice = 0;
    double settlement = 0;
    int openinterest = 0;
    int clearedvolume = 0;
    int electronicvolume = 0;
    double thresholdprice = 0;
    int thresholdvolume = 0;

    friend std::ostream &operator<<(std::ostream &os, const SessionStatistics &statistics);
  };

}}} // sp.lltp.cme