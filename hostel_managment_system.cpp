#include <iostream>
#include <string>
using namespace std;

class Student {
public:
    int id;
    string name;
    string roomNumber;
    string contact;
    double feeStatus;
    Student* next;

    Student(int id, string name, string roomNumber, string contact) {
        this->id = id;
        this->name = name;
        this->roomNumber = roomNumber;
        this->contact = contact;
        this->feeStatus = 0.0;
        this->next = nullptr;       
    } 
      
    void display()  {
        cout << "ID: " << id << endl;
        cout << "Name: " << name << endl;
        cout << "Room Number: " << roomNumber << endl;
        cout << "Contact: " << contact << endl;
        cout << "Fee Status: " << feeStatus << "\n";
    }
};

class QueueNode {
public:
    Student* student;
    QueueNode* next;

    QueueNode(Student* student) : student(student), next(nullptr) {}
};

class StackNode {
public:
    Student* student;
    StackNode* next;

    StackNode(Student* student) : student(student), next(nullptr) {}
};

// Stack class for undoing the last student added
class Stack {
public:
    StackNode* top;

    Stack() : top(nullptr) {}

    void push(Student* student) {
        StackNode* newNode = new StackNode(student);
        newNode->next = top;
        top = newNode;
    }

    Student* pop() {
        if (top == nullptr) return nullptr;
        StackNode* temp = top;
        top = top->next;
        Student* student = temp->student;
        delete temp;
        return student;
    }

    bool isEmpty() const {
        return top == nullptr;
    }

    ~Stack() {
        while (!isEmpty()) {
            pop();
        }
    }
};

// Queue class for managing students waiting for room assignments
class Queue {
public:
    QueueNode* front;
    QueueNode* rear;

    Queue() : front(nullptr), rear(nullptr) {}

    void enqueue(Student* student) {
        QueueNode* newNode = new QueueNode(student);
        if (rear == nullptr) {
            front = rear = newNode;
        } else {
            rear->next = newNode;
            rear = newNode;
        }
    }

    Student* dequeue() {
        if (front == nullptr) return nullptr;
        QueueNode* temp = front;
        front = front->next;
        if (front == nullptr) rear = nullptr;
        Student* student = temp->student;
        delete temp;
        return student;
    }

    bool isEmpty() const {
        return front == nullptr;
    }

    ~Queue() {
        while (!isEmpty()) {
            dequeue();
        }
    }
};

class FeeManagement {
public:
    void payFee(Student& student, double amount) {
        student.feeStatus += amount;
        cout << "Fee of " << amount << " paid successfully for Student ID " << student.id << ".\n";
    }

    void updateFee(Student& student, double newAmount) {
        student.feeStatus = newAmount;
        cout << "Fee updated successfully for Student ID " << student.id << ". New Fee: " << newAmount << "\n";
    }

    void displayFeeStatus(const Student& student) const {
        cout << "Fee Status for Student ID " << student.id << ": " << student.feeStatus << "\n";
    }
};

class HostelManagementSystem {
private:
    Student* head;
    FeeManagement feeManagement;
    Stack studentStack;  // Stack to undo last added student
    Queue studentQueue;  // Queue for room assignment or fee payment
    const int maxRooms = 20;

public:
    HostelManagementSystem() : head(nullptr) {}

    void addStudent() {
        int id;
        string name, roomNumber, contact;

        cout << "Enter Student ID: ";
        cin >> id;
        cin.ignore();

        // Check for duplicate ID
        if (searchStudentById(id)) {
            cout << "Student ID already exists. Please enter a unique ID.\n";
            return;
        }

        cout << "Enter Student Name: ";
        getline(cin, name);

        cout << "Enter Room Number (1-20): ";
        getline(cin, roomNumber);

        // Validate room number
        if (stoi(roomNumber) < 1 || stoi(roomNumber) > maxRooms) {
            cout << "Error: Room number must be between 1 and " << maxRooms << ".\n";
            return;
        }

        cout << "Enter Contact Information: ";
        getline(cin, contact);

        Student* newStudent = new Student(id, name, roomNumber, contact);
        newStudent->next = head;
        head = newStudent;

        studentStack.push(newStudent);  // Push new student onto the stack
        studentQueue.enqueue(newStudent);  // Enqueue new student for room/fee processing

        cout << "Student added successfully!\n";
    }

    bool searchStudentById(int id) const {
        Student* current = head;
        while (current != nullptr) {
            if (current->id == id) {
                return true; // Student found
            }
            current = current->next;
        }
        return false; // Student not found
    }

    void deleteStudent() {
        int id;
        cout << "Enter Student ID to delete: ";
        cin >> id;

        Student* current = head;
        Student* previous = nullptr;

        while (current != nullptr && current->id != id) {
            previous = current;
            current = current->next;
        }

        if (current == nullptr) {
            cout << "Student not found.\n";
            return;
        }

        if (previous == nullptr) {
            head = current->next;
        } else {
            previous->next = current->next;
        }

        delete current;
        cout << "Student with ID " << id << " deleted successfully.\n";
    }

    void displayAllStudents() const {
        if (head == nullptr) {
            cout << "No students found.\n";
            return;
        }

        cout << "\n--- List of Students ---\n";
        Student* current = head;
        while (current != nullptr) {
            current->display();
            cout << "-------------------------\n";
            current = current->next;
        }
    }

    void searchStudentById() const {
        int id;
        cout << "Enter Student ID to search: ";
        cin >> id;

        Student* current = head;
        while (current != nullptr) {
            if (current->id == id) {
                current->display();
                return;
            }
            current = current->next;
        }

        cout << "Student not found.\n";
    }

    void payFeeForStudent() {
        int id;
        double amount;
        cout << "Enter Student ID to pay fee: ";
        cin >> id;
        cout << "Enter amount to pay: ";
        cin >> amount;

        Student* current = head;
        while (current != nullptr) {
            if (current->id == id) {
                feeManagement.payFee(*current, amount);
                return;
            }
            current = current->next;
        }
        cout << "Student not found.\n";
    }

    void updateFeeForStudent() {
        int id;
        double newAmount;
        cout << "Enter Student ID to update fee: ";
        cin >> id;
        cout << "Enter new fee amount: ";
        cin >> newAmount;

        Student* current = head;
        while (current != nullptr) {
            if (current->id == id) {
                feeManagement.updateFee(*current, newAmount);
                return;
            }
            current = current->next;
        }
        cout << "Student not found.\n";
    }

    void displayFeeStatusForStudent() const {
        int id;
        cout << "Enter Student ID to check fee status: ";
        cin >> id;

        Student* current = head;
        while (current != nullptr) {
            if (current->id == id) {
                feeManagement.displayFeeStatus(*current);
                return;
            }
            current = current->next;
        }
        cout << "Student not found.\n";
    }

    void undoAddStudent() {
        if (studentStack.isEmpty()) {
            cout << "No student to undo.\n";
            return;
        }
        Student* lastAddedStudent = studentStack.pop();

        // Remove student from linked list
        if (head == lastAddedStudent) {
            head = head->next;
        } else {
            Student* current = head;
            while (current != nullptr && current->next != lastAddedStudent) {
                current = current->next;
            }
            if (current != nullptr) {
                current->next = lastAddedStudent->next;
            }
        }

        delete lastAddedStudent;
        cout << "Last student addition undone.\n";
    }

    void menu() {
        int choice;
        while (true) {
            cout << "\n--------------------------- Hostel Management System Menu -----------------------------\n";
            cout << "1. Add Student\n";
            cout << "2. Display All Students\n";
            cout << "3. Search Student by ID\n";
            cout << "4. Delete Student\n";
            cout << "5. Pay Fee for Student\n";
            cout << "6. Update Fee for Student\n";
            cout << "7. Display Fee Status for Student\n";
            cout << "8. Undo Last Add Student\n";
            cout << "9. Exit\n";
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
                case 1:
                    addStudent();
                    break;
                case 2:
                    displayAllStudents();
                    break;
                case 3:
                    searchStudentById();
                    break;
                case 4:
                    deleteStudent();
                    break;
                case 5:
                    payFeeForStudent();
                    break;
                case 6:
                    updateFeeForStudent();
                    break;
                case 7:
                    displayFeeStatusForStudent();
                    break;
                case 8:
                    undoAddStudent();
                    break;
                case 9:
                    cout << "Exiting...\n";
                    return;
                default:
                    cout << "Invalid choice. Please try again.\n";
            }
        }
    }

    ~HostelManagementSystem() {
        while (head != nullptr) {
            Student* temp = head;
            head = head->next;
            delete temp;
        }
    }
};

int main() {
    HostelManagementSystem hms;
    hms.menu();
    return 0;
}
