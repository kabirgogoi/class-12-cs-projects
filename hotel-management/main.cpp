// C++ program to implement Hotel Management System using File I/O

#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

struct Customer
{
    int room_no;
    char name[50];
    char address[50];
    char phone[15];
};

class Hotel
{
private:
    static const int MAX_ROOMS = 50;
    char file_name[20] = "hotel_data.dat";

public:
    void main_menu();
    void add();
    void display();
    void rooms_status();
    void edit();
    void delete_record();

    bool room_exists(int room_no);
    void write_customer_to_file(Customer &cust);
    bool read_customer_from_file(int room_no, Customer &cust);
    void delete_customer_from_file(int room_no);
    void update_customer_in_file(Customer &cust);
};

void Hotel::main_menu()
{
    int choice;
    do
    {
        cout << "\n********** HOTEL MANAGEMENT MENU **********\n";
        cout << "1. Book a Room\n";
        cout << "2. Display Customer Details\n";
        cout << "3. Show All Booked Rooms\n";
        cout << "4. Edit Customer Details\n";
        cout << "5. Vacate a Room\n";
        cout << "6. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (cin.fail())
        {
            cout << "Invalid input! Please enter a valid number between 1 and 6.\n";
            exit(1);
        }

        switch (choice)
        {
        case 1:
            add();
            break;
        case 2:
            display();
            break;
        case 3:
            rooms_status();
            break;
        case 4:
            edit();
            break;
        case 5:
            delete_record();
            break;
        case 6:
            cout << "Exiting program...\n";
            break;
        default:
            cout << "Invalid choice! Try again.\n";
        }
    } while (choice != 6);
}

void Hotel::add()
{
    Customer cust;
    cout << "Enter room number to book (1 to " << Hotel::MAX_ROOMS << "): ";
    cin >> cust.room_no;

    if (room_exists(cust.room_no))
    {
        cout << "Sorry, Room " << cust.room_no << " is already booked.\n";
        return;
    }

    cin.ignore();
    cout << "Enter customer name: ";
    cin.getline(cust.name, 50);
    cout << "Enter customer address: ";
    cin.getline(cust.address, 50);
    cout << "Enter customer phone number: ";
    cin.getline(cust.phone, 15);

    write_customer_to_file(cust);
    cout << "Room " << cust.room_no << " has been successfully booked.\n";
}

void Hotel::display()
{
    Customer cust;
    int room_no;
    cout << "Enter room number to view details: ";
    cin >> room_no;

    if (read_customer_from_file(room_no, cust))
    {
        cout << "\nCustomer Details:\n";
        cout << "Room Number: " << cust.room_no << "\n";
        cout << "Name: " << cust.name << "\n";
        cout << "Address: " << cust.address << "\n";
        cout << "Phone: " << cust.phone << "\n";
    }
    else
    {
        cout << "Room " << room_no << " is vacant or does not exist.\n";
    }
}

void Hotel::rooms_status()
{
    ifstream fin(file_name, ios::binary);
    if (!fin)
    {
        cout << "No records found.\n";
        return;
    }

    Customer cust;
    cout << "\nBooked Rooms:\n";
    cout << "Room No.\tName\n";

    while (fin.read((char *)&cust, sizeof(Customer)))
    {
        cout << cust.room_no << "\t\t" << cust.name << "\n";
    }

    fin.close();
}

void Hotel::edit()
{
    Customer cust;
    int room_no;
    cout << "Enter room number to edit details: ";
    cin >> room_no;

    if (read_customer_from_file(room_no, cust))
    {
        cin.ignore(); // Clear the input buffer
        cout << "Enter new name: ";
        cin.getline(cust.name, 30);
        cout << "Enter new address: ";
        cin.getline(cust.address, 50);
        cout << "Enter new phone number: ";
        cin.getline(cust.phone, 15);

        update_customer_in_file(cust);
        cout << "Room " << room_no << " details updated successfully.\n";
    }
    else
    {
        cout << "Room " << room_no << " is vacant or does not exist.\n";
    }
}

void Hotel::delete_record()
{
    int room_no;
    cout << "Enter room number to vacate: ";
    cin >> room_no;

    if (room_exists(room_no))
    {
        delete_customer_from_file(room_no);
        cout << "Room " << room_no << " is now vacant.\n";
    }
    else
    {
        cout << "Room " << room_no << " is already vacant or does not exist.\n";
    }
}

bool Hotel::room_exists(int room_no)
{
    Customer cust;
    return read_customer_from_file(room_no, cust);
}

void Hotel::write_customer_to_file(Customer &cust)
{
    ofstream fout(file_name, ios::binary | ios::app);
    fout.write((char *)&cust, sizeof(Customer));
    fout.close();
}

bool Hotel::read_customer_from_file(int room_no, Customer &cust)
{
    ifstream fin(file_name, ios::binary);
    if (!fin)
        return false;

    while (fin.read((char *)&cust, sizeof(Customer)))
    {
        if (cust.room_no == room_no)
        {
            fin.close();
            return true;
        }
    }

    fin.close();
    return false;
}

void Hotel::delete_customer_from_file(int room_no)
{
    ifstream fin(file_name, ios::binary);
    ofstream fout("temp.dat", ios::binary);

    Customer cust;
    while (fin.read((char *)&cust, sizeof(Customer)))
    {
        if (cust.room_no != room_no)
        {
            fout.write((char *)&cust, sizeof(Customer));
        }
    }

    fin.close();
    fout.close();

    remove(file_name);
    rename("temp.dat", file_name);
}

void Hotel::update_customer_in_file(Customer &cust)
{
    fstream file(file_name, ios::binary | ios::in | ios::out);
    Customer temp;

    while (file.read((char *)&temp, sizeof(Customer)))
    {
        if (temp.room_no == cust.room_no)
        {
            file.seekp(-static_cast<int>(sizeof(Customer)), ios::cur);
            file.write((char *)&cust, sizeof(Customer));
            break;
        }
    }

    file.close();
}

int main()
{
    Hotel hotel;
    hotel.main_menu();
    return 0;
}
