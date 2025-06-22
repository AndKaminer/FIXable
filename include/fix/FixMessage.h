#ifndef INCLUDE_FIX_FIXMESSAGE_H_
#define INCLUDE_FIX_FIXMESSAGE_H_

#include <optional>
#include <string>
#include <vector>

#include "fix/FixField.h"

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

#endif  // INCLUDE_FIX_FIXMESSAGE_H_
