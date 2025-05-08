#include "RoomManager.h"
#include "Room.h"
#include "Employee.h"
#include "Revenue.h"
#include <iostream>
#include <list>
#include <fstream>
#include <algorithm>
#include <functional>
#include <vector>

using namespace std;

list<Room> rooms;
list<Employee> employees;
RoomManager rm; // Create a single instance of RoomManager
Revenue revenue; // Create a single instance of Revenue

void loadEmployees() {
    ifstream infile("passwords.in");
    string id, password;
    while (infile >> id >> password) {
        employees.push_back(Employee(id, password));
    }
    infile.close();
}

bool login(string id, string password) {
    for (const auto& emp : employees) {
        if (emp.getId() == id && emp.getPassword() == password) {
            std::cout << "Access granted" << std::endl;
            return true;
        }
    }
    std::cout << "Login failed!" << std::endl;
    return false;
}

void changePassword(string id) {
    string oldPassword, newPassword;
    std::cout << "Enter current password: ";
    std::cin >> oldPassword;
    if (login(id, oldPassword)) {
        std::cout << "Enter new password: ";
        std::cin >> newPassword;
        for (auto& emp : employees) {
            if (emp.getId() == id) {
                emp.setPassword(newPassword);
                // Cập nhật mật khẩu trong file
                std::vector<Employee> updatedEmployees(employees.begin(), employees.end());
                std::ofstream outfile("passwords.in");
                if (outfile) {
                    for (const auto& e : updatedEmployees) {
                        outfile << e.getId() << " " << e.getPassword() << std::endl;
                    }
                    outfile.close();
                    std::cout << "Password changed and saved successfully!" << std::endl;
                }
                else {
                    std::cerr << "Failed to open password file for writing." << std::endl;
                }
                return;
            }
        }
    }
}

void addEmployee() {
    string id, password;
    std::cout << "Enter new employee ID: ";
    std::cin >> id;
    std::cout << "Enter new employee password: ";
    std::cin >> password;
    Employee newEmployee(id, password);
    employees.push_back(newEmployee);
    newEmployee.saveEmployeeInfo(); // Save new employee info to passwords.in
    std::cout << "Employee added successfully!" << std::endl;
}

void removeEmployee() {
    string id;
    std::cout << "Enter employee ID to remove: ";
    std::cin >> id;

    // Xóa nhân viên khỏi danh sách employees
    employees.remove_if([&id](const Employee& emp) { return emp.getId() == id; });

    // Đọc toàn bộ nội dung file passwords.in vào một vector tạm thời
    std::vector<Employee> updatedEmployees;
    std::ifstream infile("passwords.in");
    string empId, empPassword;
    while (infile >> empId >> empPassword) {
        if (empId != id) { // Chỉ thêm những nhân viên không bị xóa
            updatedEmployees.push_back(Employee(empId, empPassword));
        }
    }
    infile.close();

    // Ghi lại toàn bộ nội dung đã cập nhật vào file passwords.in
    std::ofstream outfile("passwords.in");
    if (outfile) {
        for (const auto& e : updatedEmployees) {
            outfile << e.getId() << " " << e.getPassword() << std::endl;
        }
        outfile.close();
        std::cout << "Employee removed successfully!" << std::endl;
    }
    else {
        std::cerr << "Failed to open password file for writing." << std::endl;
    }
}

void addGuest() {
    string id, roomType, roomNumber;
    int days;

    // Input employee ID
    std::cout << "Enter your employee ID: ";
    std::cin >> id;

    // Input room type
    std::cout << "Enter room type (1 = Standard, 2 = 10% Extra, 3 = 20% Extra): ";
    std::cin >> roomType;

    // Validate room type
    if (roomType != "1" && roomType != "2" && roomType != "3") {
        std::cout << "Error: Invalid room type. Please choose 1, 2, or 3." << std::endl;
        return;
    }

    // Input room number
    std::cout << "Enter room number: ";
    std::cin >> roomNumber;

    // Input number of days
    std::cout << "Enter number of days: ";
    std::cin >> days;

    // Find the room in the list
    auto it = std::find_if(rooms.begin(), rooms.end(), [&](const Room& room) {
        return room.getRoomNumber() == roomNumber;
        });

    if (it != rooms.end()) {
        // Check if the room is already occupied
        if (it->getRoomStatus()) {
            std::cout << "Error: Room " << roomNumber << " is already occupied." << std::endl;
            return;
        }

        // Set room details
        it->setGuestName("Guest_" + id); // Example guest name
        it->setDaysBooked(days);
        it->setRentedBy(id);
        it->setRoomStatus(true);

        // Calculate total cost based on room type
        double totalCost = 0.0;
        if (roomType == "1") {
            totalCost = static_cast<Room1&>(*it).calculateTotalCost(days);
        }
        else if (roomType == "2") {
            totalCost = static_cast<Room2&>(*it).calculateTotalCost(days);
        }
        else if (roomType == "3") {
            totalCost = static_cast<Room3&>(*it).calculateTotalCost(days);
        }

        // Display success message
        std::cout << "Room " << roomNumber << " rented successfully for " << days
            << " days. Total cost: $" << totalCost << std::endl;
    }
    else {
        std::cout << "Error: Room " << roomNumber << " not found." << std::endl;
    }
}


void removeGuest() {
    string id, roomType, roomNumber;
    std::cout << "Enter your employee ID: ";
    std::cin >> id;
    std::cout << "Enter room type (1, 2, or 3): ";
    std::cin >> roomType;
    std::cout << "Enter room number: ";
    std::cin >> roomNumber;
    rm.removeGuest(id, roomType, roomNumber); // Use the single instance of RoomManager
}

void showRoomPrices() {
    for (auto& room : rooms) {
        std::cout << "Room " << room.getRoomNumber() << ": $" << room.getPrice() << std::endl;
    }
}

void addRevenue(double amount, int month) {
    revenue.addRevenue(amount, month);
}

void displayRevenue() {
    char order;
    std::cout << "Display revenue in (a) ascending or (d) descending order? ";
    std::cin >> order;
    bool ascending = (order == 'a');
    revenue.displayRevenue(ascending);
}

void menu() {
    char option;
    string id, password;
    std::cout << "Enter your employee ID: ";
    std::cin >> id;
    std::cout << "Enter your password: ";
    std::cin >> password;
    if (login(id, password)) {
        do {
            std::cout << "Menu:\n";
            std::cout << "(a) Add guest\n";
            std::cout << "(b) Remove guest\n";
            std::cout << "(c) Show room prices\n";
            std::cout << "(d) Change password\n";
            std::cout << "(e) Add employee\n";
            std::cout << "(f) Remove employee\n";
            std::cout << "(g) Show available rooms\n";
            std::cout << "(h) Display revenue\n";
            std::cout << "(i) Quit\n";
            std::cin >> option;
            switch (option) {
            case 'a': addGuest(); break;
            case 'b': removeGuest(); break;
            case 'c': showRoomPrices(); break;
            case 'd': changePassword(id); break;
            case 'e': addEmployee(); break;
            case 'f': removeEmployee(); break;
            case 'g': rm.showAvailableRooms(); break;
            case 'h': displayRevenue(); break;
            case 'i': std::cout << "Goodbye!" << std::endl; break;
            default: std::cout << "Invalid option!" << std::endl; break;
            }
        } while (option != 'i');
    }
}

int main() {
    // Load employees from file
    loadEmployees();
    // Sample data
  // Create instances of different room types
    Room1 room1("101", 100.0); // Standard room
    Room2 room2("102", 150.0); // Room with 10% extra charge
    Room3 room3("103", 200.0); // Room with 20% extra charge

    // Add rooms to the list
    for (int i = 1; i <= 20; ++i) {
        rooms.push_back(Room1(std::to_string(i), 100.0)); // Rooms 1-20 are Standard
    }
    for (int i = 21; i <= 40; ++i) {
        rooms.push_back(Room2(std::to_string(i), 150.0)); // Rooms 21-40 have 10% extra charge
    }
    for (int i = 41; i <= 50; ++i) {
        rooms.push_back(Room3(std::to_string(i), 200.0)); // Rooms 41-50 have 20% extra charge
    }

    // Set guest details for each room
    room1.setGuestName("Alice");
    room1.setDaysBooked(3);
    room1.setRentedBy("Employee001");

    room2.setGuestName("Bob");
    room2.setDaysBooked(4);
    room2.setRentedBy("Employee002");

    room3.setGuestName("Charlie");
    room3.setDaysBooked(5);
    room3.setRentedBy("Employee003");

    // Calculate and display total costs for each room
    cout << "Room 1 (Standards):" << endl;
    cout << "Guest Name: " << room1.getGuestName() << endl;
    cout << "Days Booked: " << room1.getDaysBooked() << endl;
    cout << "Total Cost: $" << room1.calculateTotalCost(room1.getDaysBooked()) << endl;

    cout << "\nRoom 2 (10% Extra Charge):" << endl;
    cout << "Guest Name: " << room2.getGuestName() << endl;
    cout << "Days Booked: " << room2.getDaysBooked() << endl;
    cout << "Total Cost: $" << room2.calculateTotalCost(room2.getDaysBooked()) << endl;

    cout << "\nRoom 3 (20% Extra Charge):" << endl;
    cout << "Guest Name: " << room3.getGuestName() << endl;
    cout << "Days Booked: " << room3.getDaysBooked() << endl;
    cout << "Total Cost: $" << room3.calculateTotalCost(room3.getDaysBooked()) << endl;

    // Add sample revenue data
    addRevenue(1000.0, 1);
    addRevenue(1500.0, 2);
    addRevenue(2000.0, 3);

    // Display menu
    menu();

    return 0;
}