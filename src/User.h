#ifndef USER_H
#define USER_H

#include <iostream>
#include <string>
#include <fstream>

class User
{
private:
    std::string userID;
    std::string name;
    std::string contact;
    double walletBalance;
    int totalSessions;

public:
    User();
    User(const std::string& id, const std::string& n, const std::string& c);
    virtual ~User();

    // Getters
    std::string getUserID() const;
    std::string getName() const;
    std::string getContact() const;
    double getWalletBalance() const;
    int getTotalSessions() const;

    // Setters
    void setContact(const std::string& c);
    void setWalletBalance(double balance);
    void addSession();
    void deductBalance(double amount);
    void creditBalance(double amount);

    // Pure virtual - each tier implements differently
    virtual std::string getTier() const = 0;
    virtual double calculateDiscount(double basePrice) const = 0;

    // Virtual display
    virtual void displayInfo() const;

    // File I/O
    virtual void saveToFile(std::ofstream& file) const;
    virtual void loadFromFile(std::ifstream& file);
};

#endif // USER_H
