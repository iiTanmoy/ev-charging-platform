#include "DCUltraFastStation.h"
#include "User.h"
#include <iostream>

DCUltraFastStation::DCUltraFastStation()
    : Station(), basePrice(3.00), ultraPower(60.0)
{
}

DCUltraFastStation::DCUltraFastStation(const std::string& id, const std::string& loc, double lat, double lon,
                                       double power, int slots)
    : Station(id, loc, lat, lon, power, slots), basePrice(3.00), ultraPower(power)
{
}

DCUltraFastStation::~DCUltraFastStation()
{
}

double DCUltraFastStation::calculatePricePerMin(User* user) const
{
    double price = basePrice;
    if (user)
        price -= user->calculateDiscount(basePrice);
    if (price < 0.0)
        price = 0.0;
    return price;
}

std::string DCUltraFastStation::getStationType() const
{
    return "DCUltra";
}

void DCUltraFastStation::displayInfo() const
{
    Station::displayInfo();
    std::cout << "Type: DC Ultra-Fast" << std::endl;
    std::cout << "Base Price per Min: " << basePrice << std::endl;
}

void DCUltraFastStation::saveToFile(std::ofstream& file) const
{
    Station::saveToFile(file);
}

void DCUltraFastStation::loadFromFile(std::ifstream& file)
{
    Station::loadFromFile(file);
}
