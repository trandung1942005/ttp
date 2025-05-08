#ifndef ROOMMANAGER_H
#define ROOMMANAGER_H

#include <string>
#include <vector>
#include "Room.h"
#include <fstream> // Include for file operations

struct Customer {
    std::string id;
    std::string roomType;
    std::string roomNumber;
    int daysBooked = 0; // Initialize to 0
    double totalCost = 0.0; // Initialize to 0.0
};

class RoomManager {
public:
    RoomManager();
    void displayMenu() const;
    void chooseRoomType();
    void showRoomStatus() const;
    void showAvailableRooms() const;
    void addGuest(const std::string& id, const std::string& roomType, const std::string& roomNumber, int days);
    void removeGuest(const std::string& id, const std::string& roomType, const std::string& roomNumber);
    void saveCustomerInfo(const Customer& customer); // Save customer info to file
    void loadCustomerInfo(); // Load customer info from file
    void saveRoomStatus(); // Save room statuses to file
    const std::vector<int>& getPhong1() const; // Getter for phong1
    const std::vector<int>& getPhong2() const; // Getter for phong2
    const std::vector<int>& getPhong3() const; // Getter for phong3

private:
    std::vector<int> phong1; // Status of rooms 1-20
    std::vector<int> phong2; // Status of rooms 21-40
    std::vector<int> phong3; // Status of rooms 41-50
    std::vector<Room> roomsList; // Store Room objects
    std::vector<Customer> customers; // Store customer information
    void initRooms();
    void printRoomList(const std::vector<int>& rooms) const;
    void printAvailableRooms(const std::vector<int>& rooms) const; // New method to print available rooms
    void rentFrom(std::vector<int>& rooms, int roomType, int days, const std::string& id);
};

#endif // ROOMMANAGER_H
