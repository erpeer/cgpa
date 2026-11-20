#include <string>
#include "course.h"

Course::Course(std::string code, std::string name, std::string grade, int cred){
    courseCode = code;
    courseName = name;
    letterGrade = grade;
    credit = cred;
}

std::string Course::getCourseCode() const {
    return courseCode;
}

std::string Course::getCourseName() const {
    return courseName;
} 

std::string Course::getLetterGrade() const {
    return letterGrade;
}

int Course::getCredit() const {
    return credit;
}

double Course::gradePoints() const {
        if(letterGrade == "A"  || letterGrade == "a" ) return 4.00;
        else if(letterGrade == "AB" || letterGrade == "ab") return 3.50;
        else if(letterGrade == "B"  || letterGrade == "b" ) return 3.00;
        else if(letterGrade == "BC" || letterGrade == "bc") return 2.50;
        else if(letterGrade == "C"  || letterGrade == "c" ) return 2.00;
        else if(letterGrade == "D"  || letterGrade == "d" ) return 1.00;
        else if(letterGrade == "E"  || letterGrade == "e" ) return 0.00;
        else return 0.00;
}

double Course::totalPoints() const {
    return gradePoints()*credit;
}