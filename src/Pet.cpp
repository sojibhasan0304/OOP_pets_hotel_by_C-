#include "Pet.hpp" // this the main class

Pet::Pet(std::string name, int age, std::string breed) // 
    : name_(std::move(name)), age_(age), breed_(std::move(breed)) {}

Pet::~Pet() = default;

const std::string& Pet::getName() const { return name_; }
void Pet::setName(const std::string& n) { name_ = n; }

// getter and setter function 

int Pet::getAge() const { return age_; }
void Pet::setAge(int a) { age_ = a; }

const std::string& Pet::getBreed() const { return breed_; }
void Pet::setBreed(const std::string& b) { breed_ = b; }
