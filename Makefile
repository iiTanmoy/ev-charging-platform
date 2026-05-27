main: src/Station.cpp src/ACStation.cpp src/DCFastStation.cpp src/DCUltraFastStation.cpp src/User.cpp src/RegularUser.cpp src/PremiumUser.cpp src/FleetUser.cpp src/Booking.cpp src/ChargingSession.cpp src/EVChargingManager.cpp main.cpp
	g++ -std=c++98 -Wall -pedantic -o EVPlatform $^
	@echo "Build successful! Run with: ./EVPlatform"

%.o: %.cpp
	g++ -std=c++98 -Wall -pedantic -c $<

clean:
	rm -f *.o EVPlatform
	@echo "Cleaned build files"

.PHONY: all clean
