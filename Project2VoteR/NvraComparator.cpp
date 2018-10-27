/*
Comparator specific to NVRA Records
Takes the column number into account to determine whether to use getString or getNum
*/

#include "NvraComparator.h"

NvraComparator::NvraComparator(unsigned int column)
{
	this->column = column; // column to compare by
}

int NvraComparator::compare(const NvraRecord& item1, const NvraRecord& item2) const
{
	if (this->column == 3 || this->column == 11 || this->column == 12) // it's a string
	{
		// Set Index:
		unsigned int index;
		if (column == 3)
		{
			index = 0;
		}
		else if (column == 11)
		{
			index = 1;
		}
		else
		{
			index = 2;
		}
		// Comparison:
		if (item1.getString(index) < item2.getString(index))
		{
			return -1;
		}
		else if (item1.getString(index) > item2.getString(index))
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else // it's a number
	{
		// Set index:
		unsigned int index;
		if (column < 3)
		{
			index = column;
		}
		else if (column > 3 && column < 11)
		{
			index = column - 1;
		}
		else
		{
			index = column - 3;
		}
		// Comparison:
		if (item1.getNum(index) < item2.getNum(index))
		{
			return -1;
		}
		else if (item1.getNum(index) > item2.getNum(index))
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
}