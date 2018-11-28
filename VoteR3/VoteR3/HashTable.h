#ifndef HASH_TABLE
#define HASH_TABLE

#include "Exceptions.h"
#include "Comparator.h"
#include "OULinkedList.h"
#include "OULinkedListEnumerator.h"
#include "Hasher.h"

#include "NvraRecord.h"

const unsigned int SCHEDULE_SIZE = 25;			// the number of items in the size schedule
const unsigned int SCHEDULE[SCHEDULE_SIZE] = { 1, 2, 5, 11, 23, 53, 107, 223, 449, 907, 1823, 3659, 7309, 14621, 29243, 58511, 117023, 234067, 468157, 936319, 1872667, 3745283, 7490573, 14981147, 29962343 };		// the size schedule (all primes)
const unsigned int DEFAULT_SCHEDULE_INDEX = 3;	// the default position in the size schedule
const unsigned long DEFAULT_BASE_CAPACITY = SCHEDULE[DEFAULT_SCHEDULE_INDEX]; 	// the default size of the array
const float DEFAULT_MAX_LOAD_FACTOR = 0.9f;		// the default load factor used to determine when to increase the table size
const float DEFAULT_MIN_LOAD_FACTOR = 0.4f;		// the default load factor used to determine when to decrease the table size
const unsigned long DEFAULT_TABLE_SIZE = floor(DEFAULT_BASE_CAPACITY*DEFAULT_MAX_LOAD_FACTOR);

template <typename T>
class HashTable {
	template <typename U>
	friend class HashTableEnumerator;						// necessary to allow the enumerator to access the table's private data
private:
	Comparator<T>* comparator = NULL;						// used to determine item equality
	Hasher<T>* hasher = NULL;								// used to compute hash value
	unsigned long size = 0;									// actual number of items currently in hash table
	float maxLoadFactor = DEFAULT_MAX_LOAD_FACTOR;			// the load factor used to determine when to increase the table size
	float minLoadFactor = DEFAULT_MIN_LOAD_FACTOR;			// the load factor used to determine when to decrease the table size
	unsigned int scheduleIndex = DEFAULT_SCHEDULE_INDEX;	// the index of current location in the size schedule
	unsigned long baseCapacity = DEFAULT_BASE_CAPACITY;		// the size of the array
	unsigned long totalCapacity = baseCapacity;				// the size of the array plus chains of more than one link
	OULinkedList<T>** table = NULL;							// table will be an array of pointers to OULinkedLists of type T
	// you may add additional member variables and functions here to support the operation of your code
	// From Dr. Hougen's lecture:
	OULinkedList<T>** initializeTable(unsigned long capacity, Comparator<T>* comparator);
	void copyTable(OULinkedList<T>** newTable);
	void deleteTable();
	void expandTable();
	void contractTable();
public:
	HashTable(Comparator<T>* comparator, Hasher<T>* hasher);			// creates an empty table of DEFAULT_BASE_CAPACITY
	HashTable(Comparator<T>* comparator, Hasher<T>* hasher,
		// if size given, creates empty table with size from schedule of sufficient capacity (considering maxLoadFactor)
		unsigned long size = DEFAULT_TABLE_SIZE,
		float maxLoadFactor = DEFAULT_MAX_LOAD_FACTOR,
		float minLoadFactor = DEFAULT_MIN_LOAD_FACTOR);
	virtual ~HashTable();

	// if an equivalent item is not already present, insert item at proper location and return true
	// if an equivalent item is already present, leave table unchanged and return false
	bool insert(T* item);

	// if an equivalent item is already present, replace item and return true
	// if an equivalent item is not already present, leave table unchanged and return false
	bool replace(T* item);

	// if an equivalent item is already present, remove item and return true
	// if an equivalent item is not already present, leave table unchanged and return false
	bool remove(T* item);

	// if an equivalent item is present, return a copy of the item
	// if an equivalent item is not present, throw a new ExceptionHashTableAccess
	T find(const T* item) const;

	unsigned long getSize() const;						// returns the current number of items in the table
	unsigned long getBaseCapacity() const;				// returns the current base capacity of the table
	unsigned long getTotalCapacity() const;				// returns the current total capacity of the table
	float getLoadFactor() const;						// returns the current load factor of the table
	unsigned long getBucketNumber(const T* item) const;	// returns the bucket number for an item using its hash function mod array size

};

// put implementation for HashTable here
template <typename T>
HashTable<T>::HashTable(Comparator<T>* comparator, Hasher<T>* hasher)
{
	this->comparator = comparator;
	this->hasher = hasher;
	table = initializeTable(baseCapacity, comparator);
}

template <typename T>
HashTable<T>::HashTable(Comparator<T>* comparator, Hasher<T>* hasher, unsigned long size,
	float maxLoadFactor, float minLoadFactor)
{
	this->comparator = comparator;
	this->hasher = hasher;
	this->size = size;
	this->maxLoadFactor = maxLoadFactor;
	this->minLoadFactor = minLoadFactor;
	table = initializeTable(baseCapacity, comparator);
}

template <typename T>
HashTable<T>::~HashTable() 
{

}

template <typename T>
OULinkedList<T>** HashTable<T>::initializeTable(unsigned long capacity, Comparator<T>* comparator)
{
	table = new OULinkedList<T>*[capacity]; //array of pointers to linked lists
	for (unsigned int i = 0; i < capacity; ++i) //loop through the table and initialize the linked lists
	{
		table[i] = new OULinkedList<T>(comparator);
	}
	return table;
}

template <typename T>
bool HashTable<T>::insert(T* item)
{
	unsigned long bucketIndex = getBucketNumber(item); //get bucket number for the item
	if (table[bucketIndex]->insert(item)) //attempt to insert
	{
		//success
		++size; //increment size
		if (table[bucketIndex]->getSize() >= 2)
		{
			++totalCapacity; //increment capacity if overflowing
		}
		//TODO: resizeTable();
		return true;
	}
	return false; //failure, item alread exists in the bucket
}

template <typename T>
bool HashTable<T>::replace(T* item)
{

}

template <typename T>
bool HashTable<T>::remove(T* item)
{

}

template <typename T>
T HashTable<T>::find(const T* item) const
{

}

template <typename T>
unsigned long HashTable<T>::getSize() const
{
	return size;
}

template <typename T>
unsigned long HashTable<T>::getBaseCapacity() const
{
	return baseCapacity;
}

template <typename T>
unsigned long HashTable<T>::getTotalCapacity() const
{
	return totalCapacity;
}

template <typename T>
float HashTable<T>::getLoadFactor() const
{

}

template <typename T>
unsigned long HashTable<T>::getBucketNumber(const T* item) const
{

}

#endif // !HASH_TABLE
