#include "net/TCPServer.h"
#include "fix/FixMessage.h"

#include <unistd.h>
#include <iostream>

#include <spdlog/spdlog.h>

int main() {
    spdlog::set_level(spdlog::level::trace);

    FixMessage fm = FixMessage();

    fm.addField(1, "hello");
    fm.addField(2, "goodbye");
    spdlog::debug(fm.get(1).value_or(""));
    spdlog::debug(fm.get(2).value_or(""));
    spdlog::debug(fm.get(3).value_or(""));
    fm.addField(2, "goodgood");
    spdlog::debug(fm.get(2).value_or(""));
}
