#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
#include <sstream>
#include <cctype>
#include "student.h"
#include "course.h"

// Convert a grade string to uppercase (e.g., "ab" -> "AB")
std::string toUpperCase(std::string text){
    for(char& ch : text){
        ch = std::toupper(static_cast<unsigned char>(ch));
    }
    return text;
}

// Check whether the letter grade matches letter_convert.txt
bool isValidGrade(const std::string& grade){
    return grade == "A" || grade == "AB" || grade == "B" ||
           grade == "BC" || grade == "C" || grade == "D" || grade == "E";
}

// Trim leading and trailing whitespace characters
std::string trimText(std::string s){
    while(!s.empty() && std::isspace(static_cast<unsigned char>(s.front()))){
        s.erase(s.begin());
    }

    while(!s.empty() && std::isspace(static_cast<unsigned char>(s.back()))){
        s.pop_back();
    }

    return s;
}

// Load existing courses from transcript_<ID>.txt if the file exists
bool loadExistingTxt(const std::string& filename, Student& student){
    std::ifstream inFile(filename);
    if(!inFile.is_open()){
        return false;
    }

    std::string line;
    bool inCourses = false;
    while(std::getline(inFile, line)){
        if(!line.empty() && line.back() == '\r'){
            line.pop_back();
        }

        if(line.find("Code") == 0 && line.find("Course Name") != std::string::npos){
            inCourses = true;
            continue;
        }

        if(!inCourses){
            continue;
        }

        if(line.empty() || line[0] == '-'){
            if(!student.getCourseList().empty()){
                break;
            }
            continue;
        }

        if(line.size() >= 61){
            std::string code = trimText(line.substr(0, 10));
            std::string name = trimText(line.substr(10, 38));
            std::string credStr = trimText(line.substr(48, 13));
            std::string grade = toUpperCase(trimText(line.substr(61, 13)));

            int credit = 0;
            try {
                credit = std::stoi(credStr);
            } 
            
            catch(...) {
                continue;
            }

            if(!code.empty() && !name.empty() && credit > 0 && isValidGrade(grade)){
                if(student.searchCourseCode(code) == -1){
                    student.addCourse(Course(code, name, grade, credit));
                }
            }
        }
    }
    return true;
}

// Load existing courses from transcript_<ID>.csv if the file exists
bool loadExistingCsv(const std::string& filename, Student& student){
    std::ifstream inFile(filename);
    if(!inFile.is_open()){
        return false;
    }

    std::string line;
    bool inCourses = false;
    while(std::getline(inFile, line)){
        if(!line.empty() && line.back() == '\r'){
            line.pop_back();
        }

        if(line == "Code,Course Name,Credit,Grade,Points"){
            inCourses = true;
            continue;
        }

        if(!inCourses){
            continue;
        }

        if(line.empty() || line.find("Total Credits,") == 0 || line.find("CGPA,") == 0){
            break;
        }

        size_t c1 = line.find(',');
        if(c1 == std::string::npos) continue;
        std::string code = trimText(line.substr(0, c1));

        size_t pos = c1 + 1;
        std::string name;
        if(pos < line.size() && line[pos] == '"'){
            size_t endQuote = line.find('"', pos + 1);
            if(endQuote == std::string::npos) continue;
            name = trimText(line.substr(pos + 1, endQuote - (pos + 1)));
            pos = endQuote + 1;
            if(pos < line.size() && line[pos] == ',') ++pos;
        } 
        
        else {
            size_t c2 = line.find(',', pos);
            if(c2 == std::string::npos) continue;
            name = trimText(line.substr(pos, c2 - pos));
            pos = c2 + 1;
        }

        size_t c3 = line.find(',', pos);
        if(c3 == std::string::npos) continue;
        std::string credStr = trimText(line.substr(pos, c3 - pos));

        size_t c4 = line.find(',', c3 + 1);
        std::string grade = (c4 == std::string::npos)
                          ? toUpperCase(trimText(line.substr(c3 + 1)))
                          : toUpperCase(trimText(line.substr(c3 + 1, c4 - (c3 + 1))));

        int credit = 0;
        try {
            credit = std::stoi(credStr);
        } 
        
        catch(...) {
            continue;
        }

        if(!code.empty() && !name.empty() && credit > 0 && isValidGrade(grade)){
            if(student.searchCourseCode(code) == -1){
                student.addCourse(Course(code, name, grade, credit));
            }
        }
    }
    return true;
}

// Save transcript to a .txt file (formatted for Notepad)
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

// Save transcript to a .csv file (formatted for Excel/Sheets)
void saveTranscriptCsv(const Student& student, const std::string& filename) {
    std::ofstream outFile(filename);
    if (!outFile.is_open()) {
        std::cout << "Failed to create " << filename << "\n";
        return;
    }

    outFile << "Student Name," << student.getName() << "\n";
    outFile << "Student ID,"   << student.getId() << "\n\n";
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

    // Load existing transcript data if available before displaying the menu
    std::string txtFile = "transcript_" + student.getId() + ".txt";
    std::string csvFile = "transcript_" + student.getId() + ".csv";

    if(loadExistingTxt(txtFile, student)){
        std::cout << "Loaded existing data from " << txtFile << "\n";
    } 
    
    else if(loadExistingCsv(csvFile, student)){
        std::cout << "Loaded existing data from " << csvFile << "\n";
    } 
    
    else {
        std::cout << "No existing transcript found. A new file will be created when saved.\n";
    }

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

            // Prevent duplicate course codes in the transcript
            if(student.searchCourseCode(courseCode) != -1){
                std::cout << "Course code already exists in transcript!\n";
            } 
            
            else {
                student.addCourse(Course(courseCode, courseName, letterGrade, credit));
                std::cout << "Course added!\n";
            }
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