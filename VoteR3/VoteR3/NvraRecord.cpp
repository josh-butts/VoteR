/*
Represents a single NVRA Record
Contains implementation for several get and set methods
Includes an overloaded << operator for easier printing
*/

#include <iostream>
#include <string>
#include "NvraRecord.h"

// overloaded output operator that sends one formatted record to ostream
std::ostream& operator<<(std::ostream& os, const NvraRecord& record)
{
	//reset iterators
	int numsCount = 0;
	int stringsCount = 0;
	for (unsigned int j = 0; j < 24; ++j)
	{
		if (j != 3 && j != 11 && j != 12) //if its a number
		{
			os << record.nums[numsCount]; //print the number
			if (j != 23) //print a ";" unless its the last element
			{
				os << ";";
			}
			++numsCount;
		}
		else
		{
			os << record.strings[stringsCount] << ";"; //print the string plus a ";"
			++stringsCount;
		}
	}
	return os;
}

void NvraRecord::addNum(unsigned int num) // puts num in array, increments counter
{
	nums[numCtr] = num;
	++numCtr;
}

void NvraRecord::addString(std::string string) // puts string in array, increments counter
{
	strings[strCtr] = string;
	++strCtr;
}

unsigned int NvraRecord::getNum(unsigned int index) const // returns (copy of) num at index in array
{
	if (index >= MAX_NUMS)
	{
		throw new ExceptionIndexOutOfRange();
	}
	return nums[index];
}

std::string NvraRecord::getString(unsigned int index) const // returns (copy of) string at index in array
{
	if (index >= MAX_STRINGS)
	{
		throw new ExceptionIndexOutOfRange();
	}
	return strings[index];
}

void NvraRecord::setNum(unsigned int num, unsigned int index) // sets the number at index to be the given number
{
	if (index >= MAX_NUMS)
	{
		throw new ExceptionIndexOutOfRange();
	}
	nums[index] = num;
}

void NvraRecord::setString(std::string string, unsigned int index) // sets the string at index to be the given string
{
	if (index >= MAX_STRINGS)
	{
		throw new ExceptionIndexOutOfRange();
	}
	strings[index] = string;
}