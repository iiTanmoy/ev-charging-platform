#include "ACStation.h"
#include "User.h"
#include <iostream>

ACStation::ACStation()
    : Station(), basePrice(0.50)
{
}

ACStation::ACStation(const std::string& id, const std::string& loc, double lat, double lon,
                     double power, int slots)
    : Station(id, loc, lat, lon, power, slots), basePrice(0.50)
{
}

ACStation::~ACStation()
{
}

double ACStation::calculatePricePerMin(User* user) const
{
    double price = basePrice;
    if (user)
        price -= user->calculateDiscount(basePrice);
    if (price < 0.0)
        price = 0.0;
    return price;
}

std::string ACStation::getStationType() const
{
    return "AC";
}

void ACStation::displayInfo() const
{
    Station::displayInfo();
    std::cout << "Type: AC Standard" << std::endl;
    std::cout << "Base Price per Min: " << basePrice << std::endl;
}

void ACStation::saveToFile(std::ofstream& file) const
{
    Station::saveToFile(file);
}

void ACStation::loadFromFile(std::ifstream& file)
{
    Station::loadFromFile(file);
}
