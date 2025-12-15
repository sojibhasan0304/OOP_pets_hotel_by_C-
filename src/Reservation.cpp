#include "Reservation.hpp"  // Constructor

Reservation::Reservation(std::string customerName,
                         std::string petName,
                         DateRange period,
                         CareSchedule schedule,
                         bool allowCohabitation)
    : customerName_(std::move(customerName)),
      petName_(std::move(petName)),
      period_(period),
      schedule_(std::move(schedule)),
      allowCohabitation_(allowCohabitation) {}

Reservation::~Reservation() = default;      //Destructor

// get and setcustmer name 

const std::string& Reservation::getCustomerName() const { return customerName_; }
void Reservation::setCustomerName(const std::string& n) { customerName_ = n; }

// get and set pet name 
const std::string& Reservation::getPetName() const { return petName_; }
void Reservation::setPetName(const std::string& n) { petName_ = n; }

// date
const DateRange& Reservation::getPeriod() const { return period_; }
void Reservation::setPeriod(const DateRange& p) { period_ = p; }
// careschedule 
const CareSchedule& Reservation::getCareSchedule() const { return schedule_; }
void Reservation::setCareSchedule(const CareSchedule& c) { schedule_ = c; }
// reservation 
bool Reservation::getAllowCohabitation() const { return allowCohabitation_; }
void Reservation::setAllowCohabitation(bool v) { allowCohabitation_ = v; }
