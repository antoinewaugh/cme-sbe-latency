#pragma once

#include <ostream>

namespace sp {
namespace lltp {
namespace cme {

  struct SessionStatistics {

    int lowlimitprice = 0;
    int highlimitprice = 0;
    int openingprice = 0;
    int fixingprice = 0;
    int indictiveopeningprice = 0;
    int sessionhighbid = 0;
    int sessionlowask = 0;
    int sessionhighprice = 0;
    int sessionlowprice = 0;
    int settlement = 0;
    int openinterest = 0;
    int clearedvolume = 0;
    int electronicvolume = 0;

    friend std::ostream &operator<<(std::ostream &os, const SessionStatistics &statistics);
  };

}}} // sp.lltp.cme