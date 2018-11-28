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
public:
	HashTableEnumerator(HashTable<T>* hashTable);			// constructor needs a pointer to the HashTable to be enumerated
	virtual ~HashTableEnumerator();
	bool hasNext() const;									// true if there are elements that have not yet been returned via next()
	T next();												// throws ExceptionEnumerationBeyondEnd if no next item is available
	T peek() const;											// throws ExceptionEnumerationBeyondEnd if no next item is available
};

// put implementation for HashTableEnumerator here
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
	for (unsigned long i = bucket; i < hashTable->getBaseCapacity; ++i) //iterating through the buckets
	{
		OULinkedListEnumerator<T>* newChainEnumerator = &hashTable->table[i]->enumerator();
		if (newChainEnumerator->hasNext())
		{
			return true;
		}
	}
	return false;
}

template <typename T>
T HashTableEnumerator<T>::next()
{
	for (unsigned long i = bucket; i < hashTable->getBaseCapacity; ++i)
	{
		chainEnumerator = &hashTable->table[i]->enumerator();
		if (chainEnumerator->hasNext())
		{
			return chainEnumerator->next();
		}
	}
	throw new ExceptionEnumerationBeyondEnd();
}

template <typename T>
T HashTableEnumerator<T>::peek() const
{
	for (unsigned long i = bucket; i < hashTable->getBaseCapacity; ++i)
	{
		OULinkedListEnumerator<T>* newChainEnumerator = &hashTable->table[i]->enumerator();
		if (newChainEnumerator->hasNext())
		{
			return newChainEnumerator->peek();
		}
	}
	throw new ExceptionEnumerationBeyondEnd();
}

#endif // !HASH_TABLE_ENUMERATOR
