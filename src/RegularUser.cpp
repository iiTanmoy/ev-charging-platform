#include "RegularUser.h"
#include <iostream>

RegularUser::RegularUser()
    : User()
{
}

RegularUser::RegularUser(const std::string& id, const std::string& name, const std::string& contact)
    : User(id, name, contact)
{
}

RegularUser::~RegularUser()
{
}

std::string RegularUser::getTier() const
{
    return "Regular";
}

double RegularUser::calculateDiscount(double basePrice) const
{
    return 0.0;
}

void RegularUser::displayInfo() const
{
    std::cout << "=== Regular User ===" << std::endl;
    User::displayInfo();
    std::cout << "Discount: 0%" << std::endl;
}

void RegularUser::saveToFile(std::ofstream& file) const
{
    User::saveToFile(file);
}

void RegularUser::loadFromFile(std::ifstream& file)
{
    User::loadFromFile(file);
}
