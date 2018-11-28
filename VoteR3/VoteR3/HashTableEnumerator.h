#ifndef HASH_TABLE_ENUMERATOR
#define HASH_TABLE_ENUMERATOR

#include "Enumerator.h"
#include "Exceptions.h"
#include "HashTable.h"

template <typename T>
class HashTableEnumerator : public Enumerator<T>
{
private:
	unsigned long bucket = 0;								// the current bucket during the enumeration process
	OULinkedListEnumerator<T>* chainEnumerator = NULL;		// used to move through the linked list of the current bucket
	HashTable<T>* hashTable = NULL;							// pointer to the HashTable being enumerated
	// you may add additional member variables and functions here to support the operation of your code
	void findNextBucket();
public:
	HashTableEnumerator(HashTable<T>* hashTable);			// constructor needs a pointer to the HashTable to be enumerated
	virtual ~HashTableEnumerator();
	bool hasNext() const;									// true if there are elements that have not yet been returned via next()
	T next();												// throws ExceptionEnumerationBeyondEnd if no next item is available
	T peek() const;											// throws ExceptionEnumerationBeyondEnd if no next item is available
};

// put implementation for HashTableEnumerator here
//template <typename>


template <typename T>
HashTableEnumerator<T>::HashTableEnumerator(HashTable<T>* hashTable)
{
	if (hashTable->getSize() < 0)
	{
		throw new ExceptionHashTableAccess();
	}
	this->hashTable = hashTable;
	chainEnumerator = &hashTable->table[bucket]->enumerator();
}

template <typename T>
HashTableEnumerator<T>::~HashTableEnumerator() {}

template <typename T>
bool HashTableEnumerator<T>::hasNext() const
{
	return chainEnumerator->hasNext();
}

template <typename T>
T HashTableEnumerator<T>::next()
{
	T temp;
	if (chainEnumerator->hasNext())
	{
		temp = chainEnumerator->next();
	}
	while (!chainEnumerator->hasNext()) //while at the end of a list/in an empty list
	{
		++bucket; //move to the next bucket
		if (bucket >= hashTable->baseCapacity) //throw exception if end of table is reached
		{
			throw new ExceptionEnumerationBeyondEnd();
		}
		chainEnumerator = new OULinkedListEnumerator<T>(hashTable->table[bucket]->enumerator()); //update the enumerator to the next bucket
	}

	return temp; //return item and move to next
}

template <typename T>
T HashTableEnumerator<T>::peek() const
{
	return chainEnumerator->peek();
}

#endif // !HASH_TABLE_ENUMERATOR
