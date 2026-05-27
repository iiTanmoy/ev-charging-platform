#ifndef STATION_H
#define STATION_H

#include <iostream>
#include <string>
#include <fstream>
#include <ctime>

class User;

class Station
{
private:
    std::string stationID;
    std::string location;
    double latitude;
    double longitude;
    int status;           // 0=Available, 1=Occupied, 2=Faulty, 3=Maintenance
    double powerRating;
    int maxSlots;

public:
    Station();
    Station(const std::string& id, const std::string& loc, double lat, double lon, 
            double power, int slots);
    virtual ~Station();

    // Getters
    std::string getStationID() const;
    std::string getLocation() const;
    double getLatitude() const;
    double getLongitude() const;
    int getStatus() const;
    double getPowerRating() const;
    int getMaxSlots() const;

    // Setters
    void setStatus(int s);
    void setLocation(const std::string& loc);
    void setPowerRating(double power);

    bool isAvailable() const;
    double calculateDistance(double lat, double lon) const;

    // Pure virtual functions (polymorphism)
    virtual double calculatePricePerMin(User* user) const = 0;
    virtual std::string getStationType() const = 0;

    // Virtual display
    virtual void displayInfo() const;

    // File I/O
    virtual void saveToFile(std::ofstream& file) const;
    virtual void loadFromFile(std::ifstream& file);
};

#endif // STATION_H
