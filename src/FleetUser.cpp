#include "FleetUser.h"
#include <iostream>

FleetUser::FleetUser()
    : User(), fleetID(""), companyName("")
{
}

FleetUser::FleetUser(const std::string& id, const std::string& name, const std::string& contact,
                     const std::string& fleetId, const std::string& company)
    : User(id, name, contact), fleetID(fleetId), companyName(company)
{
}

FleetUser::~FleetUser()
{
}

std::string FleetUser::getTier() const
{
    return "Fleet";
}

double FleetUser::calculateDiscount(double basePrice) const
{
    return basePrice * 0.25;
}

std::string FleetUser::getFleetID() const
{
    return fleetID;
}

std::string FleetUser::getCompanyName() const
{
    return companyName;
}

void FleetUser::displayInfo() const
{
    std::cout << "\n=== Fleet User ===" << std::endl;
    User::displayInfo();
    std::cout << "Fleet ID: " << fleetID << std::endl;
    std::cout << "Company: " << companyName << std::endl;
    std::cout << "Discount: 25%" << std::endl;
}

void FleetUser::saveToFile(std::ofstream& file) const
{
    file << "Fleet,";
    User::saveToFile(file);
    file << "," << fleetID << "," << companyName;
}

void FleetUser::loadFromFile(std::ifstream& file)
{
    std::string tier;
    char comma;
    file >> tier >> comma;
    User::loadFromFile(file);
    file >> comma >> fleetID >> comma >> companyName;
}
