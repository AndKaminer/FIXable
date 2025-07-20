#ifndef INCLUDE_FIX_FIXENCODER_H_
#define INCLUDE_FIX_FIXENCODER_H_

#include <iomanip>
#include <sstream>
#include <string>

#include "fix/FixMessage.h"

class FixEncoder {
 public:
  static std::string encode(const FixMessage& msg,
                            const std::string& beginString);
 private:
  static const char delimiter = '\x01';
};

#endif  // INCLUDE_FIX_FIXENCODER_H_
