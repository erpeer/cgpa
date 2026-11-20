#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <cctype>
#include "student.h"
#include "course.h"

// Mengubah huruf nilai menjadi kapital (misal "ab" -> "AB")
std::string toUpperCase(std::string text){
    for(char& ch : text){
        ch = std::toupper(static_cast<unsigned char>(ch));
    }
    return text;
}

// Mengecek apakah nilai huruf sesuai dengan letter_convert.txt
bool isValidGrade(const std::string& grade){
    return grade == "A" || grade == "AB" || grade == "B" ||
           grade == "BC" || grade == "C" || grade == "D" || grade == "E";
}

// Menyimpan transkrip ke file .txt (rapi dibuka di Notepad)
void saveTranscriptTxt(const Student& student, const std::string& filename){
    std::ofstream outFile(filename);
    if(!outFile.is_open()){
        std::cout << "Failed to create " << filename << "\n";
        return;
    }

    outFile << "Student Name : " << student.getName() << "\n"
            << "Student ID   : " << student.getId()   << "\n"
            << "----------------------------------------------------------------------------------------\n"
            << std::left 
            << std::setw(10)  << "Code"
            << std::setw(38)  << "Course Name"
            << std::setw(13)  << "Credit"
            << std::setw(13)  << "Grade"
            << std::setw(13)  << "Points" << "\n"
            << "----------------------------------------------------------------------------------------\n";

    for (const Course& course : student.getCourseList()) {
        outFile << std::left 
                << std::setw(10) << course.getCourseCode()
                << std::setw(38) << course.getCourseName()
                << std::setw(13) << course.getCredit()
                << std::setw(13) << course.getLetterGrade()
                << std::fixed    << std::setprecision(2) << course.gradePoints() << "\n";
    }

    outFile << "----------------------------------------------------------------------------------------\n"
            << "Total Credits : " << student.getTotalCredits() << "\n"
            << "CGPA          : " << std::fixed << std::setprecision(2) << student.calculateCGPA() << "\n";

    outFile.close();
    std::cout << "Transcript saved to " << filename << "\n";
}

// Menyimpan transkrip ke file .csv (opsional, mudah dibuka di Excel/Sheets)
void saveTranscriptCsv(const Student& student, const std::string& filename) {
    std::ofstream outFile(filename);
    if (!outFile.is_open()) {
        std::cout << "Failed to create " << filename << "\n";
        return;
    }

    outFile << "Student Name," << student.getName() << "\n";
    outFile << "Student ID," << student.getId() << "\n\n";
    outFile << "Code,Course Name,Credit,Grade,Points\n";

    for (const Course& course : student.getCourseList()) {
        outFile << course.getCourseCode() << ","
                << "\"" << course.getCourseName() << "\","
                << course.getCredit() << ","
                << course.getLetterGrade() << ","
                << std::fixed << std::setprecision(2) << course.gradePoints() << "\n";
    }

    outFile << "\nTotal Credits," << student.getTotalCredits() << "\n";
    outFile << "CGPA," << std::fixed << std::setprecision(2) << student.calculateCGPA() << "\n";

    outFile.close();
    std::cout << "Transcript saved to " << filename << "\n";
}

int main() {
    std::string studentId;
    std::string studentName;

    std::cout << "=== Student Profile ===\n";
    std::cout << "Enter Student ID   : ";
    std::getline(std::cin >> std::ws, studentId);

    std::cout << "Enter Student Name : ";
    std::getline(std::cin >> std::ws, studentName);

    Student student(studentId, studentName);

    int choice = 0;
    do {
        std::cout << "\n=== GPA / CGPA Menu ===\n";
        std::cout << "1. Add Course\n";
        std::cout << "2. Sort Courses by Grade\n";
        std::cout << "3. Search Course by Code\n";
        std::cout << "4. Print Transcript (Console)\n";
        std::cout << "5. Save Transcript (.txt for Notepad)\n";
        std::cout << "6. Save Transcript (.csv for Excel)\n";
        std::cout << "0. Exit\n";
        std::cout << "Choose an option: ";
        std::cin  >> choice;

        if(choice == 1){
            std::string courseCode, courseName, letterGrade;
            int credit;

            std::cout << "Course Code : ";
            std::getline(std::cin >> std::ws, courseCode);

            std::cout << "Course Name : ";
            std::getline(std::cin >> std::ws, courseName);

            do {
                std::cout << "Letter Grade (A, AB, B, BC, C, D, E): ";
                std::getline(std::cin >> std::ws, letterGrade);
                letterGrade = toUpperCase(letterGrade);
                if(!isValidGrade(letterGrade)){
                    std::cout << "Invalid grade! Please check letter_convert.txt.\n";
                }
            } while(!isValidGrade(letterGrade));

            do {
                std::cout << "Credit      : ";
                std::cin >> credit;
                if (credit <= 0) {
                    std::cout << "Credit must be greater than 0.\n";
                }
            } while(credit <= 0);

            student.addCourse(Course(courseCode, courseName, letterGrade, credit));
            std::cout << "Course added!\n";
        }
        else if(choice == 2){
            student.sortCourseGrade();
            std::cout << "Courses sorted from highest to lowest grade.\n";
        }
        else if(choice == 3){
            std::string courseCode;
            std::cout << "Enter Course Code to search: ";
            std::getline(std::cin >> std::ws, courseCode);

            int index = student.searchCourseCode(courseCode);
            if(index != -1){
                const Course& found = student.getCourseList()[index];
                std::cout << "Found: " << found.getCourseCode() << " - "
                          << found.getCourseName() << " | Credit: " << found.getCredit()
                          << " | Grade: " << found.getLetterGrade() << "\n";
            } 
            
            else {
                std::cout << "Course not found.\n";
            }
        }
        else if(choice == 4){
            std::cout << "\n";
            student.printTranscript();
        }
        else if(choice == 5){
            saveTranscriptTxt(student, "transcript_" + student.getId() + ".txt");
        }
        else if(choice == 6){
            saveTranscriptCsv(student, "transcript_" + student.getId() + ".csv");
        }
        else if(choice != 0){
            std::cout << "Invalid choice.\n";
        }

    } while(choice != 0);

    return 0;
}