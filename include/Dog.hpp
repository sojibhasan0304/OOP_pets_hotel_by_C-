#pragma once
#include "Pet.hpp"

enum class DogSize { Small, Medium, Large };

class Dog : public Pet {
public:
    Dog(std::string name, int age, std::string breed, double weightKg);
    ~Dog() override;

    PetType type() const override { return PetType::Dog; }
    std::string makeSound() const override;
    std::string careSchedule() const override;

    double getWeightKg() const;
    void setWeightKg(double w);
    DogSize sizeCategory() const;

private:
    double weightKg_{};
};
