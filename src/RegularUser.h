#ifndef REGULARUSER_H
#define REGULARUSER_H

#include "User.h"

class RegularUser : public User
{
public:
    RegularUser();
    RegularUser(const std::string& id, const std::string& name, const std::string& contact);
    ~RegularUser();

    std::string getTier() const;
    double calculateDiscount(double basePrice) const;  // No discount
    void displayInfo() const;

    void saveToFile(std::ofstream& file) const;
    void loadFromFile(std::ifstream& file);
};

#endif // REGULARUSER_H
