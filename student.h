#pragma once
#include <string>
#include <vector>
#include "course.h"

class Student {
    private:
      std::string studentId;
      std::string studentName;
      std::vector<Course> courseList;

    public:
      Student(std::string id, std::string name);
      std::string getId() const;
      std::string getName() const;
      const std::vector<Course>& getCourseList() const;

      void addCourse(const Course& c);
      int getTotalCredits() const;
      double calculateCGPA() const;
      void printTranscript() const;
      void sortCourseGrade();
      int searchCourseCode(const std::string& code) const;
};