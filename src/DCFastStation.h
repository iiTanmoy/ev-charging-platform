#ifndef DCFASTSTATION_H
#define DCFASTSTATION_H

#include "Station.h"

class DCFastStation : public Station
{
private:
    double basePrice;  // per minute (higher than AC)
    double maxPower;

public:
    DCFastStation();
    DCFastStation(const std::string& id, const std::string& loc, double lat, double lon,
                  double power, int slots);
    ~DCFastStation();

    double calculatePricePerMin(User* user) const;
    std::string getStationType() const;
    void displayInfo() const;

    void saveToFile(std::ofstream& file) const;
    void loadFromFile(std::ifstream& file);
};

#endif // DCFASTSTATION_H
