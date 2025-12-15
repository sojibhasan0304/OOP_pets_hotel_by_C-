#include "User.hpp"

User::User(std::string name, UserRole role)
    : name_(std::move(name)), role_(role) {}

User::~User() = default;
 // get and set 
const std::string& User::getName() const { return name_; }
void User::setName(const std::string& n) { name_ = n; }

UserRole User::getRole() const { return role_; }
void User::setRole(UserRole r) { role_ = r; }
