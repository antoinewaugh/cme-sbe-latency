#pragma once

struct StreamValidator {

  StreamValidator(uint64_t securityid);
  bool RecoveryMode();
  template <class T> bool Validate(T);

private:
  uint64_t securityid_;
  bool recoverymode_ = false;
  int seqnum_ = 0;

  void StartRecovery();
  void StopRecovery();

};
