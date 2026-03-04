//============================================================================
// Name        : Code.cpp
// Author      : eric
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>

#include <vector>//store data
#include <string>//text (strings)
#include <fstream>//file handling (reading and writing files)
#include <algorithm>//built in algorithms like sorting or searching
#include <cctype>//character checking and conversion
using namespace std;

const double tariffRate = 0.50; // RM per kWh



struct DailyRecord {
    int day;
    double usageHours;
    double temperature;
    double energyKWh;
    double cost;
    string safetyStatus;
};

struct Equipment {
    string id;
    string name;
    double powerRating;
    double maintenanceLimit;
    double totalUsage = 0;
    vector<DailyRecord> records;
};

string toUpperCase(string str) { //make small letter upper case
    for(char &c : str) {
        c = toupper(c);
    }
    return str;
}

int findEquipmentIndex(const vector<Equipment> &list, string id) {
    id = toUpperCase(id);
    for(int i = 0; i < list.size(); i++) {
        if(list[i].id == id)
            return i;
    }
    return -1;
}

void addEquipment(vector<Equipment> &list) { //add equipment

    int num;
    cout << "How many equipment to add?:  ";
    cin >> num;

    for(int i = 0; i < num; i++) {

        cout << "\nEntering Equipment " << i+1 << endl;

        Equipment eq;
        bool validID = false;

        while(!validID) {
            cout << "Enter Equipment ID: ";
            cin >> eq.id;
            eq.id = toUpperCase(eq.id);

            if(findEquipmentIndex(list, eq.id) != -1)
                cout << "Error: ID already exists. Try again.\n";
            else
                validID = true;
        }

        cin.ignore();

        cout << "Enter Equipment Name: ";
        getline(cin, eq.name);

        while(true) { //make sure user enter number instead of letters
            cout << "Enter Power Rating (W): ";
            cin >> eq.powerRating;

            if(cin.fail()) {
                cout << "Invalid input! Please enter a number.\n";
                cin.clear();
                cin.ignore(1000, '\n');
            }
            else {
                break;
            }
        }

        while(true) {
            cout << "Enter Maintenance Limit (Hours): ";
            cin >> eq.maintenanceLimit;

            if(cin.fail()) {
                cout << "Invalid input! Please enter a number.\n";
                cin.clear();
                cin.ignore(1000, '\n');
            }
            else {
                break;
            }
        }

        list.push_back(eq);

        cout << "Equipment added successfully!\n";
    }
}

void viewEquipment(const vector<Equipment> &list) { // To view all equipment

    if(list.empty()) {
        cout << "No equipment available.\n";
        return;
    }

    for(int i = 0; i < list.size(); i++) {

        cout << "\n====== Equipment " << i+1 << " ======\n";
        cout << "ID: " << list[i].id << endl;
        cout << "Name: " << list[i].name << endl;
        cout << "Power: " << list[i].powerRating << " W\n";
        cout << "Maintenance Limit: " << list[i].maintenanceLimit << " hrs\n";
        cout << "Total Usage: " << list[i].totalUsage << " hrs\n";

        if(list[i].totalUsage >= list[i].maintenanceLimit)
            cout << "Maintenance Status: MAINTENANCE DUE\n";
        else
            cout << "Maintenance Status: NOT DUE\n";

        if(list[i].records.empty()) {
            cout << "No daily records available.\n";
        }
        else {
            cout << "\n--- Daily Usage Records ---\n";
            for(int j = 0; j < list[i].records.size(); j++) {
                cout << "Day " << list[i].records[j].day
                     << " | Usage: " << list[i].records[j].usageHours << " hrs"
                     << " | Temp: " << list[i].records[j].temperature << " °C"
                     << " | Energy: " << list[i].records[j].energyKWh << " kWh"
                     << " | Cost: RM " << list[i].records[j].cost
                     << " | Safety: " << list[i].records[j].safetyStatus
                     << endl;
            }
        }
    }
}

void updateEquipment(vector<Equipment> &list) { //update equipment
    string id;
    cout << "Enter Equipment ID to update: ";
    cin >> id;

    int index = findEquipmentIndex(list, id);
    if(index == -1) {
        cout << "Equipment not found.\n";
        return;
    }

    string newID;
    bool validID = false;

    while(!validID) {
        cout << "Enter new Equipment ID: ";
        cin >> newID;
        newID = toUpperCase(newID);

        int checkIndex = findEquipmentIndex(list, newID);

        if(checkIndex != -1 && checkIndex != index) {
            cout << "Error: ID already exists. Try again.\n";
        }
        else {
            validID = true;
        }
    }

    list[index].id = newID;

    // POWER RATING VALIDATION
    while(true) {
        cout << "Enter new power rating (W): ";
        cin >> list[index].powerRating;

        if(cin.fail()) {
            cout << "Invalid input! Please enter a number.\n";
            cin.clear();
            cin.ignore(1000, '\n');
        }
        else {
            break;
        }
    }

    // MAINTENANCE LIMIT VALIDATION
    while(true) {
        cout << "Enter new maintenance limit (Hours): ";
        cin >> list[index].maintenanceLimit;

        if(cin.fail()) {
            cout << "Invalid input! Please enter a number.\n";
            cin.clear();
            cin.ignore(1000, '\n');
        }
        else {
            break;
        }
    }

    cout << "Equipment updated successfully.\n";
}

void deleteEquipment(vector<Equipment> &list) { // DELETE AN EQUIPMENT

    int num;
    cout << "How many equipment do you want to delete? ";
    cin >> num;

    for(int i = 0; i < num; i++) {

        string id;

        cout << "\nEnter Equipment ID to delete: ";
        cin >> id;

        int index = findEquipmentIndex(list, id);

        if(index == -1) {
            cout << "Equipment not found.\n";
        }
        else {
            list.erase(list.begin() + index);
            cout << "Equipment deleted successfully.\n";
        }
    }
}

void dailyUpdate(vector<Equipment> &list) { // DAILY USAGE OF THE EQUIPMENT
    string id;
    cout << "Enter Equipment ID for daily update: ";
    cin >> id;

    int index = findEquipmentIndex(list, id);
    if(index == -1) {
        cout << "Equipment not found.\n";
        return;
    }

    Equipment &eq = list[index];
    DailyRecord record;

    record.day = eq.records.size() + 1;

    cout << "Day " << record.day << " update for " << eq.name << endl;

    cout << "Enter usage hours: ";
    cin >> record.usageHours;

    cout << "Enter temperature (°C): ";
    cin >> record.temperature;

    record.energyKWh = (eq.powerRating / 1000) * record.usageHours;
    record.cost = record.energyKWh * tariffRate;

    if(record.temperature >= 80)
        record.safetyStatus = "OVERHEAT";
    else if(record.temperature >= 60)
        record.safetyStatus = "NEAR-OVERHEAT";
    else
        record.safetyStatus = "NORMAL";

    eq.totalUsage += record.usageHours;
    eq.records.push_back(record);

    cout << "Daily record added successfully!\n";
}

void serviceEquipment(vector<Equipment> &list) { //services equipment
    string id;
    cout << "Enter Equipment ID for service: ";
    cin >> id;

    int index = findEquipmentIndex(list, id);

    if(index == -1) {
        cout << "Equipment not found!\n";
        return;
    }

    // Check if never used
    if(list[index].totalUsage == 0) {
        cout << "Equipment has not been used yet. No need service.\n";
        return;
    }

    // Optional: check if not yet due
    if(list[index].totalUsage < list[index].maintenanceLimit) {
        cout << "Maintenance not due yet.\n";
        cout << "Current usage: " << list[index].totalUsage
             << " / " << list[index].maintenanceLimit << " hrs\n";
        return;
    }

    // Service the equipment
    list[index].totalUsage = 0;

    cout << "Equipment " << list[index].name
         << " has been successfully serviced.\n";
}

void exportToCSV(const vector<Equipment> &list) { // USE TO EXPORT FILE
    ofstream file("SWESMS_Report.csv");

    file << "EquipmentID,Name,Day,UsageHours,Temperature,Energy(kWh),Cost(RM),SafetyStatus\n";

    for(const auto &eq : list) {
        for(const auto &rec : eq.records) {
            file << eq.id << ","
                 << eq.name << ","
                 << rec.day << ","
                 << rec.usageHours << ","
                 << rec.temperature << ","
                 << rec.energyKWh << ","
                 << rec.cost << ","
                 << rec.safetyStatus << "\n";
        }
    }

    file.close();
    cout << "Data exported to SWESMS_Report.csv successfully!\n";
}

int main() {

    vector<Equipment> equipmentList;
    int choice = 0;

    while(choice != 8) {

        cout << "\n========== SWESMS MENU ==========\n";
        cout << "1. Add Equipment\n";
        cout << "2. View Equipment\n";
        cout << "3. Update Equipment\n";
        cout << "4. Delete Equipment\n";
        cout << "5. Daily Update\n";
        cout << "6. Service Equipment\n";
        cout << "7. Export to CSV\n";
        cout << "8. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch(choice) {
            case 1: addEquipment(equipmentList); break;
            case 2: viewEquipment(equipmentList); break;
            case 3: updateEquipment(equipmentList); break;
            case 4: deleteEquipment(equipmentList); break;
            case 5: dailyUpdate(equipmentList); break;
            case 6: serviceEquipment(equipmentList); break;
            case 7: exportToCSV(equipmentList); break;
            case 8: cout << "Exiting SWESMS...\n"; break;
            default: cout << "Invalid choice!\n";
        }
    }

    return 0;
}
