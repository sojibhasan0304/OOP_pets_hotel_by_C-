#include "Rodent.hpp" // class 

Rodent::Rodent(std::string name, int age, std::string breed, RodentKind kind)
    : Pet(std::move(name), age, std::move(breed)), kind_(kind) {}

Rodent::~Rodent() = default;

std::string Rodent::makeSound() const { return "(small squeak)"; } // make sound 

std::string Rodent::careSchedule() const { // care schulde 
    return "Feed: daily; Clean cage: every 2–3 days; Fresh water.";
}

// get and set functon 
RodentKind Rodent::getKind() const { return kind_; }
void Rodent::setKind(RodentKind k) { kind_ = k; }
