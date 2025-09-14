#ifndef _STUDENT_FUNCTIONAL_HPP_
#define _STUDENT_FUNCTIONAL_HPP_

#include <iostream>

#include <span>
#include <vector>
#include <iterator>

#include "Student.hpp"

Student FindBestStudent(std::vector<Student> studentVector)
{
    std::vector<Student>::iterator currentStudent = studentVector.begin();
    Student bestStudent = *currentStudent;

    for(int i = 1; i < studentVector.size(); ++i, ++currentStudent)
    {
        if(bestStudent.GetScore() < currentStudent->GetScore())
            bestStudent = *currentStudent;
    }

    return bestStudent;
}

Student FindWorstStudent(std::vector<Student> studentVector)
{
    std::vector<Student>::iterator currentStudent = studentVector.begin();
    Student worstStudent = *currentStudent;

    for(int i = 1; i < studentVector.size(); ++i, ++currentStudent)
    {
        if(worstStudent.GetScore() > currentStudent->GetScore())
            worstStudent = *currentStudent;
    }

    return worstStudent;
}

void SetStudentScoreByName(std::vector<Student>& studentVector, const std::string& name, unsigned short score)
{
    for(int i = 0; i < studentVector.size(); ++i)
        if(studentVector[i].GetName() == name)
        {
            studentVector[i].SetScore(score);
            break;
        }
}

double GetAverageScoreOfGroup(const std::vector<Student> studentVector)
{   
    double res;
    int i = 0;

    for(i; i < studentVector.size(); ++i)
        res += (double)studentVector[i].GetScore();

    return res / (double)i;
}

void PrintStudentSublist(std::span<Student> data, size_t start, size_t end)
{
    for(int i = 0; i < data.size(); ++i)
        if(i >= start && i < end)
            std::cout << data[i].GetName() << ' ';

    std::cout << std::endl;
}

#endif