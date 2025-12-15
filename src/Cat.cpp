#include "Cat.hpp" //header file

Cat::Cat(std::string name, int age, std::string breed) // class for cat
    : Pet(std::move(name), age, std::move(breed)) {} // inheritance from main class which is pet. and I have used "{}" for end the statment.

Cat::~Cat() = default;      // Destructor

std::string Cat::makeSound() const { return "Meow."; } // this is a function for make sound.

// careshculde fanction

std::string Cat::careSchedule() const {
    return "Feed: 2–3x/day; Litter: daily check; Fresh water.";
}
