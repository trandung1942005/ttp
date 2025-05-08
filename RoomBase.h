#ifndef ROOMBASE_H
#define ROOMBASE_H

#include <string>

class RoomBase {
protected:
    std::string roomNumber;
    double price;
    bool roomStatus; // true = occupied, false = available

public:
    RoomBase(std::string rn, double p);
    virtual ~RoomBase();

    std::string getRoomNumber() const;
    double getPrice() const;
    bool getRoomStatus() const;
    void setRoomStatus(bool status);

    virtual double calculateTotalCost(int days) const = 0; // Pure virtual method
};

#endif // ROOMBASE_H
