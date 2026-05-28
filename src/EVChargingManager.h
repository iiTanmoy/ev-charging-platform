#ifndef EVCHARGINGMANAGER_H
#define EVCHARGINGMANAGER_H

#include <string>
#include <map>
#include <queue>
#include <vector>
#include "Station.h"
#include "User.h"
#include "Booking.h"
#include "ChargingSession.h"

class EVChargingManager
{
private:
    std::map<std::string, Station*> stations;
    std::map<std::string, User*> users;
    std::queue<Booking*> activeBookings;
    std::vector<ChargingSession*> completedSessions;

    std::string stationsFile;
    std::string usersFile;
    std::string bookingsFile;
    std::string sessionsLogFile;

    bool writeTempFile(const std::string& path, const std::string& content) const;
    std::vector<std::string> splitCSVLine(const std::string& line) const;
    Station* createStationFromCSV(const std::vector<std::string>& fields) const;
    User* createUserFromCSV(const std::vector<std::string>& fields) const;
    Booking* createBookingFromCSV(const std::vector<std::string>& fields) const;
    void ensureDataFilesExist() const;
    void clearActiveBookings();

public:
    EVChargingManager(const std::string& stationPath = "data/stations.csv",
                      const std::string& userPath = "data/users.csv",
                      const std::string& bookingPath = "data/bookings.csv",
                      const std::string& sessionsPath = "data/sessions.log");
    ~EVChargingManager();

    void loadAllData();
    void saveStationsToFile() const;
    void saveUsersToFile() const;
    void saveBookingsToFile() const;
    void appendSessionLog(const ChargingSession* session) const;

    void addStation(Station* station);
    void addUser(User* user);
    void addBooking(Booking* booking);
    void updateStationStatus(const std::string& id, int status);
    void completeBooking(const std::string& bookingId);

    void listAllStations() const;
    Station* findStationByID(const std::string& id) const;
    void searchStationByID(const std::string& id) const;
    void removeStation(const std::string& id);

    void listAllUsers() const;
    User* findUserByID(const std::string& id) const;
    void searchUserByID(const std::string& id) const;
    void removeUser(const std::string& id);

    Booking* findBookingByID(const std::string& id) const;
    void listBookingsForUser(const std::string& userId) const;
    void cancelBooking(const std::string& bookingId);
    void exportBookingsToCSV(const std::string& filename) const;

    void listActiveBookings() const;
    void displaySystemStatus() const;

    void generateRevenueReport() const;
    void generateUtilizationReport() const;
    void findTopRevenueStations(int topN) const;
    void generateInactiveUsersReport() const;
    void generatePeakHourAnalysis() const;

    void backupSystem(const std::string& filename) const;
    void restoreSystem(const std::string& filename);
    void exportStationsToCSV(const std::string& filename) const;
    void exportUsersToCSV(const std::string& filename) const;
};

#endif // EVCHARGINGMANAGER_H
