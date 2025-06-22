#ifndef INCLUDE_FIX_FIXENCODER_H_
#define INCLUDE_FIX_FIXENCODER_H_

#include <sstream>
#include <iomanip>
#include <string>

#include "fix/FixMessage.h"

class FixEncoder {
 public:
    static std::string encode(const FixMessage& msg,
            const std::string& beginString, const char delimiter);
};

#endif  // INCLUDE_FIX_FIXENCODER_H_
