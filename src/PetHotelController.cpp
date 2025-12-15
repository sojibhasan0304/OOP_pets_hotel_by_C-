#include "PetHotelController.hpp"
#include <algorithm>
#include "Dog.hpp"
#include "Cat.hpp"
#include "Rodent.hpp"
#include "DateUtil.hpp"

// Default constructor:
PetHotelController::PetHotelController() = default;
// Default destructor:
PetHotelController::~PetHotelController() = default;

// to add a new kennel in a kennel vector.

void PetHotelController::addKennel(const Kennel& k) {
    kennels_.push_back(k);
}

// this one for removing a kennel by ID

bool PetHotelController::removeKennel(int kennelId) {
    auto it = std::remove_if(kennels_.begin(), kennels_.end(),
        [&](const Kennel& k) { return k.getId() == kennelId; }); // // for ID matching 
    if (it == kennels_.end()) return false;
    kennels_.erase(it, kennels_.end());
    return true;
}

// for access the kennel list.

const std::vector<Kennel>& PetHotelController::kennels() const { return kennels_; }
std::vector<Kennel>& PetHotelController::kennels() { return kennels_; }

bool PetHotelController::registerPet(const PetPtr& pet) {
    return pets_.emplace(pet->getName(), pet).second;
}

// this will add the pet in pet list

bool PetHotelController::unregisterPet(const std::string& petName) {
    stays_.erase(petName);
    reservations_.erase(petName);
    return pets_.erase(petName) > 0;
}

PetPtr PetHotelController::getPet(const std::string& petName) const {
    auto it = pets_.find(petName);
    return it == pets_.end() ? nullptr : it->second;
}

int PetHotelController::availableKennelsFor(const Pet& pet, bool allowCohabitation) const {
    int count = 0;
    for (const auto& k : kennels_)
        if (k.canFit(pet, allowCohabitation)) ++count;
    return count;
}

int PetHotelController::previewAvailability(PetType t, double w, bool allow) const {
    PetPtr temp;
    if (t == PetType::Dog)      temp = std::make_shared<Dog>("_tmp", 0, "_", w);
    else if (t == PetType::Cat) temp = std::make_shared<Cat>("_tmp", 0, "_");
    else                        temp = std::make_shared<Rodent>("_tmp", 0, "_", RodentKind::Rabbit);
    return availableKennelsFor(*temp, allow);
}
// this is for reservation 
bool PetHotelController::createReservation(const Reservation& r) {
    const int d = dateutil::daysBetween(r.getPeriod().start, r.getPeriod().end);
    if (d < 2 || d > 28) return false;
    reservations_.insert_or_assign(r.getPetName(), r);
    return true;
}

// this is for change reservation 
bool PetHotelController::editReservation(const std::string& petName, const Reservation& updated) {
    auto it = reservations_.find(petName);
    if (it == reservations_.end()) return false;
    const int d = dateutil::daysBetween(updated.getPeriod().start, updated.getPeriod().end);
    if (d < 2 || d > 28) return false;
    it->second = updated;
    return true;
}

bool PetHotelController::cancelReservation(const std::string& petName) {
    return reservations_.erase(petName) > 0;
}

// checkin 

bool PetHotelController::checkIn(const std::string& petName, int kennelId, bool allowCohabitation) {
    auto pet = getPet(petName);
    if (!pet) return false;

    Kennel* target = nullptr;
    for (auto& k : kennels_) {
        if (k.getId() == kennelId) { target = &k; break; }
    }
    if (!target || !target->addPet(pet, allowCohabitation)) return false;

    auto it = reservations_.find(petName);
    if (it == reservations_.end()) return false;

    stays_.emplace(petName, Stay{it->second, kennelId});
    return true;
}

// moving pets 

bool PetHotelController::movePet(const std::string& petName, int toKennelId, bool allowCohabitation) {
    auto itStay = stays_.find(petName);
    if (itStay == stays_.end()) return false;

    Kennel* from = nullptr;
    Kennel* to   = nullptr;
    for (auto& k : kennels_) {
        if (k.getId() == itStay->second.getKennelId()) from = &k;
        if (k.getId() == toKennelId)                   to   = &k;
    }

    auto pet = getPet(petName);
    if (!from || !to || !pet) return false;
    if (!to->canFit(*pet, allowCohabitation)) return false;

    if (!to->addPet(pet, allowCohabitation)) return false;
    from->removePet(petName);
    itStay->second.setKennelId(toKennelId);
    return true;
}
// for cheak out 
bool PetHotelController::checkOut(const std::string& petName) {
    auto itStay = stays_.find(petName);
    if (itStay == stays_.end()) return false;

    int kennelId = itStay->second.getKennelId();
    for (auto& k : kennels_) {
        if (k.getId() == kennelId) {
            k.removePet(petName);
            break;
        }
    }
    stays_.erase(itStay);
    return true;
}

std::vector<PetPtr> PetHotelController::listPetsByType(PetType t) const {
    std::vector<PetPtr> out;
    for (const auto& kv : pets_)
        if (kv.second->type() == t) out.push_back(kv.second);
    return out;
}

std::vector<PetPtr> PetHotelController::listDogsBySize(DogSize s) const {
    std::vector<PetPtr> out;
    for (const auto& kv : pets_) {
        if (kv.second->type() == PetType::Dog &&
            static_cast<Dog&>(*kv.second).sizeCategory() == s)
            out.push_back(kv.second);
    }
    return out;
}

std::vector<PetPtr> PetHotelController::listByCareScheduleContains(const std::string& substring) const {
    std::vector<PetPtr> out;
    for (const auto& kv : pets_) {
        if (kv.second->careSchedule().find(substring) != std::string::npos)
            out.push_back(kv.second);
    }
    return out;
}

std::vector<std::pair<int, std::vector<std::string>>> PetHotelController::listKennelsWithPets() const {
    std::vector<std::pair<int, std::vector<std::string>>> result;
    for (const auto& k : kennels_) {
        std::vector<std::string> names;
        for (const auto& p : k.pets()) names.push_back(p->getName());
        result.emplace_back(k.getId(), std::move(names));
    }
    return result;
}

const std::unordered_map<std::string, Reservation>& PetHotelController::reservationsRO() const {
    return reservations_;
}

const std::unordered_map<std::string, Stay>& PetHotelController::staysRO() const {
    return stays_;
}
