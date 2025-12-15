#pragma once
#include <string>

struct CareSchedule {
    std::string description;
    CareSchedule() = default;
    explicit CareSchedule(std::string d) : description(std::move(d)) {}
};
