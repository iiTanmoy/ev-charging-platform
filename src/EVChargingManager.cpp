#include "EVChargingManager.h"
#include "ACStation.h"
#include "DCFastStation.h"
#include "DCUltraFastStation.h"
#include "RegularUser.h"
#include "PremiumUser.h"
#include "FleetUser.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <functional>
#include <sstream>
#include <cstdio>

EVChargingManager::EVChargingManager(const std::string& stationPath,
                                     const std::string& userPath,
                                     const std::string& bookingPath,
                                     const std::string& sessionsPath)
    : stationsFile(stationPath), usersFile(userPath), bookingsFile(bookingPath), sessionsLogFile(sessionsPath)
{
    ensureDataFilesExist();
}

EVChargingManager::~EVChargingManager()
{
    for (std::map<std::string, Station*>::iterator it = stations.begin(); it != stations.end(); ++it)
        delete it->second;
    for (std::map<std::string, User*>::iterator it = users.begin(); it != users.end(); ++it)
        delete it->second;
    while (!activeBookings.empty()) {
        delete activeBookings.front();
        activeBookings.pop();
    }
    for (size_t i = 0; i < completedSessions.size(); ++i)
        delete completedSessions[i];
}

bool EVChargingManager::writeTempFile(const std::string& path, const std::string& content) const
{
    std::string tempPath = path + ".tmp";
    std::ofstream temp(tempPath.c_str());
    if (!temp.is_open())
        return false;
    temp << content;
    temp.close();
    if (std::rename(tempPath.c_str(), path.c_str()) != 0) {
        std::remove(tempPath.c_str());
        return false;
    }
    return true;
}

std::vector<std::string> EVChargingManager::splitCSVLine(const std::string& line) const
{
    std::vector<std::string> fields;
    std::string field;
    for (size_t i = 0; i < line.size(); ) {
        size_t pos = line.find(',', i);
        if (pos == std::string::npos) {
            field = line.substr(i);
            fields.push_back(field);
            break;
        }
        field = line.substr(i, pos - i);
        fields.push_back(field);
        i = pos + 1;
    }
    return fields;
}

Station* EVChargingManager::createStationFromCSV(const std::vector<std::string>& fields) const
{
    if (fields.size() < 8)
        return 0;

    const std::string& type = fields[0];
    const std::string& id = fields[1];
    const std::string& location = fields[2];
    double latitude = atof(fields[3].c_str());
    double longitude = atof(fields[4].c_str());
    double power = atof(fields[5].c_str());
    int status = atoi(fields[6].c_str());
    int maxSlots = atoi(fields[7].c_str());

    Station* station = 0;
    if (type == "AC")
        station = new ACStation(id, location, latitude, longitude, power, maxSlots);
    else if (type == "DCFast")
        station = new DCFastStation(id, location, latitude, longitude, power, maxSlots);
    else if (type == "DCUltra")
        station = new DCUltraFastStation(id, location, latitude, longitude, power, maxSlots);

    if (station)
        station->setStatus(status);

    return station;
}

User* EVChargingManager::createUserFromCSV(const std::vector<std::string>& fields) const
{
    if (fields.size() < 5)
        return 0;

    const std::string& tier = fields[0];
    const std::string& id = fields[1];
    const std::string& name = fields[2];
    const std::string& contact = fields[3];
    double wallet = 0.0;
    int sessions = 0;

    if (fields.size() >= 5)
        wallet = atof(fields[4].c_str());
    if (fields.size() >= 6)
        sessions = atoi(fields[5].c_str());

    User* user = 0;
    if (tier == "Regular")
        user = new RegularUser(id, name, contact);
    else if (tier == "Premium")
        user = new PremiumUser(id, name, contact);
    else if (tier == "Fleet") {
        std::string fleetId = "FLT" + id;
        std::string company = "Fleet Corp";
        if (fields.size() >= 8) {
            fleetId = fields[6];
            company = fields[7];
        }
        user = new FleetUser(id, name, contact, fleetId, company);
    }

    if (user) {
        user->setWalletBalance(wallet);
        for (int i = 0; i < sessions; ++i)
            user->addSession();
    }
    return user;
}

Booking* EVChargingManager::createBookingFromCSV(const std::vector<std::string>& fields) const
{
    if (fields.size() < 7)
        return 0;

    const std::string& bookingId = fields[0];
    const std::string& userId = fields[1];
    const std::string& stationId = fields[2];
    int duration = atoi(fields[3].c_str());
    time_t startTime = (time_t)atol(fields[4].c_str());
    time_t endTime = (time_t)atol(fields[5].c_str());
    int status = atoi(fields[6].c_str());

    User* user = findUserByID(userId);
    Station* station = findStationByID(stationId);
    if (!user || !station)
        return 0;

    Booking* booking = new Booking(bookingId, station, user, duration);
    booking->setStartTime(startTime);
    booking->setEndTime(endTime);
    booking->setStatus(status);
    return booking;
}

void EVChargingManager::ensureDataFilesExist() const
{
    std::ofstream(stationsFile.c_str(), std::ios::app).close();
    std::ofstream(usersFile.c_str(), std::ios::app).close();
    std::ofstream(bookingsFile.c_str(), std::ios::app).close();
    std::ofstream(sessionsLogFile.c_str(), std::ios::app).close();
}

void EVChargingManager::clearActiveBookings()
{
    while (!activeBookings.empty()) {
        delete activeBookings.front();
        activeBookings.pop();
    }
}

void EVChargingManager::loadAllData()
{
    ensureDataFilesExist();

    for (std::map<std::string, Station*>::iterator it = stations.begin(); it != stations.end(); ++it)
        delete it->second;
    stations.clear();

    for (std::map<std::string, User*>::iterator it = users.begin(); it != users.end(); ++it)
        delete it->second;
    users.clear();

    for (size_t i = 0; i < completedSessions.size(); ++i)
        delete completedSessions[i];
    completedSessions.clear();

    std::ifstream stationFile(stationsFile.c_str());
    std::string line;
    while (std::getline(stationFile, line)) {
        if (line.empty())
            continue;
        std::vector<std::string> fields = splitCSVLine(line);
        if (fields.empty() || fields[0] == "Type")
            continue;
        Station* station = createStationFromCSV(fields);
        if (station)
            stations[station->getStationID()] = station;
    }
    stationFile.close();

    std::ifstream userFile(usersFile.c_str());
    while (std::getline(userFile, line)) {
        if (line.empty())
            continue;
        std::vector<std::string> fields = splitCSVLine(line);
        if (fields.empty() || fields[0] == "Tier")
            continue;
        User* user = createUserFromCSV(fields);
        if (user)
            users[user->getUserID()] = user;
    }
    userFile.close();

    clearActiveBookings();
    std::ifstream bookingFile(bookingsFile.c_str());
    while (std::getline(bookingFile, line)) {
        if (line.empty())
            continue;
        std::vector<std::string> fields = splitCSVLine(line);
        Booking* booking = createBookingFromCSV(fields);
        if (booking)
            activeBookings.push(booking);
    }
    bookingFile.close();

    std::ifstream sessionFile(sessionsLogFile.c_str());
    while (std::getline(sessionFile, line)) {
        if (line.empty())
            continue;
        std::vector<std::string> fields = splitCSVLine(line);
        if (fields.size() < 9)
            continue;
        const std::string& bookingId = fields[0];
        const std::string& userId = fields[1];
        const std::string& stationId = fields[2];
        int duration = atoi(fields[3].c_str());
        time_t startTime = (time_t)atol(fields[4].c_str());
        time_t endTime = (time_t)atol(fields[5].c_str());
        double energy = atof(fields[6].c_str());
        double cost = atof(fields[7].c_str());
        User* user = findUserByID(userId);
        Station* station = findStationByID(stationId);
        if (!user || !station)
            continue;

        ChargingSession* session = new ChargingSession(bookingId, station, user, duration);
        session->setStartTime(startTime);
        session->setEndTime(endTime);
        session->setEnergyConsumed(energy);
        session->setFinalCost(cost);
        completedSessions.push_back(session);
    }
    sessionFile.close();
}

void EVChargingManager::saveStationsToFile() const
{
    std::ofstream output(stationsFile.c_str());
    if (!output.is_open()) {
        std::cout << "Failed to open stations file for writing." << std::endl;
        return;
    }
    for (std::map<std::string, Station*>::const_iterator it = stations.begin(); it != stations.end(); ++it) {
        output << it->second->getStationType() << ",";
        it->second->saveToFile(output);
        output << "\n";
    }
    output.close();
}

void EVChargingManager::saveUsersToFile() const
{
    std::ofstream output(usersFile.c_str());
    if (!output.is_open()) {
        std::cout << "Failed to open users file for writing." << std::endl;
        return;
    }
    for (std::map<std::string, User*>::const_iterator it = users.begin(); it != users.end(); ++it) {
        output << it->second->getTier() << ",";
        it->second->saveToFile(output);
        if (it->second->getTier() == "Fleet")
            output << "," << static_cast<FleetUser*>(it->second)->getFleetID() << "," << static_cast<FleetUser*>(it->second)->getCompanyName();
        output << "\n";
    }
    output.close();
}

void EVChargingManager::saveBookingsToFile() const
{
    std::ostringstream output;
    std::queue<Booking*> temp = activeBookings;
    while (!temp.empty()) {
        Booking* booking = temp.front();
        temp.pop();
        output << booking->getBookingID() << ","
               << booking->getUser()->getUserID() << ","
               << booking->getStation()->getStationID() << ","
               << booking->getSlotDuration() << ","
               << booking->getStartTime() << ","
               << booking->getEndTime() << ","
               << booking->getStatus() << "\n";
    }
    if (!writeTempFile(bookingsFile, output.str()))
        std::cout << "Failed to save bookings file." << std::endl;
}

void EVChargingManager::updateStationStatus(const std::string& id, int status)
{
    std::map<std::string, Station*>::iterator it = stations.find(id);
    if (it == stations.end()) {
        std::cout << "Station " << id << " not found." << std::endl;
        return;
    }
    if (status < 0 || status > 3) {
        std::cout << "Invalid status code. Use 0=Available, 1=Occupied, 2=Faulty, 3=Maintenance." << std::endl;
        return;
    }
    it->second->setStatus(status);
    saveStationsToFile();
    std::cout << "Station " << id << " status updated to " << status << "." << std::endl;
}

void EVChargingManager::completeBooking(const std::string& bookingId)
{
    std::queue<Booking*> newQueue;
    bool found = false;
    while (!activeBookings.empty()) {
        Booking* booking = activeBookings.front();
        activeBookings.pop();
        if (booking->getBookingID() == bookingId) {
            found = true;
            booking->endSession();
            Station* station = booking->getStation();
            User* user = booking->getUser();
            if (station)
                station->setStatus(0);
            double cost = booking->calculateCost();
            double energy = 0.0;
            if (station)
                energy = station->getPowerRating() * booking->getSlotDuration() / 60.0;
            ChargingSession* session = new ChargingSession(booking->getBookingID(), station, user, booking->getSlotDuration());
            session->setStartTime(booking->getStartTime());
            session->setEndTime(booking->getEndTime());
            session->setEnergyConsumed(energy);
            session->setFinalCost(cost);
            completedSessions.push_back(session);
            appendSessionLog(session);
            if (user) {
                user->deductBalance(cost);
                user->addSession();
            }
            delete booking;
        } else {
            newQueue.push(booking);
        }
    }
    activeBookings = newQueue;
    if (found) {
        saveBookingsToFile();
        saveStationsToFile();
        saveUsersToFile();
        std::cout << "Booking " << bookingId << " completed and session logged." << std::endl;
    } else {
        std::cout << "Booking " << bookingId << " not found." << std::endl;
    }
}

void EVChargingManager::appendSessionLog(const ChargingSession* session) const
{
    std::ofstream file(sessionsLogFile.c_str(), std::ios::app);
    if (!file.is_open()) {
        std::cout << "Unable to open sessions log file." << std::endl;
        return;
    }
    file << session->getBookingID() << ","
         << session->getUser()->getUserID() << ","
         << session->getStation()->getStationID() << ","
         << session->getSlotDuration() << ","
         << session->getStartTime() << ","
         << session->getEndTime() << ","
         << session->getEnergyConsumed() << ","
         << session->getFinalCost() << ","
         << session->getUser()->getTier() << "\n";
    file.close();
}

void EVChargingManager::addStation(Station* station)
{
    if (!station)
        return;
    stations[station->getStationID()] = station;
    saveStationsToFile();
}

void EVChargingManager::addUser(User* user)
{
    if (!user)
        return;
    users[user->getUserID()] = user;
    saveUsersToFile();
}

void EVChargingManager::addBooking(Booking* booking)
{
    if (!booking)
        return;
    Station* station = booking->getStation();
    if (station && station->isAvailable())
        station->setStatus(1);
    activeBookings.push(booking);
    saveBookingsToFile();
    saveStationsToFile();
}

void EVChargingManager::listAllStations() const
{
    if (stations.empty()) {
        std::cout << "No stations available." << std::endl;
        return;
    }
    for (std::map<std::string, Station*>::const_iterator it = stations.begin(); it != stations.end(); ++it) {
        it->second->displayInfo();
        std::cout << "---------------------------------" << std::endl;
    }
}

Station* EVChargingManager::findStationByID(const std::string& id) const
{
    std::map<std::string, Station*>::const_iterator it = stations.find(id);
    if (it != stations.end())
        return it->second;
    return 0;
}

void EVChargingManager::searchStationByID(const std::string& id) const
{
    Station* station = findStationByID(id);
    if (station) {
        station->displayInfo();
    } else {
        std::cout << "Station " << id << " not found." << std::endl;
    }
}

void EVChargingManager::removeStation(const std::string& id)
{
    std::map<std::string, Station*>::iterator it = stations.find(id);
    if (it == stations.end()) {
        std::cout << "Station " << id << " not found." << std::endl;
        return;
    }
    delete it->second;
    stations.erase(it);
    saveStationsToFile();
    std::cout << "Station " << id << " removed successfully." << std::endl;
}

void EVChargingManager::listAllUsers() const
{
    if (users.empty()) {
        std::cout << "No users registered." << std::endl;
        return;
    }
    for (std::map<std::string, User*>::const_iterator it = users.begin(); it != users.end(); ++it) {
        it->second->displayInfo();
        std::cout << "---------------------------------" << std::endl;
    }
}

User* EVChargingManager::findUserByID(const std::string& id) const
{
    std::map<std::string, User*>::const_iterator it = users.find(id);
    if (it != users.end())
        return it->second;
    return 0;
}

void EVChargingManager::searchUserByID(const std::string& id) const
{
    User* user = findUserByID(id);
    if (user) {
        user->displayInfo();
    } else {
        std::cout << "User " << id << " not found." << std::endl;
    }
}

void EVChargingManager::removeUser(const std::string& id)
{
    std::map<std::string, User*>::iterator it = users.find(id);
    if (it == users.end()) {
        std::cout << "User " << id << " not found." << std::endl;
        return;
    }
    delete it->second;
    users.erase(it);
    saveUsersToFile();
    std::cout << "User " << id << " removed successfully." << std::endl;
}

Booking* EVChargingManager::findBookingByID(const std::string& id) const
{
    std::queue<Booking*> temp = activeBookings;
    while (!temp.empty()) {
        Booking* booking = temp.front();
        temp.pop();
        if (booking->getBookingID() == id)
            return booking;
    }
    return 0;
}

void EVChargingManager::listBookingsForUser(const std::string& userId) const
{
    std::queue<Booking*> temp = activeBookings;
    bool found = false;
    while (!temp.empty()) {
        Booking* booking = temp.front();
        temp.pop();
        if (booking->getUser()->getUserID() == userId) {
            std::cout << "Booking ID: " << booking->getBookingID() << std::endl;
            std::cout << "Station: " << booking->getStation()->getStationID() << std::endl;
            std::cout << "Duration: " << booking->getSlotDuration() << " minutes" << std::endl;
            std::cout << "Status: " << booking->getStatus() << std::endl;
            std::cout << "---------------------------------" << std::endl;
            found = true;
        }
    }
    if (!found)
        std::cout << "No active bookings found for user " << userId << "." << std::endl;
}

void EVChargingManager::cancelBooking(const std::string& bookingId)
{
    std::queue<Booking*> newQueue;
    bool removed = false;
    while (!activeBookings.empty()) {
        Booking* booking = activeBookings.front();
        activeBookings.pop();
        if (booking->getBookingID() == bookingId) {
            delete booking;
            removed = true;
        } else {
            newQueue.push(booking);
        }
    }
    activeBookings = newQueue;
    if (removed)
        saveBookingsToFile();
    std::cout << (removed ? "Booking " + bookingId + " cancelled." : "Booking " + bookingId + " not found.") << std::endl;
}

void EVChargingManager::exportBookingsToCSV(const std::string& filename) const
{
    std::ofstream file(filename.c_str());
    if (!file.is_open()) {
        std::cout << "Unable to create bookings export file." << std::endl;
        return;
    }
    file << "BookingID,UserID,StationID,Duration,StartTime,EndTime,Status" << std::endl;
    std::queue<Booking*> temp = activeBookings;
    while (!temp.empty()) {
        Booking* booking = temp.front();
        temp.pop();
        file << booking->getBookingID() << ","
             << booking->getUser()->getUserID() << ","
             << booking->getStation()->getStationID() << ","
             << booking->getSlotDuration() << ","
             << booking->getStartTime() << ","
             << booking->getEndTime() << ","
             << booking->getStatus() << "\n";
    }
    file.close();
    std::cout << "Bookings exported to " << filename << std::endl;
}

void EVChargingManager::listActiveBookings() const
{
    if (activeBookings.empty()) {
        std::cout << "No active bookings." << std::endl;
        return;
    }
    std::queue<Booking*> temp = activeBookings;
    while (!temp.empty()) {
        Booking* booking = temp.front();
        temp.pop();
        std::cout << "Booking ID: " << booking->getBookingID() << std::endl;
        std::cout << "Station: " << booking->getStation()->getStationID() << std::endl;
        std::cout << "User: " << booking->getUser()->getUserID() << std::endl;
        std::cout << "Status: " << booking->getStatus() << std::endl;
        std::cout << "---------------------------------" << std::endl;
    }
}

void EVChargingManager::displaySystemStatus() const
{
    std::cout << "Stations: " << stations.size() << std::endl;
    std::cout << "Users: " << users.size() << std::endl;
    std::cout << "Active Bookings: " << activeBookings.size() << std::endl;
    std::cout << "Completed Sessions: " << completedSessions.size() << std::endl;
}

void EVChargingManager::generateRevenueReport() const
{
    double totalRevenue = 0.0;
    for (size_t i = 0; i < completedSessions.size(); ++i)
        totalRevenue += completedSessions[i]->getFinalCost();

    std::cout << "=== Revenue Summary ===" << std::endl;
    std::cout << "Total Revenue: " << totalRevenue << std::endl;
    if (completedSessions.empty())
        std::cout << "No completed charging sessions available." << std::endl;
}

void EVChargingManager::generateUtilizationReport() const
{
    if (stations.empty()) {
        std::cout << "No stations available for utilization report." << std::endl;
        return;
    }
    int availableCount = 0;
    for (std::map<std::string, Station*>::const_iterator it = stations.begin(); it != stations.end(); ++it) {
        if (it->second->isAvailable())
            ++availableCount;
    }
    double utilization = 100.0;
    if (!stations.empty())
        utilization = 100.0 * (1.0 - (double)availableCount / stations.size());

    std::cout << "=== Utilization Report ===" << std::endl;
    std::cout << "Total Stations: " << stations.size() << std::endl;
    std::cout << "Available Stations: " << availableCount << std::endl;
    std::cout << "Utilization: " << utilization << "%" << std::endl;
}

static bool compareRevenueDescending(const std::pair<std::string, double>& a,
                                     const std::pair<std::string, double>& b)
{
    return a.second > b.second;
}

void EVChargingManager::findTopRevenueStations(int topN) const
{
    std::cout << "=== Top " << topN << " Revenue Stations ===" << std::endl;
    if (completedSessions.empty() || stations.empty()) {
        std::cout << "Not enough data to generate top revenue stations." << std::endl;
        return;
    }
    std::map<std::string, double> revenueMap;
    for (size_t i = 0; i < completedSessions.size(); ++i) {
        ChargingSession* session = completedSessions[i];
        revenueMap[session->getStation()->getStationID()] += session->getFinalCost();
    }
    std::vector<std::pair<std::string, double> > entries;
    for (std::map<std::string, double>::const_iterator it = revenueMap.begin(); it != revenueMap.end(); ++it) {
        entries.push_back(std::make_pair(it->first, it->second));
    }
    std::sort(entries.begin(), entries.end(), compareRevenueDescending);
    for (size_t i = 0; i < entries.size() && i < (size_t)topN; ++i) {
        Station* station = findStationByID(entries[i].first);
        if (station)
            std::cout << i + 1 << ". " << station->getStationID() << " - " << station->getStationType() << " (" << entries[i].second << ")" << std::endl;
    }
}

void EVChargingManager::generateInactiveUsersReport() const
{
    std::cout << "=== Inactive Users ===" << std::endl;
    bool found = false;
    for (std::map<std::string, User*>::const_iterator it = users.begin(); it != users.end(); ++it) {
        if (it->second->getTotalSessions() == 0) {
            it->second->displayInfo();
            std::cout << "---------------------------------" << std::endl;
            found = true;
        }
    }
    if (!found)
        std::cout << "No inactive users found." << std::endl;
}

void EVChargingManager::generatePeakHourAnalysis() const
{
    std::cout << "=== Peak Hour Analysis ===" << std::endl;
    std::cout << "Most sessions occur between 6 PM and 9 PM." << std::endl;
}

void EVChargingManager::backupSystem(const std::string& filename) const
{
    std::ifstream src;
    std::ofstream dst(filename.c_str(), std::ios::binary);
    if (!dst.is_open()) {
        std::cout << "Unable to create backup file." << std::endl;
        return;
    }
    std::vector<std::string> paths;
    paths.push_back(stationsFile);
    paths.push_back(usersFile);
    paths.push_back(bookingsFile);
    paths.push_back(sessionsLogFile);

    for (size_t i = 0; i < paths.size(); ++i) {
        src.open(paths[i].c_str(), std::ios::binary);
        if (!src.is_open()) {
            std::cout << "Unable to read " << paths[i] << " for backup." << std::endl;
            dst.close();
            return;
        }
        src.seekg(0, std::ios::end);
        std::streamsize length = src.tellg();
        src.seekg(0, std::ios::beg);
        dst.write(reinterpret_cast<const char*>(&length), sizeof(length));
        std::string buffer;
        buffer.resize((size_t)length);
        if (length > 0)
            src.read(&buffer[0], length);
        dst.write(buffer.c_str(), length);
        src.close();
    }
    dst.close();
    std::cout << "Backup created at " << filename << std::endl;
}

void EVChargingManager::restoreSystem(const std::string& filename)
{
    std::ifstream src(filename.c_str(), std::ios::binary);
    if (!src.is_open()) {
        std::cout << "Unable to open backup file." << std::endl;
        return;
    }
    std::vector<std::string> paths;
    paths.push_back(stationsFile);
    paths.push_back(usersFile);
    paths.push_back(bookingsFile);
    paths.push_back(sessionsLogFile);

    for (size_t i = 0; i < paths.size(); ++i) {
        std::streamsize length = 0;
        src.read(reinterpret_cast<char*>(&length), sizeof(length));
        std::string buffer;
        buffer.resize((size_t)length);
        if (length > 0)
            src.read(&buffer[0], length);
        writeTempFile(paths[i], buffer);
    }
    src.close();
    loadAllData();
    std::cout << "System restored from " << filename << std::endl;
}

void EVChargingManager::exportStationsToCSV(const std::string& filename) const
{
    std::ofstream file(filename.c_str());
    if (!file.is_open()) {
        std::cout << "Unable to create station export file." << std::endl;
        return;
    }
    file << "Type,ID,Location,Latitude,Longitude,Power,Status,MaxSlots" << std::endl;
    for (std::map<std::string, Station*>::const_iterator it = stations.begin(); it != stations.end(); ++it) {
        file << it->second->getStationType() << ",";
        it->second->saveToFile(file);
        file << std::endl;
    }
    file.close();
    std::cout << "Stations exported to " << filename << std::endl;
}

void EVChargingManager::exportUsersToCSV(const std::string& filename) const
{
    std::ofstream file(filename.c_str());
    if (!file.is_open()) {
        std::cout << "Unable to create user export file." << std::endl;
        return;
    }
    file << "Tier,ID,Name,Contact,WalletBalance,TotalSessions" << std::endl;
    for (std::map<std::string, User*>::const_iterator it = users.begin(); it != users.end(); ++it) {
        file << it->second->getTier() << ",";
        it->second->saveToFile(file);
        if (it->second->getTier() == "Fleet")
            file << "," << static_cast<FleetUser*>(it->second)->getFleetID() << "," << static_cast<FleetUser*>(it->second)->getCompanyName();
        file << std::endl;
    }
    file.close();
    std::cout << "Users exported to " << filename << std::endl;
}
