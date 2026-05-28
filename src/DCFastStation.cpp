#include "DCFastStation.h"
#include "User.h"
#include <iostream>

DCFastStation::DCFastStation()
    : Station(), basePrice(1.50), maxPower(25.0)
{
}

DCFastStation::DCFastStation(const std::string& id, const std::string& loc, double lat, double lon,
                             double power, int slots)
    : Station(id, loc, lat, lon, power, slots), basePrice(1.50), maxPower(power)
{
}

DCFastStation::~DCFastStation()
{
}

double DCFastStation::calculatePricePerMin(User* user) const
{
    double price = basePrice;
    if (user)
        price -= user->calculateDiscount(basePrice);
    if (price < 0.0)
        price = 0.0;
    return price;
}

std::string DCFastStation::getStationType() const
{
    return "DCFast";
}

void DCFastStation::displayInfo() const
{
    Station::displayInfo();
    std::cout << "Type: DC Fast" << std::endl;
    std::cout << "Base Price per Min: " << basePrice << std::endl;
}

void DCFastStation::saveToFile(std::ofstream& file) const
{
    Station::saveToFile(file);
}

void DCFastStation::loadFromFile(std::ifstream& file)
{
    Station::loadFromFile(file);
}
