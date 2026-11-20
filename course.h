#pragma once
#include <string>

class Course {
    private:
      std::string courseCode;
      std::string courseName;
      std::string letterGrade;
      int credit;

    public:
      Course(std::string code, std::string name, std::string grade, int cred);
      std::string getCourseCode() const;
      std::string getCourseName() const;
      std::string getLetterGrade() const;
      int getCredit() const;

      double gradePoints() const; //convert letterGrade to grade point (A => 4.00)
      double totalPoints() const; //calculates total points for this course (gradePoints * credit)
};