#include <iostream>
#include <string>
#include <iomanip>
#include <algorithm>
#include "student.h"
#include "course.h"

Student::Student(std::string id, std::string name){
    studentId   = id;
    studentName = name;
}

std::string Student::getId() const {
    return studentId;
}

std::string Student::getName() const {
    return studentName;
}

const std::vector<Course>& Student::getCourseList() const {
    return courseList;
}

void Student::addCourse(const Course& c){
    courseList.push_back(c);
}

int Student::getTotalCredits() const {
    int total = 0;
    for (const Course& course : courseList) {
        total += course.getCredit();
    }
    return total;
}

double Student::calculateCGPA() const {
    double totalCredits = getTotalCredits();
    double points = 0;
    
    if(totalCredits == 0){
        return 0.0;
    }

    else {
        for(const Course& course : courseList){
            points += course.totalPoints();
        }
    }

    return points/totalCredits;
}

void Student::sortCourseGrade() {
    std::sort(courseList.begin(), courseList.end(), [](const Course& a, const Course& b){
        return a.gradePoints() > b.gradePoints();
    });
}

int Student::searchCourseCode(const std::string& code) const {
    for (size_t i = 0; i < courseList.size(); ++i) {
        if (courseList[i].getCourseCode() == code) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

void Student::printTranscript() const {
    std::cout << "Student Name : " << studentName << "\n";
    std::cout << "Student ID   : " << studentId << "\n";
    std::cout << "----------------------------------------------------------------------------------------\n";
    std::cout << std::left 
              << std::setw(10) << "Code"
              << std::setw(38) << "Course Name"
              << std::setw(13) << "Credit"
              << std::setw(13) << "Grade"
              << std::setw(13) << "Points" << "\n";
    std::cout << "----------------------------------------------------------------------------------------\n";

    for (const Course& course : courseList) {
        std::cout << std::left 
                  << std::setw(10) << course.getCourseCode()
                  << std::setw(38) << course.getCourseName()
                  << std::setw(13) << course.getCredit()
                  << std::setw(13) << course.getLetterGrade()
                  << std::fixed << std::setprecision(2) << course.gradePoints() << "\n";
    }

    std::cout << "----------------------------------------------------------------------------------------\n";
    std::cout << "Total Credits : " << getTotalCredits() << "\n";
    std::cout << "CGPA          : " << std::fixed << std::setprecision(2) << calculateCGPA() << "\n";
}