#include "PremiumUser.h"
#include <iostream>

PremiumUser::PremiumUser()
    : User(), freeMinutes(30)
{
}

PremiumUser::PremiumUser(const std::string& id, const std::string& name, const std::string& contact)
    : User(id, name, contact), freeMinutes(30)
{
}

PremiumUser::~PremiumUser()
{
}

std::string PremiumUser::getTier() const
{
    return "Premium";
}

double PremiumUser::calculateDiscount(double basePrice) const
{
    return basePrice * 0.15;
}

int PremiumUser::getFreeMinutes() const
{
    return freeMinutes;
}

void PremiumUser::setFreeMinutes(int minutes)
{
    if (minutes >= 0)
        freeMinutes = minutes;
}

void PremiumUser::displayInfo() const
{
    std::cout << "=== Premium User ===" << std::endl;
    User::displayInfo();
    std::cout << "Discount: 15%" << std::endl;
    std::cout << "Free Minutes: " << freeMinutes << std::endl;
}

void PremiumUser::saveToFile(std::ofstream& file) const
{
    User::saveToFile(file);
    file << "," << freeMinutes;
}

void PremiumUser::loadFromFile(std::ifstream& file)
{
    User::loadFromFile(file);
    char comma;
    file >> comma >> freeMinutes;
}
