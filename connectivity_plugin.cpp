
#include "DepthBook.h"
#include "Trade.h"
#include "SessionStatistics.h"
#include "SecurityStatus.h"

#include <sag_connectivity_plugins.hpp>

#include "boost/asio.hpp"
#include "Config.h"
#include "Channel.h"

using namespace sp::lltp::cme;
using namespace com::softwareag::connectivity;

namespace sp {
namespace atp {
namespace cme {

using sp::lltp::cme::SecurityEvent;
using sp::lltp::cme::SecurityStatus ;
using sp::lltp::cme::SessionStatistics;

class CMEConnectivityPlugin: public AbstractTransport {
public:
  CMEConnectivityPlugin(const TransportConstructorParameters& params)
      : AbstractTransport(params), handler(std::move(std::make_unique<HandlerImpl>(hostSide)))
  {}

  virtual void sendBatchTowardsTransport(com::softwareag::connectivity::Message* start, com::softwareag::connectivity::Message* end) {}
  virtual void start() { logger.info("CME SBE plugin started.");

    boost::asio::io_service io_service;
    auto listen_address = boost::asio::ip::address::from_string("0.0.0.0");
    auto configs = Config::load("config.xml");
    auto channel = Channel<Handler>::make_channel(handler.get(), "310", configs, io_service, listen_address);
    channel->Subscribe("ESH8");
    io_service.run();
  }
  virtual void shutdown() { logger.info("CME SBE plugin shutting down."); }

private:

  class HandlerImpl: public Handler {
  public:
    HandlerImpl(HostSide::ptr_t host):host(host) {
    }
    void OnQuote(uint64_t securityid,DepthBook const& book);
    void OnTrade(uint64_t securityid, Trade& trade);
    void OnStatus(uint64_t securityid, SecurityStatus status, SecurityEvent event);
    void OnStatistics(uint64_t securityid, SessionStatistics &statistics);

  private:
    void Send(DepthBook const& book);
    HostSide::ptr_t host;
    com::softwareag::connectivity::Message depth;
  };

  std::unique_ptr<Handler> handler;

};

void CMEConnectivityPlugin::HandlerImpl::OnQuote(uint64_t securityid, DepthBook const &book) {
  Send(book);
}

void CMEConnectivityPlugin::HandlerImpl::Send(DepthBook const &book) {
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
    bidp.push_back(data_t(level.price));
    bidv.push_back(data_t(static_cast<int64_t>(level.quantity)));
  }

  payload[data_t("bid_prices")] = data_t(std::move(bidp));
  payload[data_t("bid_volumes")] = data_t(std::move(bidv));
  payload[data_t("ask_prices")] = data_t(std::move(askp));
  payload[data_t("ask_volumes")] = data_t(std::move(askv));

  com::softwareag::connectivity::Message msg(data_t(std::move(payload)));
  msg.putMetadataValue(com::softwareag::connectivity::Message::HOST_MESSAGE_TYPE(), "com.apama.marketdata.Depth");
  host->sendBatchTowardsHost(&msg, (&msg)+1);
}

  void CMEConnectivityPlugin::HandlerImpl::OnTrade(uint64_t securityid, Trade &trade) {

  }

  void CMEConnectivityPlugin::HandlerImpl::OnStatus(uint64_t securityid, SecurityStatus status, SecurityEvent event) {

  }

  void CMEConnectivityPlugin::HandlerImpl::OnStatistics(uint64_t securityid, SessionStatistics &statistics) {

  }


  SAG_DECLARE_CONNECTIVITY_TRANSPORT_CLASS(CMEConnectivityPlugin);

}}} // namespace
