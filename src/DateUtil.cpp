#include "DateUtil.hpp"
#include <iomanip>
#include <sstream>
#include <ctime>
#include <stdexcept>
#include <cstdio>

namespace dateutil {

    // Helper function - "to_tm" 

static std::tm to_tm(int Y, int M, int D) {
    std::tm tm{};           // it's a time structure
    tm.tm_year = Y - 1900; // form year 1990
    tm.tm_mon  = M - 1;
    tm.tm_mday = D;
    tm.tm_hour = 0;
    tm.tm_min  = 0;
    tm.tm_sec  = 0;
    return tm;
}

// there is one funciton wiht 'praseYMD" name 

std::chrono::system_clock::time_point parseYMD(const std::string& ymd) {
    int Y = 0, M = 0, D = 0;
    if (std::sscanf(ymd.c_str(), "%d-%d-%d", &Y, &M, &D) != 3) {
        throw std::runtime_error("Invalid date format (use YYYY-MM-DD).");
    }
    auto tm = to_tm(Y, M, D);
    std::time_t tt = std::mktime(&tm);
    if (tt == -1) throw std::runtime_error("Invalid date value."); // this is for 30 ferbruary 
    return std::chrono::system_clock::from_time_t(tt);
}
            // function formatYMD for converting date to the string .

std::string formatYMD(const std::chrono::system_clock::time_point& tp) { 
    std::time_t tt = std::chrono::system_clock::to_time_t(tp);
    std::tm tm = *std::localtime(&tt);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d");
    return oss.str();
}
            // finction daysBetween for Calculates number of days
int daysBetween(const std::chrono::system_clock::time_point& a,
                const std::chrono::system_clock::time_point& b) {
    using namespace std::chrono;
    return (int)duration_cast<hours>(b - a).count() / 24;
}

} // namespace dateutil
