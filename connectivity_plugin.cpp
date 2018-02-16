
#include "config.h"
#include "DepthBook.h"
#include "Trade.h"
#include "SessionStatistics.h"
#include "SecurityStatus.h"
#include <sag_connectivity_plugins.hpp>

#include <thread>
#include "boost/asio.hpp"
#include "ChannelImpl.h"
#include <chrono>

using namespace sp::lltp::cme;
using namespace com::softwareag::connectivity;

namespace sp {
namespace atp {
namespace cme {

using sp::lltp::cme::SecurityEvent;
using sp::lltp::cme::SecurityStatus ;
using sp::lltp::cme::SessionStatistics;

class HandlerImpl: public MarketDataListener {
  public:
    HandlerImpl(HostSide::ptr_t* host):host(host) {}
    HandlerImpl() = default;
    void OnQuote(uint64_t securityid,DepthBook const& book);
    void OnTrade(uint64_t securityid, Trade& trade);
    void OnStatus(uint64_t securityid, SecurityStatus status, SecurityEvent event);
    void OnStatistics(uint64_t securityid, SessionStatistics &statistics);

  private:
    void Send(DepthBook const& book);
    HostSide::ptr_t* host;
  };

//   void Subscribe(HostSide::ptr_t* hostSide) {
//       map_t payload;
//  payload.insert(data_t("symbol"), data_t("ESH8"));
//
//  list_t bidp, bidv, askp, askv;
//
//  payload[data_t("bid_prices")] = data_t(std::move(bidp));
//  payload[data_t("bid_volumes")] = data_t(std::move(bidv));
//  payload[data_t("ask_prices")] = data_t(std::move(askp));
//  payload[data_t("ask_volumes")] = data_t(std::move(askv));
//
//  com::softwareag::connectivity::Message msg(data_t(std::move(payload)));
//  msg.putMetadataValue(com::softwareag::connectivity::Message::HOST_MESSAGE_TYPE(), "com.apama.marketdata.Depth");
//     (*hostSide)->sendBatchTowardsHost(&msg, (&msg)+1);
//
//   }

void Subscribe(MarketDataListener* handler) {
    boost::asio::io_service io_service;
    auto listen_address = boost::asio::ip::address::from_string("0.0.0.0");
    auto configs = Config::load("config.xml");
    auto channel = ChannelImpl::make_channel("310", configs, io_service, listen_address);
    channel->RegisterMarketDataListener(handler);
    channel->Subscribe("ESH8");
    io_service.run();
}

class CMEConnectivityPlugin: public AbstractTransport {
public:
  CMEConnectivityPlugin(const TransportConstructorParameters& params)
      : AbstractTransport(params)
  {

  }

  virtual void sendBatchTowardsTransport(com::softwareag::connectivity::Message* start, com::softwareag::connectivity::Message* end) {}
  virtual void start() { logger.info("CME SBE plugin started."); }
  virtual void shutdown() { logger.info("CME SBE plugin shutting down."); }
  virtual void hostReady() {
      handler = HandlerImpl(&hostSide);
      std::thread t(Subscribe, &handler);
//      std::thread t(Subscribe, &hostSide);
      t.detach();
      logger.info("HostReady received.");
  }

private:
    HandlerImpl handler;
};

void HandlerImpl::OnQuote(uint64_t securityid, DepthBook const &book) {
  Send(book);
}

void HandlerImpl::Send(DepthBook const &book) {
  map_t payload;
  payload.insert(data_t("symbol"), data_t("ESH8"));

  list_t bidp, bidv, askp, askv;

  auto bids = book.Bids();
  for(int i = 0; i < bids.Size(); ++i) {
    auto level = bids.Get(i);
    bidp.push_back(data_t(level.price));
    bidv.push_back(data_t(static_cast<int64_t>(level.quantity)));
  }

  auto asks = book.Asks();
  for(int i = 0; i < asks.Size(); ++i) {
    auto level = asks.Get(i);
    askp.push_back(data_t(level.price));
    askv.push_back(data_t(static_cast<int64_t>(level.quantity)));
  }

  payload[data_t("bid_prices")] = data_t(std::move(bidp));
  payload[data_t("bid_volumes")] = data_t(std::move(bidv));
  payload[data_t("ask_prices")] = data_t(std::move(askp));
  payload[data_t("ask_volumes")] = data_t(std::move(askv));

  list_t timestamps;
  int64_t ts = std::chrono::duration_cast<std::chrono::nanoseconds>(
          std::chrono::system_clock::now().time_since_epoch()).count();

  timestamps.push_back(data_t(rec_ts));
  timestamps.push_back(data_t(ts));
  payload[data_t("timestamps")] = data_t(std::move(timestamps));

  com::softwareag::connectivity::Message msg(data_t(std::move(payload)));
  msg.putMetadataValue(com::softwareag::connectivity::Message::HOST_MESSAGE_TYPE(), "com.apama.marketdata.Depth");
  (*host)->sendBatchTowardsHost(&msg, (&msg) + 1);
}

void HandlerImpl::OnTrade(uint64_t securityid, Trade &trade) {

}

void HandlerImpl::OnStatus(uint64_t securityid, SecurityStatus status, SecurityEvent event) {

}

void HandlerImpl::OnStatistics(uint64_t securityid, SessionStatistics &statistics) {

}

SAG_DECLARE_CONNECTIVITY_TRANSPORT_CLASS(CMEConnectivityPlugin);

}}} // namespace
