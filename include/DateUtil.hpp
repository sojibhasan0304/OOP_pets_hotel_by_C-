#pragma once
#include <chrono>
#include <string>

namespace dateutil {

std::chrono::system_clock::time_point parseYMD(const std::string& ymd);
std::string formatYMD(const std::chrono::system_clock::time_point& tp);
int daysBetween(const std::chrono::system_clock::time_point& a,
                const std::chrono::system_clock::time_point& b);

} // namespace dateutil
