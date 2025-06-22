#ifndef INCLUDE_FIX_FIXFIELD_H_
#define INCLUDE_FIX_FIXFIELD_H_

#include <string>

struct FixField {
    int tag;
    std::string value;
};

#endif  // INCLUDE_FIX_FIXFIELD_H_
