#include "Booking.h"
#include "Station.h"
#include "User.h"

Booking::Booking()
    : bookingID(""), station(0), user(0), startTime(0), endTime(0), slotDuration(0), status(0)
{
}

Booking::Booking(const std::string& id, Station* s, User* u, int duration)
    : bookingID(id), station(s), user(u), startTime(0), endTime(0), slotDuration(duration), status(0)
{
}

Booking::~Booking()
{
}

std::string Booking::getBookingID() const
{
    return bookingID;
}

Station* Booking::getStation() const
{
    return station;
}

User* Booking::getUser() const
{
    return user;
}

time_t Booking::getStartTime() const
{
    return startTime;
}

time_t Booking::getEndTime() const
{
    return endTime;
}

int Booking::getSlotDuration() const
{
    return slotDuration;
}

int Booking::getStatus() const
{
    return status;
}

void Booking::setStatus(int s)
{
    if (s >= 0 && s <= 3)
        status = s;
}

void Booking::setStartTime(time_t t)
{
    startTime = t;
}

void Booking::setEndTime(time_t t)
{
    endTime = t;
}

bool Booking::isActive() const
{
    return status == 1;
}

void Booking::startSession()
{
    status = 1;
    startTime = std::time(0);
}

void Booking::endSession()
{
    status = 2;
    endTime = std::time(0);
}

double Booking::calculateCost() const
{
    if (!station || !user)
        return 0.0;
    return station->calculatePricePerMin(user) * slotDuration;
}

void Booking::saveToFile(std::ofstream& file) const
{
    file << bookingID << "," << station->getStationID() << "," << user->getUserID() << ","
         << startTime << "," << endTime << "," << slotDuration << "," << status;
}

void Booking::loadFromFile(std::ifstream& file)
{
    char comma;
    file >> bookingID >> comma >> std::ws;
    std::getline(file, bookingID, ',');
    std::getline(file, bookingID, ',');
    file >> startTime >> comma >> endTime >> comma >> slotDuration >> comma >> status;
}
