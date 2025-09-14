#include <string>
#include <vector>
#include <initializer_list>

enum cnt_parameter
{
	upper,
	lower,
	digits,
	spaces
};

size_t mix(const std::string& destination, const std::string& source, unsigned short depth = 0)
{
	static std::string combination;
	
	if(depth == 0)
		combination = "";

	combination += tolower(destination[depth]);
	
	if(depth < destination.length() - 1)
	{
		size_t result = mix(destination, source, depth + 1);

		if(result != std::string::npos)
			return result;
	}
	else
	{
		size_t pos = source.find(combination);

		if(pos != std::string::npos)
			return pos;
	}

	combination.pop_back();

	combination += toupper(destination[depth]);
	
	if(depth < destination.length() - 1)
	{
		size_t result = mix(destination, source, depth + 1);

		if(result != std::string::npos)
			return result;
	}
	else
	{
		size_t pos = source.find(combination);

		if(pos != std::string::npos)
			return pos;
	}

	combination.pop_back();

	return std::string::npos;
}

std::vector<int> str_count(const std::string& str)
{
	std::vector<int> counter = {0, 0, 0, 0};
	int cnt = 0;

	for(int i = 0; i < str.length(); ++i)
	{
		if (isupper(str[i]))
		{
			++counter[0];
			continue;
		}

		if (islower(str[i]))
		{
			++counter[1];
			continue;
		}

		if (isdigit(str[i]))
		{
			++counter[2];
			continue;
		}

		if (isspace(str[i]))
		{
			++counter[3];
			continue;
		}
	}	

	return counter;
}

std::vector<int> str_count(const std::string& str, std::initializer_list<cnt_parameter> parameters)
{
	char i = -1;
	char vectorLen = 0;

	char posUp {-1}, posLw {-1}, posDg {-1}, posSp {-1};

	for(int x : parameters)
	{
		++i;

		if(posUp == -1 && x == upper)
		{
			posUp = i;
			++vectorLen;
			continue;
		}

		if(posLw == -1 && x == lower)
		{
			posLw = i;
			++vectorLen;
			continue;
		}

		if(posDg == -1 && x == digits)
		{
			posDg = i;
			++vectorLen;
			continue;
		}

		if(posSp == -1 && x == spaces)
		{
			posSp = i;
			++vectorLen;
			continue;
		}
	}

	std::vector<int> counter(vectorLen);

	for(int i = 0; i < str.length(); ++i)
	{
		if (posUp != -1 && isupper(str[i]))
		{
			++counter[posUp];
			continue;
		}

		if (posLw != -1 && islower(str[i]))
		{
			++counter[posLw];
			continue;
		}

		if (posDg != -1 && isdigit(str[i]))
		{
			++counter[posDg];
			continue;
		}

		if (posSp != -1 && isspace(str[i]))
		{
			++counter[posSp];
			continue;
		}
	}	

	return counter;
}

std::string transform_string(const std::string& source)
{
	std::string newString = "";

	for(int i = 0; i < source.length(); ++i)
	{
		char c = source[i];

		if(isupper(c))
		{
			newString += tolower(c);
			continue;
		}

		if(islower(c))
		{
			newString += toupper(c);
			continue;
		}

		if(isspace(c))
			continue;
		
		newString += c;
	}

	while (true)
	{
		size_t pos = mix("hello", newString);
		
		if(pos != std::string::npos)
			newString.replace(pos, pos + 5, "hi");
		else break;
	}

	return newString;
}
