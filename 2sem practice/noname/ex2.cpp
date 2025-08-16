#include <iostream>

#include <algorithm>

#include "ex2_headers/Student.hpp"
#include "ex2_headers/StudentFunctional.hpp"

int main()
{
    Student daniil {3, "Daniil"},
            nikolay {4, "Nikolay"},
            gleb {5, "Gleb"},
            egor {2, "Egor"};

    std::vector<Student> studentVector = {gleb, daniil, nikolay, egor};

    FindBestStudent(studentVector).Print();
    FindWorstStudent(studentVector).Print();

    PrintStudentSublist(studentVector, 1, 3);

    SetStudentScoreByName(studentVector, "Gleb", 2);

    studentVector[0].Print();

    std::cout << GetAverageScoreOfGroup(studentVector) << std::endl;

    std::sort(studentVector.begin(), studentVector.end(), CompareByScore);

    for(int i = 0; i < studentVector.size(); ++i)
    {
        std::cout << std::endl;
        studentVector[i].Print();
        std::cout << std::endl;
    }

    return 0;
}