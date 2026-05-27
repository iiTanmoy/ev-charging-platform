#include "Station.h"
#include "User.h"
#include <cmath>
#include <iostream>
#include <sstream>

Station::Station()
    : stationID(""), location(""), latitude(0.0), longitude(0.0),
      status(0), powerRating(0.0), maxSlots(0)
{
}

Station::Station(const std::string& id, const std::string& loc, double lat, double lon,
                 double power, int slots)
    : stationID(id), location(loc), latitude(lat), longitude(lon),
      status(0), powerRating(power), maxSlots(slots)
{
}

Station::~Station()
{
}

std::string Station::getStationID() const
{
    return stationID;
}

std::string Station::getLocation() const
{
    return location;
}

double Station::getLatitude() const
{
    return latitude;
}

double Station::getLongitude() const
{
    return longitude;
}

int Station::getStatus() const
{
    return status;
}

double Station::getPowerRating() const
{
    return powerRating;
}

int Station::getMaxSlots() const
{
    return maxSlots;
}

void Station::setStatus(int s)
{
    if (s >= 0 && s <= 3)
        status = s;
}

void Station::setLocation(const std::string& loc)
{
    location = loc;
}

void Station::setPowerRating(double power)
{
    if (power > 0)
        powerRating = power;
}

bool Station::isAvailable() const
{
    return status == 0;
}

double Station::calculateDistance(double lat, double lon) const
{
    double dLat = latitude - lat;
    double dLon = longitude - lon;
    return std::sqrt(dLat * dLat + dLon * dLon);
}

void Station::displayInfo() const
{
    std::cout << "Station ID: " << stationID << std::endl;
    std::cout << "Location: " << location << std::endl;
    std::cout << "Coordinates: (" << latitude << ", " << longitude << ")" << std::endl;
    std::cout << "Power Rating: " << powerRating << " kW" << std::endl;
    std::string statusStr = "";
    switch (status) {
        case 0: statusStr = "Available"; break;
        case 1: statusStr = "Occupied"; break;
        case 2: statusStr = "Faulty"; break;
        case 3: statusStr = "Maintenance"; break;
    }
    std::cout << "Status: " << statusStr << std::endl;
}

void Station::saveToFile(std::ofstream& file) const
{
    file << stationID << "," << location << "," << latitude << ","
         << longitude << "," << powerRating << "," << status << "," << maxSlots;
}

void Station::loadFromFile(std::ifstream& file)
{
    char comma;
    file >> stationID >> comma >> location >> comma >> latitude >> comma
         >> longitude >> comma >> powerRating >> comma >> status >> comma >> maxSlots;
}
