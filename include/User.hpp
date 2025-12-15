#pragma once
#include <string>

enum class UserRole { Customer, Employee };

class User {
public:
    User(std::string name, UserRole role);
    virtual ~User();

    const std::string& getName() const;
    void setName(const std::string& n);

    UserRole getRole() const;
    void setRole(UserRole r);

private:
    std::string name_;
    UserRole role_;
};
