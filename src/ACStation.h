#ifndef ACSTATION_H
#define ACSTATION_H

#include "Station.h"

class ACStation : public Station
{
private:
    // AC specific: lower power, basic charging
    double basePrice;  // per minute

public:
    ACStation();
    ACStation(const std::string& id, const std::string& loc, double lat, double lon,
              double power, int slots);
    ~ACStation();

    double calculatePricePerMin(User* user) const;
    std::string getStationType() const;
    void displayInfo() const;

    void saveToFile(std::ofstream& file) const;
    void loadFromFile(std::ifstream& file);
};

#endif // ACSTATION_H
