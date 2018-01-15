#pragma once

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

    int openinterest;
    int clearedvolume;

  };

}}} // sp.lltp.cme