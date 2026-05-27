#ifndef FLEETUSER_H
#define FLEETUSER_H

#include "User.h"
#include <string>

class FleetUser : public User
{
private:
    std::string fleetID;
    std::string companyName;

public:
    FleetUser();
    FleetUser(const std::string& id, const std::string& name, const std::string& contact,
              const std::string& fleetId, const std::string& company);
    ~FleetUser();

    std::string getTier() const;
    double calculateDiscount(double basePrice) const;  // 25% discount
    std::string getFleetID() const;
    std::string getCompanyName() const;
    void displayInfo() const;

    void saveToFile(std::ofstream& file) const;
    void loadFromFile(std::ifstream& file);
};

#endif // FLEETUSER_H
