#ifndef SORT_H
#define SORT_H

#include "Exceptions.h"
#include "TemplatedArray.h"
#include "Comparator.h"

template <typename T>
class Sorter {
private:
	// Both private methods are adapted from ZyBooks
	static void mergeSort(TemplatedArray<T>& array, Comparator<T>& comparator, unsigned long i, unsigned long k);
	static void merge(TemplatedArray<T>& array, Comparator<T>& comparator, unsigned long i, unsigned long j, unsigned long k);
public:
	static void sort(TemplatedArray<T>& array, const Comparator<T>& comparator);
};

// Calls mergeSort
template<typename T>
void Sorter<T>::sort(TemplatedArray<T>& array, const Comparator<T>& comparator)
{
	mergeSort(array, comparator, 0, array.getSize() - 1);
}

// Recursively defined merge sort algorithm
template<typename T>
void Sorter<T>::mergeSort(TemplatedArray<T>& array, Comparator<T>& comparator, unsigned long i, unsigned long k)
{
	unsigned long j = 0;
	if (i < k)
	{
		j = (i + k) / 2;  // Find the midpoint in the partition

		// Recursively sort left and right partitions
		mergeSort(array, comparator, i, j);
		mergeSort(array, comparator, j + 1, k);

		// Merge left and right partition in sorted order
		merge(array, comparator, i, j, k);

	}
}

// Used only by mergeSort, the function merges two arrays and copies back the merged array to the original
template<typename T>
void Sorter<T>::merge(TemplatedArray<T>& array, Comparator<T>& comparator, unsigned long i, unsigned long j, unsigned long k)
{
	unsigned long mergedSize = k - i + 1;								// Size of merged partition
	unsigned long mergePos = 0;											// Position to insert merged number
	unsigned long leftPos = 0;											// Position of elements in left partition
	unsigned long rightPos = 0;											// Position of elements in right partition
	TemplatedArray<T>* mergedArray = new TemplatedArray<T>(mergedSize);	// Dynamically allocates temporary array for merged items
	if (mergedArray == nullptr)
	{
		throw new ExceptionMemoryNotAvailable();
	}
	leftPos = i;														// Initialize left partition position
	rightPos = j + 1;													// Initialize right partition position
	// Add smallest element from left or right partition to merged numbers
	while (leftPos <= j && rightPos <= k)
	{
		if (comparator.compare(array.get(leftPos), array.get(rightPos)) <= 0)
		{
			T temp = array.get(leftPos);
			mergedArray->add(&(temp));
			++leftPos;
		}
		else
		{
			T temp = array.get(rightPos);
			mergedArray->add(&(temp));
			++rightPos;
		}
		++mergePos;
	}
	// If left partition is not empty, add remaining elements to merged numbers
	while (leftPos <= j)
	{
		T temp = array.get(leftPos);
		mergedArray->add(&(temp));
		++leftPos;
		++mergePos;
	}
	// If right partition is not empty, add remaining elements to merged numbers
	while (rightPos <= k)
	{
		T temp = array.get(rightPos);
		mergedArray->add(&(temp));
		++rightPos;
		++mergePos;
	}
	// Copy merge number back to numbers
	for (mergePos = 0; mergePos < mergedSize; ++mergePos)
	{
		T temp = mergedArray->get(mergePos);
		array.replaceAt(&(temp), i + mergePos);
	}
}

#endif