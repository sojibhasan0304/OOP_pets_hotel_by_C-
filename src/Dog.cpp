#include "Dog.hpp"

// this is for the class for DOG.
Dog::Dog(std::string name, int age, std::string breed, double weightKg) // as same like cat class to inherits from main class
    : Pet(std::move(name), age, std::move(breed)), weightKg_(weightKg) {} //Calling form parent constructor

Dog::~Dog() = default;      //Destructor

std::string Dog::makeSound() const { return "Woof!"; } // make sound. 

std::string Dog::careSchedule() const {
    return "Walk: 3x/day; Feed: 2x/day; Fresh water always."; // careschedule for dog 
}

double Dog::getWeightKg() const { return weightKg_; } // getfunction for weight
void Dog::setWeightKg(double w) { weightKg_ = w; } // set function for weight

// function for size which is make 

DogSize Dog::sizeCategory() const { 
    if (weightKg_ > 25.0) return DogSize::Large;
    if (weightKg_ > 10.0) return DogSize::Medium;
    return DogSize::Small;
}
