#pragma once
#include <vector>
#include <unordered_map>
#include <utility>
#include "Kennel.hpp"
#include "Reservation.hpp"
#include "Stay.hpp"
#include "Pet.hpp"

class PetHotelController {
public:
    PetHotelController();
    ~PetHotelController();

    void addKennel(const Kennel& k);
    bool removeKennel(int kennelId);
    const std::vector<Kennel>& kennels() const;
    std::vector<Kennel>& kennels();

    bool registerPet(const PetPtr& pet);
    bool unregisterPet(const std::string& petName);
    PetPtr getPet(const std::string& petName) const;

    int availableKennelsFor(const Pet& pet, bool allowCohabitation) const;
    int previewAvailability(PetType type, double dogWeightKg, bool allowCohabitation) const;

    bool createReservation(const Reservation& r);
    bool editReservation(const std::string& petName, const Reservation& updated);
    bool cancelReservation(const std::string& petName);

    bool checkIn(const std::string& petName, int kennelId, bool allowCohabitation);
    bool movePet(const std::string& petName, int toKennelId, bool allowCohabitation);
    bool checkOut(const std::string& petName);

    std::vector<PetPtr> listPetsByType(PetType t) const;
    std::vector<PetPtr> listDogsBySize(DogSize s) const;
    std::vector<PetPtr> listByCareScheduleContains(const std::string& substring) const;
    std::vector<std::pair<int, std::vector<std::string>>> listKennelsWithPets() const;

    const std::unordered_map<std::string, Reservation>& reservationsRO() const;
    const std::unordered_map<std::string, Stay>& staysRO() const;

private:
    std::vector<Kennel> kennels_;
    std::unordered_map<std::string, PetPtr> pets_;
    std::unordered_map<std::string, Reservation> reservations_;
    std::unordered_map<std::string, Stay> stays_;
};
