#ifndef CHARGINGSESSION_H
#define CHARGINGSESSION_H

#include "Booking.h"
#include <string>

class ChargingSession : public Booking
{
private:
    double energyConsumed;  // in kWh
    double finalCost;

public:
    ChargingSession();
    ChargingSession(const std::string& id, Station* s, User* u, int duration);
    ~ChargingSession();

    // Getters
    double getEnergyConsumed() const;
    double getFinalCost() const;

    // Setters
    void setEnergyConsumed(double energy);
    void setFinalCost(double cost);

    // Override - polymorphic pricing
    double calculateCost() const;
    void logSession(const std::string& logFile);

    // File I/O
    void saveToFile(std::ofstream& file) const;
    void loadFromFile(std::ifstream& file);
};

#endif // CHARGINGSESSION_H
