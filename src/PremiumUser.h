#ifndef PREMIUMUSER_H
#define PREMIUMUSER_H

#include "User.h"

class PremiumUser : public User
{
private:
    int freeMinutes;  // Monthly free charging minutes

public:
    PremiumUser();
    PremiumUser(const std::string& id, const std::string& name, const std::string& contact);
    ~PremiumUser();

    std::string getTier() const;
    double calculateDiscount(double basePrice) const;  // 15% discount
    int getFreeMinutes() const;
    void setFreeMinutes(int minutes);
    void displayInfo() const;

    void saveToFile(std::ofstream& file) const;
    void loadFromFile(std::ifstream& file);
};

#endif // PREMIUMUSER_H
