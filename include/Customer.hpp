#pragma once
#include "User.hpp"

class Customer : public User {
public:
    explicit Customer(std::string name)
        : User(std::move(name), UserRole::Customer) {}
    ~Customer() override = default;
};
