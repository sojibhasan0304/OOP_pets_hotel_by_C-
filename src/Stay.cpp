#include "Stay.hpp" // class 

Stay::Stay(Reservation reservation, int kennelId)
    : reservation_(std::move(reservation)), kennelId_(kennelId) {}

Stay::~Stay() = default;

const Reservation& Stay::getReservation() const { return reservation_; }
int Stay::getKennelId() const { return kennelId_; } // get 
void Stay::setKennelId(int id) { kennelId_ = id; }// set 
