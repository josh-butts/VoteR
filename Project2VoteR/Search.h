#ifndef SEARCH_H
#define SEARCH_H

#include "Exceptions.h"
#include "TemplatedArray.h"
#include "Comparator.h"

// Returns FIRST array location matching the given item (based on the comparator)
// If not found, return the negative version of the index of where it goes minus one.
// E.g. if it belongs at index zero, return -1, or index 2, return -3
template <typename T>
long long binarySearch(const T& item, const TemplatedArray<T>& array, const Comparator<T>& comparator) {
	long high = array.getSize() - 1;
	long low = 0;
	unsigned long mid;
	while (high >= low)
	{
		mid = (high + low) / 2;
		if (comparator.compare(item, array.get(mid)) == -1) // it's less than mid
		{
			high = mid - 1;
		}
		else if (comparator.compare(item, array.get(mid)) == 1) // it's greater than mid
		{
			low = mid + 1;
		}
		else // it's found
		{
			// find the first instance of the found object
			while (mid != 0 && comparator.compare(array.get(mid), array.get(mid - 1)) == 0) // if it's the same as the one before it
			{
				mid = mid - 1; // move the index back one
			}
			return mid;
		}
	}
	// it's not found
	return -1 * low - 1; // return the negative version of the index of where it goes minus one
}

#endif