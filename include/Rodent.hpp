#pragma once
#include "Pet.hpp"

enum class RodentKind { Rabbit, GuineaPig, Mouse };

class Rodent : public Pet {
public:
    Rodent(std::string name, int age, std::string breed, RodentKind kind);
    ~Rodent() override;

    PetType type() const override { return PetType::Rodent; }
    std::string makeSound() const override;
    std::string careSchedule() const override;

    RodentKind getKind() const;
    void setKind(RodentKind k);

private:
    RodentKind kind_{RodentKind::Rabbit};
};
