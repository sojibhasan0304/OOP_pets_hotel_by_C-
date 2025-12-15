#pragma once
#include <vector>
#include <memory>
#include <string>
#include "Pet.hpp"
#include "Dog.hpp"
#include "Rodent.hpp"

enum class KennelType { LargeDog, MediumDog, SmallDogOrCat, RodentGroup };

class Kennel {
public:
    Kennel(int id, KennelType type, int capacity = 1);
    ~Kennel();

    int getId() const;
    KennelType getType() const;

    int capacity() const;
    int occupancy() const;

    bool canFit(const Pet& pet, bool allowCohabitation) const;
    bool addPet(const PetPtr& pet, bool allowCohabitation);
    bool removePet(const std::string& petName);

    const std::vector<PetPtr>& pets() const;

private:
    int id_{};
    KennelType type_{};
    int capacity_{1};
    std::vector<PetPtr> pets_;
};
