#include "User.h"

User::User()
    : userID(""), name(""), contact(""), walletBalance(0.0), totalSessions(0)
{
}

User::User(const std::string& id, const std::string& n, const std::string& c)
    : userID(id), name(n), contact(c), walletBalance(0.0), totalSessions(0)
{
}

User::~User()
{
}

std::string User::getUserID() const
{
    return userID;
}

std::string User::getName() const
{
    return name;
}

std::string User::getContact() const
{
    return contact;
}

double User::getWalletBalance() const
{
    return walletBalance;
}

int User::getTotalSessions() const
{
    return totalSessions;
}

void User::setContact(const std::string& c)
{
    contact = c;
}

void User::setWalletBalance(double balance)
{
    walletBalance = balance;
}

void User::addSession()
{
    ++totalSessions;
}

void User::deductBalance(double amount)
{
    if (amount > 0)
        walletBalance -= amount;
}

void User::creditBalance(double amount)
{
    if (amount > 0)
        walletBalance += amount;
}

void User::displayInfo() const
{
    std::cout << "User ID: " << userID << std::endl;
    std::cout << "Name: " << name << std::endl;
    std::cout << "Contact: " << contact << std::endl;
    std::cout << "Wallet Balance: " << walletBalance << std::endl;
    std::cout << "Total Sessions: " << totalSessions << std::endl;
}

void User::saveToFile(std::ofstream& file) const
{
    file << userID << "," << name << "," << contact << "," << walletBalance << "," << totalSessions;
}

void User::loadFromFile(std::ifstream& file)
{
    char comma;
    file >> userID >> comma >> name >> comma >> contact >> comma >> walletBalance >> comma >> totalSessions;
}
