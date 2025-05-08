//quan li phong
#include "RoomManager.h"
#include <iostream>
#include <fstream>
#include <algorithm> // For std::find_if

using namespace std;

RoomManager::RoomManager() {
    initRooms();
    // Initialize roomsList with Room objects and assign prices based on room type
    for (int i = 1; i <= 50; ++i) {
        double price = 0.0;
        if (i <= 20) {
            price = 100.0; // Rooms 1-20 are phong1
            phong1.push_back(0); // Add to phong1 with status "trống"
        }
        else if (i <= 40) {
            price = 150.0; // Rooms 21-40 are phong2
            phong2.push_back(0); // Add to phong2 with status "trống"
        }
        else {
            price = 200.0; // Rooms 41-50 are phong3
            phong3.push_back(0); // Add to phong3 with status "trống"
        }
        roomsList.emplace_back(to_string(i), price);
    }
    loadCustomerInfo(); // Load customer info from file
}

void RoomManager::initRooms() {
    phong1.clear();
    phong2.clear();
    phong3.clear();
}

// Display menu
void RoomManager::displayMenu() const {
    cout << "Menu:\n";
    cout << "(a) Add guest\n";
    cout << "(b) Remove guest\n";
    cout << "(c) Show room prices\n";
    cout << "(d) Change password\n";
    cout << "(e) Add employee\n";
    cout << "(f) Remove employee\n";
    cout << "(g) Show available rooms\n";
    cout << "(h) Display revenue\n";
    cout << "(i) Quit\n";
}

void RoomManager::chooseRoomType() {
    int roomType;
    vector<int>* selectedRooms = nullptr;
    double pricePerDay = 0.0;

    // Loop until a valid room type is entered
    while (true) {
        cout << "Choose room type (1, 2, or 3): ";
        cin >> roomType;

        // Determine the room type and price per day
        switch (roomType) {
        case 1:
            selectedRooms = &phong1;
            pricePerDay = 100.0; // Price for phong1
            break;
        case 2:
            selectedRooms = &phong2;
            pricePerDay = 150.0; // Price for phong2
            break;
        case 3:
            selectedRooms = &phong3;
            pricePerDay = 200.0; // Price for phong3
            break;
        default:
            cout << "Error: Invalid room type selected. Please choose 1, 2, or 3." << endl;
            continue; // Prompt the user again
        }
        break; // Exit the loop if a valid room type is entered
    }

    // Display available rooms
    cout << "Available rooms for type " << roomType << ":\n";
    printRoomList(*selectedRooms);

    // Select a room to rent
    int roomNumber;
    cout << "Enter the room number to rent (1 -> " << selectedRooms->size() << "): ";
    cin >> roomNumber;

    // Validate room number
    if (roomNumber < 1 || roomNumber >(int)selectedRooms->size()) {
        cout << "Error: Invalid room number! Please choose a number between 1 and " << selectedRooms->size() << "." << endl;
        return;
    }

    // Check if the room is already occupied
    if ((*selectedRooms)[roomNumber - 1] == 0) {
        (*selectedRooms)[roomNumber - 1] = 1; // Mark room as occupied

        // Ask for the number of days to rent
        int days;
        cout << "Enter the number of days to rent: ";
        cin >> days;

        // Calculate the total cost
        double totalCost = days * pricePerDay;

        // Save customer information
        string customerId;
        cout << "Enter your customer ID: ";
        cin >> customerId;

        customers.push_back({ customerId, to_string(roomType), to_string(roomNumber), days, totalCost });
        saveCustomerInfo(customers.back()); // Save customer info
        saveRoomStatus(); // Save room statuses

        cout << "Room " << roomNumber << " rented successfully for " << days << " days. Total cost: $" << totalCost << endl;
    }


}



void RoomManager::printRoomList(const vector<int>& rooms) const {
    for (size_t i = 0; i < rooms.size(); ++i) {
        cout << "Phong " << (i + 1) << ": "
            << (rooms[i] == 0 ? "Con trong" : "Da thue") << '\n';
    }
}
// Rent a room
void RoomManager::rentFrom(vector<int>& rooms, int roomType, int days, const string& id) {
    cout << "Danh sach phong:\n";
    printRoomList(rooms);
    int roomNumber;
    cout << "Nhap so phong muon thue (1 -> " << rooms.size() << "): ";
    cin >> roomNumber;
    if (roomNumber < 1 || roomNumber >(int)rooms.size()) {
        cout << "So phong khong hop le.\n";
        return;
    }
    if (rooms[roomNumber - 1] == 0) {
        rooms[roomNumber - 1] = 1; // Mark room as occupied
        cout << "Thue phong thanh cong.\n";

        // Determine the price per day based on the room type
        double pricePerDay = 0.0;
        if (roomType == 1) {
            pricePerDay = 100.0; // Price for phong1
        }
        else if (roomType == 2) {
            pricePerDay = 150.0; // Price for phong2
        }
        else if (roomType == 3) {
            pricePerDay = 200.0; // Price for phong3
        }

        // Calculate the total cost
        double totalCost = days * pricePerDay;

        // Save customer information
        customers.push_back({ id, to_string(roomType), to_string(roomNumber), days, totalCost });
        saveCustomerInfo(customers.back());
    }
    else {
        cout << "Phong da duoc thue.\n";
    }
}

// Add a guest
void RoomManager::addGuest(const string& id, const string& roomType, const string& roomNumber, int days) {
    // Kiểm tra loại phòng hợp lệ
    if (roomType != "1" && roomType != "2" && roomType != "3") {
        cout << "Error: Invalid room type. Please choose 1, 2, or 3." << endl;
        return;
    }

    auto it = find_if(roomsList.begin(), roomsList.end(), [&](const Room& room) {
        return room.getRoomNumber() == roomNumber;
        });

    if (it != roomsList.end()) {
        it->setRentedBy(id);
        it->setDaysBooked(days);
        it->setRoomStatus(true); // Mark the room as occupied
        double totalCost = it->calculateTotalCost(it->getDaysBooked()); // Pass the number of days
        customers.push_back({ id, roomType, roomNumber, days, totalCost });
        saveCustomerInfo(customers.back());

        // Update room status in the corresponding room type list
        int roomNum = stoi(roomNumber);
        if (roomType == "1") {
            phong1[roomNum - 1] = 1; // Mark the room as occupied in phong1
        }
        else if (roomType == "2") {
            phong2[roomNum - 1] = 1; // Mark the room as occupied in phong2
        }
        else if (roomType == "3") {
            phong3[roomNum - 1] = 1; // Mark the room as occupied in phong3
        }
        saveRoomStatus();
        cout << "Guest added successfully to room " << roomNumber << " for " << days << " days." << endl;
    }
    else {
        cout << "Error: Room " << roomNumber << " not found." << endl;
    }
}



// Remove a guest
void RoomManager::removeGuest(const string& id, const string& roomType, const string& roomNumber) {
    auto it = find_if(roomsList.begin(), roomsList.end(), [&](const Room& room) {
        return room.getRoomNumber() == roomNumber;
        });


    if (it != roomsList.end()) {
        it->setRentedBy("");
        it->setDaysBooked(0);
        it->setRoomStatus(false); // Mark the room as available
        customers.erase(remove_if(customers.begin(), customers.end(), [&](const Customer& customer) {
            return customer.id == id && customer.roomNumber == roomNumber;
            }), customers.end());

        // Update room status in the corresponding room type list
        int roomNum = stoi(roomNumber);
        if (roomType == "1") {
            phong1[roomNum - 1] = 0; // Mark the room as available in phong1
        }
        else if (roomType == "2") {
            phong2[roomNum - 1] = 0; // Mark the room as available in phong2
        }
        else if (roomType == "3") {
            phong3[roomNum - 1] = 0; // Mark the room as available in phong3
        }
         // Save updated room statuses
        cout << "Guest removed successfully from room " << roomNumber << "." << endl;
    }

}


// Show room status
void RoomManager::showRoomStatus() const {
    cout << "Trang thai phong 1:\n";
    printRoomList(phong1);
    cout << "Trang thai phong 2:\n";
    printRoomList(phong2);
    cout << "Trang thai phong 3:\n";
    printRoomList(phong3);
}

// Show available rooms

void RoomManager::showAvailableRooms() const {
    std::cout << "Available Rooms:\n";

    // Group rooms by type and display them
    std::cout << "Loai Phong 1 ():\n";
    for (size_t i = 0; i < phong1.size(); ++i) {
        if (phong1[i] == 0) { // Room is available
            std::cout << "Room " << (i + 1) << " (Price: $100 per day) is available.\n";
        }
    }

    std::cout << "\nLoai Phong 2 ():\n";
    for (size_t i = 0; i < phong2.size(); ++i) {
        if (phong2[i] == 0) { // Room is available
            std::cout << "Room " << (i + 21) << " (Price: $150 per day) is available.\n";
        }
    }

    std::cout << "\nLoai Phong 3 ():\n";
    for (size_t i = 0; i < phong3.size(); ++i) {
        if (phong3[i] == 0) { // Room is available
            std::cout << "Room " << (i + 41) << " (Price: $200 per day) is available.\n";
        }
    }

    std::cout << "End of available rooms list.\n";
}



void RoomManager::printAvailableRooms(const vector<int>& rooms) const {
    bool hasAvailableRooms = false;
    for (size_t i = 0; i < rooms.size(); ++i) {
        if (rooms[i] == 0) { // Kiểm tra phòng trống
            cout << "Phong " << (i + 1) << ": Con trong\n";
            hasAvailableRooms = true;
        }
    }
    if (!hasAvailableRooms) {
        cout << "Tat ca cac phong da duoc thue.\n";
    }
}


// Load customer information from file
void RoomManager::loadCustomerInfo() {
    ifstream infile("customers.in");
    if (!infile) {
        cout << "No customer data file found." << endl;
        return;
    }

    Customer customer;
    while (infile >> customer.id >> customer.roomType >> customer.roomNumber >> customer.daysBooked >> customer.totalCost) {
        customers.push_back(customer);
    }

    infile.close();
    cout << "Customer information loaded successfully." << endl;
}

// Save customer information to file
void RoomManager::saveCustomerInfo(const Customer& customer) {
    ofstream outfile("customers.in", ios::app); // Open file in append mode
    if (outfile) {
        outfile << customer.id << " "
            << customer.roomType << " "
            << customer.roomNumber << " "
            << customer.daysBooked << " "
            << customer.totalCost << endl;
        outfile.close();
        cout << "Customer information saved successfully to customers.in." << endl;
    }
    else {
        cerr << "Failed to open customers.in for writing." << endl;
    }
}

const std::vector<int>& RoomManager::getPhong1() const {
    return phong1;
}

const std::vector<int>& RoomManager::getPhong2() const {
    return phong2;
}

const std::vector<int>& RoomManager::getPhong3() const {
    return phong3;
}


void RoomManager::saveRoomStatus() {
    ofstream outfile("customers.in"); // Open file in write mode
    if (outfile) {
        // Save phong1 status
        outfile << "Phong1:\n";
        for (size_t i = 0; i < phong1.size(); ++i) {
            outfile << "Room " << (i + 1) << ": " << (phong1[i] == 0 ? "Available" : "Occupied") << endl;
        }

        // Save phong2 status
        outfile << "\nPhong2:\n";
        for (size_t i = 0; i < phong2.size(); ++i) {
            outfile << "Room " << (i + 21) << ": " << (phong2[i] == 0 ? "Available" : "Occupied") << endl;
        }

        // Save phong3 status
        outfile << "\nPhong3:\n";
        for (size_t i = 0; i < phong3.size(); ++i) {
            outfile << "Room " << (i + 41) << ": " << (phong3[i] == 0 ? "Available" : "Occupied") << endl;
        }

        outfile.close();
        cout << "Room statuses saved successfully to customers.in." << endl;
    }
    else {
        cerr << "Failed to open customers.in for writing." << endl;
    }
}

