# Smart EV Charging Network Operations Platform

A **C++98 console-based** application for managing a complete EV charging network operations system for **ChargeGrid Mobility**.

## 🌟 Features

- **Station Management**: Add, remove, and manage AC, DC Fast, and DC Ultra-Fast charging stations
- **User Management**: Register users with different tiers (Regular, Premium, Fleet)
- **Booking System**: Book charging slots and manage sessions
- **Real-time Monitoring**: View station occupancy and user activity
- **Analytics Engine**: Generate revenue, utilization, and predictive reports
- **Backup & Restore**: Full system backup with binary file I/O
- **STL Integration**: Uses `std::map`, `std::vector`, `std::queue`, `std::algorithm`
- **Pure C++98**: No C++11+ features, compatible with legacy systems

## 📋 Compilation

### Prerequisites
- GCC or Clang compiler supporting C++98 standard
- Make (optional, for Makefile)

### Using Make
```bash
make
./EVPlatform
```

### Using g++ directly
```bash
g++ -std=c++98 -Wall -pedantic src/*.cpp main.cpp -o EVPlatform
./EVPlatform
```

### Windows (MinGW)
```bash
g++ -std=c++98 -Wall -pedantic src/*.cpp main.cpp -o EVPlatform.exe
EVPlatform.exe
```

## 📁 Project Structure

```
ev-charging-platform/
├── src/
│   ├── Station.h / Station.cpp                    (Base class)
│   ├── ACStation.h / ACStation.cpp                (7.2 kW)
│   ├── DCFastStation.h / DCFastStation.cpp        (25 kW)
│   ├── DCUltraFastStation.h / DCUltraFastStation.cpp (60 kW)
│   ├── User.h / User.cpp                          (Base class)
│   ├── RegularUser.h / RegularUser.cpp            (No discount)
│   ├── PremiumUser.h / PremiumUser.cpp            (15% discount)
│   ├── FleetUser.h / FleetUser.cpp                (25% discount)
│   ├── Booking.h / Booking.cpp                    (Base booking)
│   ├── ChargingSession.h / ChargingSession.cpp    (Session tracking)
│   └── EVChargingManager.h / EVChargingManager.cpp (Manager)
├── main.cpp                                        (Menu-driven UI)
├── Makefile
├── data/
│   ├── stations.csv       (Station database)
│   ├── users.csv          (User database)
│   ├── bookings.csv       (Active bookings)
│   ├── sessions.log       (Session transactions)
│   └── backup.dat         (Binary backup)
└── README.md
```

## 🎯 Key Design Patterns

### 1. **Object-Oriented Programming**
```cpp
// Inheritance Hierarchy
Station (abstract)
├── ACStation
├── DCFastStation
└── DCUltraFastStation

User (abstract)
├── RegularUser
├── PremiumUser
└── FleetUser

Booking (base)
└── ChargingSession (derived)
```

### 2. **Polymorphism**
- Pure virtual functions: `calculatePricePerMin()`, `getTier()`, `calculateDiscount()`
- Runtime polymorphic behavior for pricing and user tier handling

### 3. **STL Containers**
```cpp
std::map<std::string, Station*>        // O(log n) station lookup
std::map<std::string, User*>           // User directory
std::queue<Booking*>                   // FIFO active bookings
std::vector<ChargingSession>           // Completed sessions
```

### 4. **File I/O Patterns**
- **CSV Format**: Human-readable data persistence
- **Binary Backup**: System state snapshots
- **Append-only Log**: Transaction audit trail

## 💰 Pricing Model

| Station Type | Price/Min | User Tier | Discount |
|--|--|--|--|
| AC Standard | ₹0.50 | Regular | 0% |
| DC Fast | ₹1.50 | Premium | 15% |
| DC Ultra-Fast | ₹3.00 | Fleet | 25% |

**Example**: Premium user at DC Fast = ₹1.50 - (₹1.50 × 15%) = **₹1.275/min**

## 📊 Menu Structure

### Main Menu
```
1. Admin / Operator Portal
2. User Simulation Portal
3. Analytics & Reports
4. System Backup & Restore
5. Exit
```

### Admin Portal Features
- **Station Management**: Add/Remove/Update/List/Search stations
- **User Management**: Register/Remove/List/Search users
- **Real-time Status**: Live occupancy monitoring
- **Active Bookings**: Manage ongoing charges
- **System Status**: View comprehensive statistics

### Analytics & Reports
- Revenue summary (monthly)
- Station utilization percentage
- Top 10 revenue-generating stations
- Inactive users (>30 days idle)
- Peak hour utilization analysis
- Export to CSV

### System Maintenance
- Full system backup (binary format)
- Restore from backup
- Export all stations to CSV
- Export all users to CSV
- Export sessions log

## 🔧 Sample Usage

### Starting the Application
```bash
$ make
$ ./EVPlatform

============================================================
  SMART EV CHARGING NETWORK OPERATIONS PLATFORM
  ChargeGrid Mobility Pvt. Ltd.
============================================================

1. Admin / Operator Portal
2. User Simulation Portal
3. Analytics & Reports
4. System Backup & Restore
5. Exit

Enter your choice (1-5): 1
```

### Adding a Station
```
Enter Station ID: ST004
Enter Location: T Nagar
Enter Latitude: 13.0418
Enter Longitude: 80.2341
Enter Power Rating (kW): 25
Enter Max Slots: 4
Select Type (1=AC, 2=DCFast, 3=DCUltra): 2
Station ST004 added successfully.
```

### Registering a User
```
Enter User ID: U008
Enter Name: Arjun Singh
Enter Contact: 9876123456
Select Tier (1=Regular, 2=Premium, 3=Fleet): 2
User U008 registered successfully.
```

## 📊 Sample Data Files

### stations.csv
```
AC,ST001,Anna Nagar,13.0843,80.2102,7.2,0,4
DCFast,ST002,Guindy,13.0067,80.2206,25.0,0,2
DCUltra,ST003,OMR,12.9220,80.1500,60.0,1,3
```

### users.csv
```
Regular,U001,Ramesh Kumar,9876543210,450.75,12
Premium,U002,Priya Sharma,9123456789,1240.50,28
Fleet,U003,Suresh Reddy,9988776655,8750.00,67
```

### sessions.log (Append-only)
```
SESS001,B001,U003,ST008,1744400000,1744407200,28.5,1425.00,Fleet
SESS002,B002,U002,ST003,1744412000,1744416500,12.8,384.00,Premium
```

## 🚀 Performance Characteristics

| Operation | Complexity | Example |
|--|--|--|
| Find station by ID | O(log n) | `std::map::find()` |
| Find user by ID | O(log n) | `std::map::find()` |
| List all stations | O(n) | Iterate through map |
| Add booking | O(log n) | Queue push + status update |
| Search by type | O(n) | Iterate + filter |

**Handles 10,000+ records efficiently** with O(log n) average lookup time.

## 🔐 C++98 Compliance

✅ **Strict Adherence to ISO/IEC 14882:1998**

Approved Features:
- `std::string`, `std::vector`, `std::map`, `std::queue`, `std::set`
- `std::algorithm` (for_each, find_if, sort, copy_if)
- `std::fstream` (file I/O)
- Virtual functions and inheritance
- Exceptions and RTTI

❌ **Prohibited Features (C++11+)**
- `auto` keyword
- `nullptr`
- Range-based for loops
- `std::unordered_map`
- Lambda functions
- `std::array`

## 📝 Class Diagrams

### Station Hierarchy
```
     +----------+
     | Station  | (abstract)
     +----------+
        |  |  |
    +---+  |  +---+
    |      |      |
  AC     DCFast  DCUltra
  $0.5   $1.5    $3.0
```

### User Hierarchy
```
     +------+
     | User | (abstract)
     +------+
        |  |  |
    +---+  |  +---+
    |      |      |
Regular Premium Fleet
  0%     15%    25%
```

## 🎓 Educational Value

This project demonstrates:

1. **OOP Concepts**: Inheritance, polymorphism, encapsulation, abstraction
2. **STL Mastery**: Containers, iterators, algorithms for real-world scenarios
3. **File Handling**: CSV parsing, binary I/O, append-only logging
4. **System Design**: Manager pattern, facade architecture
5. **C++98 Constraints**: Working within strict language limitations

## 🔮 Future Enhancements

- GPS-based nearest station recommendation engine
- Dynamic pricing based on real-time demand
- Multi-threaded concurrent session handling
- Web API integration
- Mobile app backend support
- Machine learning for predictive maintenance

## 📄 License

Educational/Commercial Use - ChargeGrid Mobility Pvt. Ltd.

## 👤 Author

**Tanmoy Chowdhury** (iiTanmoy)
- GitHub: [@iiTanmoy](https://github.com/iiTanmoy)
- Email: tanmoy.chowdhury@cognizant.com

## 🤝 Contributing

This is an educational project. Contributions, suggestions, and improvements are welcome!

## 📞 Support

For issues or questions, please raise an issue on the GitHub repository.

---

**Built with ❤️ using pure C++98 standards**
