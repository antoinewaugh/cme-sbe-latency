#pragma once

#include <ostream>

namespace sp {
namespace lltp {
namespace cme {

  struct SessionStatistics {

    int openingprice;
    int fixingprice;
    int indictiveopeningprice;
    int sessionhighbid;
    int sessionlowask;
    int sessionhighprice;
    int sessionlowprice;
    int settlement;

    friend std::ostream &operator<<(std::ostream &os, const SessionStatistics &statistics);

    int openinterest;
    int clearedvolume;

  };

}}} // sp.lltp.cme