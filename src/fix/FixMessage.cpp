#include "fix/FixMessage.h"

#include <spdlog/spdlog.h>

std::optional<std::string> FixMessage::get(int tag) const {
    for (auto& it : fields) {
        if (it.tag == tag) {
            return it.value;
        }
    }

    spdlog::error("Could not find field with matching tag");
    return std::nullopt;
}

void FixMessage::addField(int tag, const std::string& value) {
    for (auto& it : fields) {
        if (it.tag == tag) {
            spdlog::warn("Tag already found in message. Updating value");
            it.value = value;
            return;
        }
    }
    FixField field;
    field.tag = tag;
    field.value = value;

    fields.push_back(field);
}
