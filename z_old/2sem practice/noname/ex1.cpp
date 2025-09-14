#include <iostream>

#include <string>

#include "ex1_headers/usefull_functions.hpp"

int main()
{
	std::string str;

	std::cout << "Enter string: ";
	std::getline(std::cin, str);
	
	std::vector<int> nums {str_count(str, {upper, digits, spaces})}; 

	for(int i = 0; i < nums.size(); ++i)
		std::cout << nums[i] << std::endl;

	std::string newString = transform_string(str);

	std::cout << newString << std::endl;a

	return 0;
}
