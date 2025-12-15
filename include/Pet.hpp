#pragma once
#include <string>
#include <memory>

enum class PetType { Dog, Cat, Rodent };

class Pet {
public:
    Pet(std::string name, int age, std::string breed);
    virtual ~Pet();

    virtual PetType type() const = 0;
    virtual std::string makeSound() const = 0;
    virtual std::string careSchedule() const = 0;

    const std::string& getName() const;
    void setName(const std::string& n);

    int getAge() const;
    void setAge(int a);

    const std::string& getBreed() const;
    void setBreed(const std::string& b);

protected:
    std::string name_;
    int age_{};
    std::string breed_;
};

using PetPtr = std::shared_ptr<Pet>;
