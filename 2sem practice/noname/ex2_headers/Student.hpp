#ifndef _STUDENT_HPP_
#define _STUDENT_HPP_

#include <iostream>
#include <string>

class Student
{
    unsigned short avgScore {0};
    std::string name;

    unsigned short __FitTheScore__(unsigned short num)
    {
        if(num > 5) return 5;

        return num;
    }

public:

    Student() 
    {
        name = "Unnamed";
    }

    Student(const Student& other) : avgScore(other.avgScore), name(other.name) {}

    Student(unsigned short averageScore, const std::string& name) : avgScore(__FitTheScore__(averageScore)), name(name) {}

    void SetScore(unsigned short averageScore)
    {
        avgScore = __FitTheScore__(averageScore);
    }

    void SetName(const std::string& name)
    {
        this->name = name;
    }

    unsigned short GetScore() const
    {
        return avgScore;
    }

    const std::string GetName() const
    {
        return name;
    }

    void Print() const
    {
        std::cout << "Hi! My name is " << name << "!" << std::endl;
        std::cout << "My average score is " << avgScore << std::endl;
    }

};

bool CompareByScore(const Student& a, const Student& b) {
    return a.GetScore() > b.GetScore();
}

#endif