#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;

const int MAX_STUDENTS = 100;

// Student structure
struct Student
{
    char name[50];
    char class_name[20];
    char stream[20];
    int roll_number;
    int admission_year;
};

// Class for Student Management System
class StudentManagement
{
private:
    const char *file_name = "students.dat"; // File to store student data

public:
    void add_student();
    void display_student(int roll_number);
    void display_all_students();
    void update_student(int roll_number);
    void delete_student(int roll_number);
    void main_menu();
    bool read_student_from_file(int roll_number, Student &student);
    void write_student_to_file(Student &student);
    void update_student_in_file(Student &student);
};

// Function to add a student
void StudentManagement::add_student()
{
    Student student;
    cout << "\nEnter student name: ";
    cin.ignore();
    cin.getline(student.name, 50);

    cout << "Enter student class: ";
    cin.getline(student.class_name, 20);

    cout << "Enter student admission year: ";
    cin >> student.admission_year;

    cout << "Enter student roll number: ";
    cin >> student.roll_number;

    cout << "Enter student stream (Science/Arts): ";
    cin.ignore();
    cin.getline(student.stream, 20);

    write_student_to_file(student);
    cout << "Student added successfully!\n";
}

// Function to write a student to file
void StudentManagement::write_student_to_file(Student &student)
{
    ofstream fout(file_name, ios::binary | ios::app);
    fout.write((char *)&student, sizeof(Student));
    fout.close();
}

// Function to display student details
void StudentManagement::display_student(int roll_number)
{
    Student student;
    if (read_student_from_file(roll_number, student))
    {
        cout << "\nStudent Details:\n";
        cout << "Name: " << student.name << endl;
        cout << "Class: " << student.class_name << endl;
        cout << "Admission Year: " << student.admission_year << endl;
        cout << "Roll Number: " << student.roll_number << endl;
        cout << "Stream: " << student.stream << endl;
    }
    else
    {
        cout << "Student not found!\n";
    }
}

// Function to read a student from file based on roll number
bool StudentManagement::read_student_from_file(int roll_number, Student &student)
{
    ifstream fin(file_name, ios::binary);
    if (!fin)
        return false;

    while (fin.read((char *)&student, sizeof(Student)))
    {
        if (student.roll_number == roll_number)
        {
            fin.close();
            return true;
        }
    }

    fin.close();
    return false;
}

// Function to display all students
void StudentManagement::display_all_students()
{
    ifstream fin(file_name, ios::binary);
    Student student;

    cout << "\nAll Students Details:\n";
    while (fin.read((char *)&student, sizeof(Student)))
    {
        cout << "Name: " << student.name << ", ";
        cout << "Class: " << student.class_name << ", ";
        cout << "Admission Year: " << student.admission_year << ", ";
        cout << "Roll Number: " << student.roll_number << ", ";
        cout << "Stream: " << student.stream << endl;
    }

    fin.close();
}

// Function to update student details
void StudentManagement::update_student(int roll_number)
{
    Student student;
    if (read_student_from_file(roll_number, student))
    {
        cout << "\nEnter new details for student " << roll_number << ":\n";
        cout << "Enter new name: ";
        cin.ignore();
        cin.getline(student.name, 50);
        cout << "Enter new class: ";
        cin.getline(student.class_name, 20);
        cout << "Enter new admission year: ";
        cin >> student.admission_year;
        cout << "Enter new stream (Science/Arts): ";
        cin.ignore();
        cin.getline(student.stream, 20);

        update_student_in_file(student);
        cout << "Student details updated successfully!\n";
    }
    else
    {
        cout << "Student not found!\n";
    }
}

// Function to update student data in file
void StudentManagement::update_student_in_file(Student &student)
{
    fstream file(file_name, ios::binary | ios::in | ios::out);
    Student temp;

    while (file.read((char *)&temp, sizeof(Student)))
    {
        if (temp.roll_number == student.roll_number)
        {
            file.seekp(-static_cast<int>(sizeof(Student)), ios::cur);
            file.write((char *)&student, sizeof(Student));
            break;
        }
    }

    file.close();
}

// Function to delete a student
void StudentManagement::delete_student(int roll_number)
{
    ifstream fin(file_name, ios::binary);
    ofstream fout("temp.dat", ios::binary); // Temporary file to store the data

    Student student;
    bool found = false;
    while (fin.read((char *)&student, sizeof(Student)))
    {
        if (student.roll_number == roll_number)
        {
            found = true;
        }
        else
        {
            fout.write((char *)&student, sizeof(Student));
        }
    }

    fin.close();
    fout.close();

    remove(file_name);             // Delete original file
    rename("temp.dat", file_name); // Rename temp file to original file name

    if (found)
    {
        cout << "Student deleted successfully!\n";
    }
    else
    {
        cout << "Student not found!\n";
    }
}

// Function to show the main menu
void StudentManagement::main_menu()
{
    int choice;
    do
    {
        cout << "\n********** STUDENT MANAGEMENT MENU **********\n";
        cout << "1. Add a Student\n";
        cout << "2. Display Student Details\n";
        cout << "3. Display All Students\n";
        cout << "4. Update Student Details\n";
        cout << "5. Delete a Student\n";
        cout << "6. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            add_student();
            break;
        case 2:
            int roll_number;
            cout << "Enter roll number to display: ";
            cin >> roll_number;
            display_student(roll_number);
            break;
        case 3:
            display_all_students();
            break;
        case 4:
            cout << "Enter roll number to update: ";
            cin >> roll_number;
            update_student(roll_number);
            break;
        case 5:
            cout << "Enter roll number to delete: ";
            cin >> roll_number;
            delete_student(roll_number);
            break;
        case 6:
            cout << "Exiting program...\n";
            break;
        default:
            cout << "Invalid choice! Try again.\n";
        }
    } while (choice != 6);
}

int main()
{
    StudentManagement sm;
    sm.main_menu();
    return 0;
}
