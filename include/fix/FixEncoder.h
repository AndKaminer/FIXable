#ifndef FIXENCODER_H
#define FIXENCODER_H

#include <sstream>
#include <iomanip>
#include <string>

#include "fix/FixMessage.h"

class FixEncoder {
public:
    static std::string encode(const FixMessage& msg, const std::string& beginString, const char delimiter);
};

#endif
