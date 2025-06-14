#ifndef FIXMESSAGE_H
#define FIXMESSAGE_H

#include "fix/FixField.h"

#include <vector>
#include <optional>

class FixMessage {
public:
    std::vector<FixField> fields;

    std::optional<std::string> get(int tag) const;
    void addField(int tag, const std::string& value);
};

#endif
