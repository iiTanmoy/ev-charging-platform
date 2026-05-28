#include "ChargingSession.h"
#include "Station.h"
#include "User.h"
#include <fstream>
#include <iomanip>

ChargingSession::ChargingSession()
    : Booking(), energyConsumed(0.0), finalCost(0.0)
{
}

ChargingSession::ChargingSession(const std::string& id, Station* s, User* u, int duration)
    : Booking(id, s, u, duration), energyConsumed(0.0), finalCost(0.0)
{
}

ChargingSession::~ChargingSession()
{
}

double ChargingSession::getEnergyConsumed() const
{
    return energyConsumed;
}

double ChargingSession::getFinalCost() const
{
    return finalCost;
}

void ChargingSession::setEnergyConsumed(double energy)
{
    energyConsumed = energy;
}

void ChargingSession::setFinalCost(double cost)
{
    finalCost = cost;
}

double ChargingSession::calculateCost() const
{
    return Booking::calculateCost();
}

void ChargingSession::logSession(const std::string& logFile)
{
    std::ofstream file(logFile.c_str(), std::ios::app);
    if (!file.is_open())
        return;
    file << "SessionID:" << getBookingID() << ", Station:" << getStation()->getStationID()
         << ", User:" << getUser()->getUserID() << ", Cost:" << std::fixed << std::setprecision(2) << finalCost
         << ", Energy:" << energyConsumed << std::endl;
    file.close();
}

void ChargingSession::saveToFile(std::ofstream& file) const
{
    Booking::saveToFile(file);
    file << "," << energyConsumed << "," << finalCost;
}

void ChargingSession::loadFromFile(std::ifstream& file)
{
    Booking::loadFromFile(file);
    char comma;
    file >> comma >> energyConsumed >> comma >> finalCost;
}
