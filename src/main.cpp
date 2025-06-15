#include "fix/FixParser.h"

#include <unistd.h>
#include <iostream>

#include <spdlog/spdlog.h>

int main() {
    spdlog::set_level(spdlog::level::trace);

    FixParser fp = FixParser('|');
    spdlog::debug(fp.parseMessage("35=A|8=22|9=15|10=abc").value_or(FixMessage()).toStringHR());
}
