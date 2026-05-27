#ifndef DCULTRAFASTSTATION_H
#define DCULTRAFASTSTATION_H

#include "Station.h"

class DCUltraFastStation : public Station
{
private:
    double basePrice;     // per minute (highest)
    double ultraPower;

public:
    DCUltraFastStation();
    DCUltraFastStation(const std::string& id, const std::string& loc, double lat, double lon,
                       double power, int slots);
    ~DCUltraFastStation();

    double calculatePricePerMin(User* user) const;
    std::string getStationType() const;
    void displayInfo() const;

    void saveToFile(std::ofstream& file) const;
    void loadFromFile(std::ifstream& file);
};

#endif // DCULTRAFASTSTATION_H
