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

bool FixMessage::has(int tag) const { return get(tag).has_value(); }

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
    if (fields[i].tag != other.fields[i].tag ||
        fields[i].value != other.fields[i].value) {
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

bool FixMessage::hasValidFormatFor(int tag) const {
  switch (tag) {
    default:
      return true;
  }
}

bool FixMessage::isValid(bool checksum) const {
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

  auto beginStr = get(8);
  auto bodyLenStr = get(9);
  auto msgType = get(35);
  auto checksumStr = get(10);

  int bodyLen = 0;
  int checkSum = 0;

  try {
    bodyLen = std::stoi(*bodyLenStr);
    checksum = std::stoi(*checksumStr);
  } catch (...) {
    return false;
  }

  // body length check
  std::string fixStr = toString();

  size_t pos9 = fixStr.find("9=");
  size_t startBody = fixStr.find('\x01', pos9);
  ++startBody;  // byte after first delimiter

  size_t endBody = fixStr.rfind("10=") - 1;
  int actualBodyLen = static_cast<int>(endBody - startBody + 1);
  if (actualBodyLen != bodyLen) {
    spdlog::warn("Body length mismatch: declared={}, actual={}", bodyLen,
                 actualBodyLen);
    return false;
  }

  // checksum check
  if (checksum) {
    size_t checksumRangeEnd = endBody;

    int computedChecksum = 0;
    for (size_t i = 0; i <= checksumRangeEnd; ++i) {
      computedChecksum += static_cast<unsigned char>(fixStr[i]);
    }
    computedChecksum %= 256;

    if (computedChecksum != checksum) {
      spdlog::warn("Checksum mismatch: declared={}, computed={}", checksum,
                   computedChecksum);
      return false;
    }
  }

  // valid format field
  for (auto& it : fields) {
    if (!hasValidFormatFor(it.tag)) {
      return false;
    }
  }
  return true;
}

void FixMessage::removeField(int tag) {
  auto it = std::find_if(fields.begin(), fields.end(),
                         [tag](const FixField& f) { return f.tag == tag; });
  if (it != fields.end()) fields.erase(it);
}
