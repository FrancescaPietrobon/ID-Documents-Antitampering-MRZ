#include <spdlog/spdlog.h>

namespace TestHelper
{
    static std::string PrintTo();
}

std::string TestHelper::PrintTo() {
    spdlog::dump_backtrace();  
    return "BACKTRACE";
}