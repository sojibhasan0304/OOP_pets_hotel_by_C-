#pragma once
#include "User.hpp"

class Employee : public User {
public:
    explicit Employee(std::string name)
        : User(std::move(name), UserRole::Employee) {}
    ~Employee() override = default;
};
