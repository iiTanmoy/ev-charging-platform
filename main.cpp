#include "src/EVChargingManager.h"
#include "src/ACStation.h"
#include "src/DCFastStation.h"
#include "src/DCUltraFastStation.h"
#include "src/RegularUser.h"
#include "src/PremiumUser.h"
#include "src/FleetUser.h"
#include <iostream>
#include <string>
#include <cstdlib>

EVChargingManager* manager;

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void displayMainMenu() {
    clearScreen();
    std::cout << "============================================================" << std::endl;
    std::cout << "  SMART EV CHARGING NETWORK OPERATIONS PLATFORM" << std::endl;
    std::cout << "  ChargeGrid Mobility Pvt. Ltd." << std::endl;
    std::cout << "============================================================" << std::endl;
    std::cout << "\n1. Admin / Operator Portal" << std::endl;
    std::cout << "2. User Simulation Portal" << std::endl;
    std::cout << "3. Analytics & Reports" << std::endl;
    std::cout << "4. System Backup & Restore" << std::endl;
    std::cout << "5. Exit" << std::endl;
    std::cout << "\nEnter your choice (1-5): ";
}

void displayAdminMenu() {
    clearScreen();
    std::cout << "=================== ADMIN / OPERATOR PORTAL ===================" << std::endl;
    std::cout << "\n1. Station Management" << std::endl;
    std::cout << "2. User Management" << std::endl;
    std::cout << "3. View Real-time Station Status" << std::endl;
    std::cout << "4. Manage Active Bookings" << std::endl;
    std::cout << "5. View System Status" << std::endl;
    std::cout << "6. Back to Main Menu" << std::endl;
    std::cout << "\nEnter your choice: ";
}

void displayStationMenu() {
    clearScreen();
    std::cout << "------------------- Station Management -------------------" << std::endl;
    std::cout << "\n1. Add New Charging Station" << std::endl;
    std::cout << "2. Remove Station" << std::endl;
    std::cout << "3. Update Station Status" << std::endl;
    std::cout << "4. List All Stations" << std::endl;
    std::cout << "5. Search Station by ID" << std::endl;
    std::cout << "6. Back to Admin Menu" << std::endl;
    std::cout << "\nEnter your choice: ";
}

void displayUserMenu() {
    clearScreen();
    std::cout << "------------------- User Management -------------------" << std::endl;
    std::cout << "\n1. Add New User (Register)" << std::endl;
    std::cout << "2. Remove User" << std::endl;
    std::cout << "3. List All Users" << std::endl;
    std::cout << "4. Search User by ID" << std::endl;
    std::cout << "5. View User Details" << std::endl;
    std::cout << "6. Back to Admin Menu" << std::endl;
    std::cout << "\nEnter your choice: ";
}

void addStation() {
    clearScreen();
    std::cout << "Add New Charging Station" << std::endl;
    std::string id, location;
    double lat, lon, power;
    int slots;
    
    std::cout << "Enter Station ID: ";
    std::cin >> id;
    std::cout << "Enter Location: ";
    std::cin.ignore();
    std::getline(std::cin, location);
    std::cout << "Enter Latitude: ";
    std::cin >> lat;
    std::cout << "Enter Longitude: ";
    std::cin >> lon;
    std::cout << "Enter Power Rating (kW): ";
    std::cin >> power;
    std::cout << "Enter Max Slots: ";
    std::cin >> slots;
    std::cout << "Select Type (1=AC, 2=DCFast, 3=DCUltra): ";
    int typeChoice;
    std::cin >> typeChoice;
    
    Station* station = 0;
    if (typeChoice == 1)
        station = new ACStation(id, location, lat, lon, power, slots);
    else if (typeChoice == 2)
        station = new DCFastStation(id, location, lat, lon, power, slots);
    else if (typeChoice == 3)
        station = new DCUltraFastStation(id, location, lat, lon, power, slots);
    
    if (station) {
        manager->addStation(station);
        std::cout << "\nPress Enter to continue...";
        std::cin.ignore();
        std::cin.get();
    }
}

void listStations() {
    clearScreen();
    manager->listAllStations();
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore();
    std::cin.get();
}

void searchStation() {
    clearScreen();
    std::string id;
    std::cout << "Enter Station ID to search: ";
    std::cin >> id;
    manager->searchStationByID(id);
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore();
    std::cin.get();
}

void registerUser() {
    clearScreen();
    std::cout << "Register New User" << std::endl;
    std::string id, name, contact;
    
    std::cout << "Enter User ID: ";
    std::cin >> id;
    std::cout << "Enter Name: ";
    std::cin.ignore();
    std::getline(std::cin, name);
    std::cout << "Enter Contact: ";
    std::getline(std::cin, contact);
    std::cout << "Select Tier (1=Regular, 2=Premium, 3=Fleet): ";
    int tierChoice;
    std::cin >> tierChoice;
    
    User* user = 0;
    if (tierChoice == 1)
        user = new RegularUser(id, name, contact);
    else if (tierChoice == 2)
        user = new PremiumUser(id, name, contact);
    else if (tierChoice == 3)
        user = new FleetUser(id, name, contact, "FLT" + id, "Fleet Corp");
    
    if (user) {
        manager->addUser(user);
        std::cout << "\nPress Enter to continue...";
        std::cin.ignore();
        std::cin.get();
    }
}

void listUsers() {
    clearScreen();
    manager->listAllUsers();
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore();
    std::cin.get();
}

void searchUser() {
    clearScreen();
    std::string id;
    std::cout << "Enter User ID to search: ";
    std::cin >> id;
    manager->searchUserByID(id);
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore();
    std::cin.get();
}

void stationManagement() {
    int choice;
    while (true) {
        displayStationMenu();
        std::cin >> choice;
        
        switch (choice) {
            case 1:
                addStation();
                break;
            case 2: {
                std::cout << "Remove Station - Enter station ID to remove: ";
                std::string id;
                std::cin >> id;
                manager->removeStation(id);
                std::cout << "Press Enter to continue...";
                std::cin.ignore();
                std::cin.get();
                break;
            }
            case 3:
                std::cout << "Update Station Status - Not yet fully implemented" << std::endl;
                std::cout << "Press Enter to continue...";
                std::cin.ignore();
                std::cin.get();
                break;
            case 4:
                listStations();
                break;
            case 5:
                searchStation();
                break;
            case 6:
                return;
            default:
                std::cout << "Invalid choice!" << std::endl;
        }
    }
}

void userManagement() {
    int choice;
    while (true) {
        displayUserMenu();
        std::cin >> choice;
        
        switch (choice) {
            case 1:
                registerUser();
                break;
            case 2: {
                std::cout << "Remove User - Enter user ID: ";
                std::string id;
                std::cin >> id;
                manager->removeUser(id);
                std::cout << "Press Enter to continue...";
                std::cin.ignore();
                std::cin.get();
                break;
            }
            case 3:
                listUsers();
                break;
            case 4:
                searchUser();
                break;
            case 5: {
                std::cout << "View User Details - Enter user ID: ";
                std::string id;
                std::cin >> id;
                manager->searchUserByID(id);
                std::cout << "\nPress Enter to continue...";
                std::cin.ignore();
                std::cin.get();
                break;
            }
            case 6:
                return;
            default:
                std::cout << "Invalid choice!" << std::endl;
        }
    }
}

void adminPortal() {
    int choice;
    while (true) {
        displayAdminMenu();
        std::cin >> choice;
        
        switch (choice) {
            case 1:
                stationManagement();
                break;
            case 2:
                userManagement();
                break;
            case 3:
                clearScreen();
                manager->listAllStations();
                std::cout << "\nPress Enter to continue...";
                std::cin.ignore();
                std::cin.get();
                break;
            case 4:
                clearScreen();
                manager->listActiveBookings();
                std::cout << "\nPress Enter to continue...";
                std::cin.ignore();
                std::cin.get();
                break;
            case 5:
                clearScreen();
                manager->displaySystemStatus();
                std::cout << "\nPress Enter to continue...";
                std::cin.ignore();
                std::cin.get();
                break;
            case 6:
                return;
            default:
                std::cout << "Invalid choice!" << std::endl;
        }
    }
}

void analyticsPortal() {
    int choice;
    while (true) {
        clearScreen();
        std::cout << "=================== ANALYTICS & REPORTS ===================" << std::endl;
        std::cout << "\n1. Revenue Summary Report" << std::endl;
        std::cout << "2. Station Utilization Report" << std::endl;
        std::cout << "3. Top 10 Revenue Stations" << std::endl;
        std::cout << "4. Inactive Users Report" << std::endl;
        std::cout << "5. Peak Hour Analysis" << std::endl;
        std::cout << "6. Back to Main Menu" << std::endl;
        std::cout << "\nEnter your choice: ";
        std::cin >> choice;
        
        switch (choice) {
            case 1:
                clearScreen();
                manager->generateRevenueReport();
                std::cout << "\nPress Enter to continue...";
                std::cin.ignore();
                std::cin.get();
                break;
            case 2:
                clearScreen();
                manager->generateUtilizationReport();
                std::cout << "\nPress Enter to continue...";
                std::cin.ignore();
                std::cin.get();
                break;
            case 3:
                clearScreen();
                manager->findTopRevenueStations(10);
                std::cout << "\nPress Enter to continue...";
                std::cin.ignore();
                std::cin.get();
                break;
            case 4:
                clearScreen();
                manager->generateInactiveUsersReport();
                std::cout << "\nPress Enter to continue...";
                std::cin.ignore();
                std::cin.get();
                break;
            case 5:
                clearScreen();
                manager->generatePeakHourAnalysis();
                std::cout << "\nPress Enter to continue...";
                std::cin.ignore();
                std::cin.get();
                break;
            case 6:
                return;
            default:
                std::cout << "Invalid choice!" << std::endl;
        }
    }
}

void backupRestorePortal() {
    int choice;
    while (true) {
        clearScreen();
        std::cout << "=================== SYSTEM MAINTENANCE ===================" << std::endl;
        std::cout << "\n1. Create Full System Backup" << std::endl;
        std::cout << "2. Restore System from Backup" << std::endl;
        std::cout << "3. Export All Stations to CSV" << std::endl;
        std::cout << "4. Export All Users to CSV" << std::endl;
        std::cout << "5. Back to Main Menu" << std::endl;
        std::cout << "\nEnter your choice: ";
        std::cin >> choice;
        
        switch (choice) {
            case 1:
                clearScreen();
                manager->backupSystem("backup.dat");
                std::cout << "\nPress Enter to continue...";
                std::cin.ignore();
                std::cin.get();
                break;
            case 2:
                clearScreen();
                manager->restoreSystem("backup.dat");
                std::cout << "\nPress Enter to continue...";
                std::cin.ignore();
                std::cin.get();
                break;
            case 3:
                clearScreen();
                manager->exportStationsToCSV("stations_export.csv");
                std::cout << "\nPress Enter to continue...";
                std::cin.ignore();
                std::cin.get();
                break;
            case 4:
                clearScreen();
                manager->exportUsersToCSV("users_export.csv");
                std::cout << "\nPress Enter to continue...";
                std::cin.ignore();
                std::cin.get();
                break;
            case 5:
                return;
            default:
                std::cout << "Invalid choice!" << std::endl;
        }
    }
}

int main() {
    manager = new EVChargingManager();
    
    // Load sample data
    manager->addStation(new ACStation("ST001", "Anna Nagar", 13.0843, 80.2102, 7.2, 4));
    manager->addStation(new DCFastStation("ST002", "Guindy", 13.0067, 80.2206, 25.0, 2));
    manager->addStation(new DCUltraFastStation("ST003", "OMR", 12.9220, 80.1500, 60.0, 3));
    
    manager->addUser(new RegularUser("U001", "Ramesh Kumar", "9876543210"));
    manager->addUser(new PremiumUser("U002", "Priya Sharma", "9123456789"));
    manager->addUser(new FleetUser("U003", "Suresh Reddy", "9988776655", "FLT001", "LogisticsCorp"));
    
    int choice;
    while (true) {
        displayMainMenu();
        std::cin >> choice;
        
        switch (choice) {
            case 1:
                adminPortal();
                break;
            case 2:
                clearScreen();
                std::cout << "User Portal - Coming Soon!" << std::endl;
                std::cout << "Press Enter to continue...";
                std::cin.ignore();
                std::cin.get();
                break;
            case 3:
                analyticsPortal();
                break;
            case 4:
                backupRestorePortal();
                break;
            case 5:
                clearScreen();
                std::cout << "Thank you for using ChargeGrid Mobility!" << std::endl;
                delete manager;
                return 0;
            default:
                std::cout << "Invalid choice!" << std::endl;
        }
    }
    
    return 0;
}
