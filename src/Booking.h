#ifndef BOOKING_H
#define BOOKING_H

#include <iostream>
#include <string>
#include <fstream>
#include <ctime>

class Station;
class User;

class Booking
{
private:
    std::string bookingID;
    Station* station;
    User* user;
    time_t startTime;
    time_t endTime;
    int slotDuration;  // in minutes
    int status;        // 0=Booked, 1=Active, 2=Completed, 3=Cancelled

public:
    Booking();
    Booking(const std::string& id, Station* s, User* u, int duration);
    virtual ~Booking();

    // Getters
    std::string getBookingID() const;
    Station* getStation() const;
    User* getUser() const;
    time_t getStartTime() const;
    time_t getEndTime() const;
    int getSlotDuration() const;
    int getStatus() const;

    // Setters
    void setStatus(int s);
    void setStartTime(time_t t);
    void setEndTime(time_t t);

    bool isActive() const;
    void startSession();
    void endSession();
    virtual double calculateCost() const;

    // File I/O
    virtual void saveToFile(std::ofstream& file) const;
    virtual void loadFromFile(std::ifstream& file);
};

#endif // BOOKING_H
