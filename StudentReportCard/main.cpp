#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <algorithm>

using namespace std;

class Student {
public:
    int rollNo;
    string name;
    vector<float> marks;
    float average;
    char grade;

    Student() {}

    Student(int r, string n, vector<float> m) {
        rollNo = r;
        name = n;
        marks = m;
        calculateAverage();
        assignGrade();
    }

    void calculateAverage() {
        float sum = 0;
        for (float mark : marks)
            sum += mark;
        average = sum / marks.size();
    }

    void assignGrade() {
        if (average >= 90)
            grade = 'A';
        else if (average >= 75)
            grade = 'B';
        else if (average >= 60)
            grade = 'C';
        else if (average >= 40)
            grade = 'D';
        else
            grade = 'F';
    }

    void display() {
        cout << setw(10) << rollNo
             << setw(20) << name
             << setw(10) << fixed << setprecision(2) << average
             << setw(10) << grade << endl;
    }
};

class ReportCardManager {
private:
    vector<Student> students;
    const string filename = "students.dat";

public:
    ReportCardManager() {
        loadFromFile();
    }

    ~ReportCardManager() {
        saveToFile();
    }

    void loadFromFile() {
        ifstream inFile(filename, ios::binary);
        if (inFile) {
            int size;
            inFile.read((char*)&size, sizeof(size));
            for (int i = 0; i < size; ++i) {
                Student s;
                int roll;
                string name;
                int markCount;
                vector<float> marks;

                inFile.read((char*)&roll, sizeof(roll));
                int len;
                inFile.read((char*)&len, sizeof(len));
                name.resize(len);
                inFile.read(&name[0], len);

                inFile.read((char*)&markCount, sizeof(markCount));
                marks.resize(markCount);
                inFile.read((char*)marks.data(), sizeof(float) * markCount);

                s = Student(roll, name, marks);
                students.push_back(s);
            }
        }
        inFile.close();
    }

    void saveToFile() {
        ofstream outFile(filename, ios::binary | ios::trunc);
        int size = students.size();
        outFile.write((char*)&size, sizeof(size));
        for (Student& s : students) {
            outFile.write((char*)&s.rollNo, sizeof(s.rollNo));
            int len = s.name.length();
            outFile.write((char*)&len, sizeof(len));
            outFile.write(s.name.c_str(), len);

            int markCount = s.marks.size();
            outFile.write((char*)&markCount, sizeof(markCount));
            outFile.write((char*)s.marks.data(), sizeof(float) * markCount);
        }
        outFile.close();
    }

    void addStudent() {
        int roll;
        string name;
        int subjectCount;
        vector<float> marks;

        cout << "Enter Roll No: ";
        cin >> roll;
        cin.ignore();
        cout << "Enter Name: ";
        getline(cin, name);
        cout << "Enter number of subjects: ";
        cin >> subjectCount;

        for (int i = 0; i < subjectCount; ++i) {
            float mark;
            cout << "Enter mark for subject " << i + 1 << ": ";
            cin >> mark;
            marks.push_back(mark);
        }

        students.push_back(Student(roll, name, marks));
        cout << "Student added successfully!\n";
    }

    void displayAll() {
        cout << left << setw(10) << "Roll No"
             << setw(20) << "Name"
             << setw(10) << "Average"
             << setw(10) << "Grade" << endl;
        cout << string(50, '-') << endl;
        for (Student& s : students)
            s.display();
    }

    void searchStudent() {
        int roll;
        cout << "Enter Roll No to search: ";
        cin >> roll;
        auto it = find_if(students.begin(), students.end(),
                          [roll](Student& s) { return s.rollNo == roll; });
        if (it != students.end()) {
            it->display();
        } else {
            cout << "Student not found.\n";
        }
    }

    void menu() {
        int choice;
        do {
            cout << "\n--- Student Report Card Management ---\n";
            cout << "1. Add Student\n";
            cout << "2. Display All Students\n";
            cout << "3. Search Student by Roll No\n";
            cout << "4. Exit\n";
            cout << "Enter choice: ";
            cin >> choice;

            switch (choice) {
                case 1: addStudent(); break;
                case 2: displayAll(); break;
                case 3: searchStudent(); break;
                case 4: cout << "Exiting...\n"; break;
                default: cout << "Invalid choice!\n";
            }
        } while (choice != 4);
    }
};

int main() {
    ReportCardManager manager;
    manager.menu();
    return 0;
}
