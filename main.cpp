
#include <iostream>
#include <fstream>
#include <string>
#include <limits>
using namespace std;

class Item {
private:
    int itemId;
    string itemName;
    string location;
    string status;

public:
    Item() {
        itemId = 0;
        itemName = "";
        location = "";
        status = "";
    }

    Item(int id, string name, string loc, string st) {
        itemId = id;
        itemName = name;
        location = loc;
        status = st;
    }

    void inputItem(int id) {
        itemId = id;

        cout << "Enter item name: ";
        getline(cin >> ws, itemName);

        cout << "Enter location: ";
        getline(cin, location);

        cout << "Enter status (Lost/Found): ";
        getline(cin, status);
    }

    void displayItem() const {
        cout << "\n--------------------------------";
        cout << "\nItem ID: " << itemId;
        cout << "\nItem Name: " << itemName;
        cout << "\nLocation: " << location;
        cout << "\nStatus: " << status;
        cout << "\n--------------------------------\n";
    }

    int getItemId() const {
        return itemId;
    }

    string getItemName() const {
        return itemName;
    }

    void updateDetails() {
        cout << "Enter new item name: ";
        getline(cin >> ws, itemName);

        cout << "Enter new location: ";
        getline(cin, location);

        cout << "Enter new status (Lost/Found/Claimed/Returned): ";
        getline(cin, status);
    }

    void updateStatus() {
        cout << "Enter new status (Lost/Found/Claimed/Returned): ";
        getline(cin >> ws, status);
    }

    void saveToFile(ofstream &file) const {
        file << itemId << '\n';
        file << itemName << '\n';
        file << location << '\n';
        file << status << '\n';
    }

    bool loadFromFile(ifstream &file) {
        if (!(file >> itemId))
            return false;

        file.ignore(numeric_limits<streamsize>::max(), '\n');

        if (!getline(file, itemName))
            return false;

        if (!getline(file, location))
            return false;

        if (!getline(file, status))
            return false;

        return true;
    }
};

class LostAndFound {
private:
    Item items[100];
    int count;
    int nextId;
    const string fileName = "items.txt";

public:
    LostAndFound() {
        count = 0;
        nextId = 101;
        loadItems();
    }

    void loadItems() {
        ifstream file(fileName);

        if (!file) {
            return;
        }

        Item temp;

        while (count < 100 && temp.loadFromFile(file)) {
            items[count] = temp;

            if (temp.getItemId() >= nextId)
                nextId = temp.getItemId() + 1;

            count++;
        }

        file.close();
    }

    void saveItems() {
        ofstream file(fileName);

        if (!file) {
            cout << "\nError opening file!\n";
            return;
        }

        for (int i = 0; i < count; i++) {
            items[i].saveToFile(file);
        }

        file.close();
    }

    void reportItem() {
        if (count >= 100) {
            cout << "\nItem limit reached!\n";
            return;
        }

        cout << "\nEnter item details:\n";

        items[count].inputItem(nextId);
        count++;
        nextId++;

        saveItems();

        cout << "\nItem reported successfully!\n";
    }

    void viewItems() const {
        if (count == 0) {
            cout << "\nNo items available.\n";
            return;
        }

        cout << "\n========== REPORTED ITEMS ==========\n";

        for (int i = 0; i < count; i++) {
            items[i].displayItem();
        }
    }

    void searchItem() const {
        string name;
        bool found = false;

        cout << "\nEnter item name to search: ";
        getline(cin >> ws, name);

        for (int i = 0; i < count; i++) {
            if (items[i].getItemName() == name) {
                items[i].displayItem();
                found = true;
            }
        }

        if (!found) {
            cout << "\nItem not found!\n";
        }
    }

    void modifyItem() {
        int id;
        bool found = false;

        cout << "\nEnter Item ID to modify: ";
        cin >> id;

        for (int i = 0; i < count; i++) {
            if (items[i].getItemId() == id) {
                items[i].updateDetails();
                saveItems();

                cout << "\nItem details updated successfully!\n";
                found = true;
                break;
            }
        }

        if (!found)
            cout << "\nItem ID not found!\n";
    }

    void updateItemStatus() {
        int id;
        bool found = false;

        cout << "\nEnter Item ID: ";
        cin >> id;

        for (int i = 0; i < count; i++) {
            if (items[i].getItemId() == id) {
                items[i].updateStatus();
                saveItems();

                cout << "\nItem status updated successfully!\n";
                found = true;
                break;
            }
        }

        if (!found)
            cout << "\nItem ID not found!\n";
    }

    void deleteItem() {
        int id;
        bool found = false;

        cout << "\nEnter Item ID to delete: ";
        cin >> id;

        for (int i = 0; i < count; i++) {
            if (items[i].getItemId() == id) {

                for (int j = i; j < count - 1; j++) {
                    items[j] = items[j + 1];
                }

                count--;
                saveItems();

                cout << "\nItem deleted successfully!\n";
                found = true;
                break;
            }
        }

        if (!found)
            cout << "\nItem ID not found!\n";
    }

    void showMenu() {
        int choice;

        do {
            cout << "\n========================================\n";
            cout << "       COLLEGE LOST & FOUND SYSTEM\n";
            cout << "========================================\n";
            cout << "1. Report Lost/Found Item\n";
            cout << "2. View All Items\n";
            cout << "3. Search Item\n";
            cout << "4. Modify Item Details\n";
            cout << "5. Update Item Status\n";
            cout << "6. Delete Item\n";
            cout << "7. Exit\n";
            cout << "========================================\n";
            cout << "Enter your choice: ";

            if (!(cin >> choice)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "\nInvalid input!\n";
                continue;
            }

            switch (choice) {
            case 1:
                reportItem();
                break;

            case 2:
                viewItems();
                break;

            case 3:
                searchItem();
                break;

            case 4:
                modifyItem();
                break;

            case 5:
                updateItemStatus();
                break;

            case 6:
                deleteItem();
                break;

            case 7:
                cout << "\nThank you for using the system!\n";
                break;

            default:
                cout << "\nInvalid choice! Try again.\n";
            }

        } while (choice != 7);
    }
};

int main() {
    LostAndFound system;

    cout << "\nWelcome to College Lost & Found Management System!\n";

    system.showMenu();

    return 0;
}
