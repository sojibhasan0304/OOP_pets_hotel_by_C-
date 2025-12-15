#pragma once
#include "Pet.hpp"

class Cat : public Pet {
public:
    Cat(std::string name, int age, std::string breed);
    ~Cat() override;

    PetType type() const override { return PetType::Cat; }
    std::string makeSound() const override;
    std::string careSchedule() const override;
};
