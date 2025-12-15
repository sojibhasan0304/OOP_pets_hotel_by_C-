#pragma once
#include "Reservation.hpp"

class Stay {
public:
    Stay(Reservation reservation, int kennelId);
    ~Stay();

    const Reservation& getReservation() const;
    int getKennelId() const;
    void setKennelId(int id);

private:
    Reservation reservation_;
    int kennelId_{-1};
};
