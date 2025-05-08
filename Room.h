#ifndef ROOM_H
#define ROOM_H

#include "RoomBase.h"
#include <string>

class Room : public RoomBase {
private:
    std::string guestName;
    int daysBooked;
    std::string rentedBy;

public:
    Room(std::string rn, double p);
    std::string getGuestName() const;
    int getDaysBooked() const;
    std::string getRentedBy() const;
    void setGuestName(std::string name);
    void setDaysBooked(int days);
    void setRentedBy(std::string id);
    double calculateTotalCost(int days) const override; // Match the base class signature
};

class Room1 : public Room {
public:
    Room1(std::string rn, double p) : Room(rn, p) {}
    double calculateTotalCost(int days) const override {
        return days * getPrice();
    }
};

class Room2 : public Room {
public:
    Room2(std::string rn, double p) : Room(rn, p) {}
    double calculateTotalCost(int days) const override {
        return days * getPrice() * 1.1; // Example: 10% extra charge
    }
};

class Room3 : public Room {
public:
    Room3(std::string rn, double p) : Room(rn, p) {}
    double calculateTotalCost(int days) const override {
        return days * getPrice() * 1.2; // Example: 20% extra charge
    }
};

#endif // ROOM_H
