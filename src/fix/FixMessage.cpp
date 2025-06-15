#include "fix/FixMessage.h"

#include <spdlog/spdlog.h>

std::optional<std::string> FixMessage::get(int tag) const {
    for (const auto& it : fields) {
        if (it.tag == tag) {
            return it.value;
        }
    }

    return std::nullopt;
}

void FixMessage::addField(int tag, const std::string& value) {
    for (auto& it : fields) {
        if (it.tag == tag) {
            spdlog::warn("Tag {} already found in message. Updating value", tag);
            it.value = value;
            return;
        }
    }

    fields.push_back({tag, value});
}

bool FixMessage::has(int tag) const {
    return get(tag).has_value();
}

std::string FixMessage::toString() const {
    std::string result;
    for (const auto& field : fields) {
        result += std::to_string(field.tag) + "=" + field.value + '\x01';
    }
    return result;
}

std::string FixMessage::toStringHR() const {
    std::string result;
    for (const auto& field : fields) {
        result += std::to_string(field.tag) + "=" + field.value + '\n';
    }
    return result;
}

// enforces ordering
bool FixMessage::operator==(const FixMessage& other) const {
    if (other.fields.size() != fields.size()) {
        return false;
    }
    
    for (std::size_t i = 0; i < fields.size(); ++i) {
        if (fields[i].tag != other.fields[i].tag
            or fields[i].value != other.fields[i].value) {
            return false;
        }
    }
    return true;
}

std::optional<FixField> FixMessage::getIthElement(size_t i) const {
    if (i >= fields.size()) {
        return std::nullopt;
    }

    return fields[i];
}

bool FixMessage::isValid() const {
    // header check
    
    FixField invalidField;
    invalidField.tag = -1;
    invalidField.value = "";

    if (!(getIthElement(0).value_or(invalidField).tag == 8)) return false;
    if (!(getIthElement(1).value_or(invalidField).tag == 9)) return false;
    if (!(getIthElement(2).value_or(invalidField).tag == 35)) return false;

    // footer check
    if (!getIthElement(fields.size() - 1).value_or(invalidField).tag == 10) {
        return false;
    }

    return true;
}
