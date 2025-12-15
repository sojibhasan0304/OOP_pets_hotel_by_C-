#include "Kennel.hpp"

// class kennnel

Kennel::Kennel(int id, KennelType type, int capacity) //CONSTRUCTOR
    : id_(id), type_(type), capacity_(capacity) {} // DESTRUCTOR

Kennel::~Kennel() = default;

// getter function 

int Kennel::getId() const { return id_; }
KennelType Kennel::getType() const { return type_; }
int Kennel::capacity() const { return capacity_; }
int Kennel::occupancy() const { return static_cast<int>(pets_.size()); }
const std::vector<PetPtr>& Kennel::pets() const { return pets_; }

// this is most imporatnt function that will manage where will br the pets.

bool Kennel::canFit(const Pet& pet, bool allowCohabitation) const {
    switch (type_) {
        case KennelType::LargeDog:
            if (pet.type() == PetType::Dog &&
                static_cast<const Dog&>(pet).sizeCategory() == DogSize::Large) // case1 
                return occupancy() == 0;
            return false;
        case KennelType::MediumDog:
            if (pet.type() == PetType::Dog &&
                static_cast<const Dog&>(pet).sizeCategory() == DogSize::Medium) // case2 
                return allowCohabitation ? occupancy() < capacity_ : occupancy() == 0;
            return false;
        case KennelType::SmallDogOrCat:
            if (pet.type() == PetType::Dog &&
                static_cast<const Dog&>(pet).sizeCategory() == DogSize::Small) // case3
                return allowCohabitation ? occupancy() < capacity_ : occupancy() == 0;
            if (pet.type() == PetType::Cat)         // cat will be with small dog. 
                return allowCohabitation ? occupancy() < capacity_ : occupancy() == 0;
            return false;
        case KennelType::RodentGroup: // this case for rodent. 
            if (pet.type() == PetType::Rodent) {
                if (occupancy() == 0) return occupancy() < capacity_;
                auto* existing = dynamic_cast<Rodent*>(pets_.front().get());
                auto const* incoming = dynamic_cast<const Rodent*>(&pet);
                if (!existing || !incoming) return false;
                if (existing->getKind() != incoming->getKind()) return false;
                return occupancy() < capacity_;
            }
            return false;
    }
    return false;
}

// this function for adding pets 

bool Kennel::addPet(const PetPtr& pet, bool allowCohabitation) {
    if (!canFit(*pet, allowCohabitation)) return false;
    pets_.push_back(pet); // I have done pet pointer in  a vector. 
    return true;
}

// this function to remove the pets

bool Kennel::removePet(const std::string& petName) {
    for (auto it = pets_.begin(); it != pets_.end(); ++it) { // thi is a lopp for find pets in kennel.
        if ((*it)->getName() == petName) { // if it's matched 
            pets_.erase(it); // remove form the vecotr. 
            return true;
        }
    }
    return false;
}
