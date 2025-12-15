#pragma once
#include <string>
#include <chrono>
#include "CareSchedule.hpp"

struct DateRange {
    std::chrono::system_clock::time_point start;
    std::chrono::system_clock::time_point end;
};

class Reservation {
public:
    Reservation(std::string customerName,
                std::string petName,
                DateRange period,
                CareSchedule schedule,
                bool allowCohabitation);

    ~Reservation();

    const std::string& getCustomerName() const;
    void setCustomerName(const std::string& n);

    const std::string& getPetName() const;
    void setPetName(const std::string& n);

    const DateRange& getPeriod() const;
    void setPeriod(const DateRange& p);

    const CareSchedule& getCareSchedule() const;
    void setCareSchedule(const CareSchedule& c);

    bool getAllowCohabitation() const;
    void setAllowCohabitation(bool v);

private:
    std::string customerName_;
    std::string petName_;
    DateRange period_;
    CareSchedule schedule_;
    bool allowCohabitation_{false};
};
