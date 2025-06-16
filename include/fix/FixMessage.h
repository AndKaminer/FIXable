#ifndef FIXMESSAGE_H
#define FIXMESSAGE_H

#include "fix/FixField.h"

#include <vector>
#include <optional>

class FixMessage {
private:
    std::vector<FixField> fields;
    std::optional<FixField> getIthElement(size_t i) const;

public:
    std::optional<std::string> get(int tag) const;
    void addField(int tag, const std::string& value);
    bool has(int tag) const;
    std::string toString() const;
    std::string toStringHR() const;
    bool operator==(const FixMessage& other) const;
    bool isValid(bool checksum = false) const;
    bool hasValidFormatFor(int tag) const;
    void removeField(int tag);
};

#endif
