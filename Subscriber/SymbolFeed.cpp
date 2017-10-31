#include "SymbolFeed.h"

void SymbolFeed::StartRecovery() {
  if(!recovery_mode) {
    recovery_mode = true;
    book_.clear();
    snapshotA_.join();
    snapshotB_.join();
  }
}

void SymbolFeed::StopRecovery() {
  if(recovery_mode) {
    recovery_mode = false;
    snapshotA_.leave();
    snapshotB_.leave();
  }
}

SymbolFeed::SymbolFeed(Handler *handler, Decoder *decoder):handler_(handler), decoder_(decoder) {
  incrementalA_.join();
  incrementalB_.join();
  StartRecovery();
}
