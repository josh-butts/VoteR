#ifndef TEMPLATED_ARRAY_H
#define TEMPLATED_ARRAY_H

#include "Exceptions.h"
#include <cstdlib> //importing so that NULL will work

const unsigned long DEFAULT_ARRAY_CAPACITY = 10;		// capacity used in no arg constructor

template <typename T>
class TemplatedArray {
private:
	unsigned long capacity = DEFAULT_ARRAY_CAPACITY;	// maximum capacity, in items
	unsigned long size = 0;								// actual number of items currently in array
	T* data = NULL;										// pointer to array of any type
	void doubleCapacity();								// method to double array capacity
	void halveCapacity();								// method to halve array capacity
public:
	TemplatedArray();									// constructs array with default capacity
	TemplatedArray(unsigned long capacity);				// constructs array with specified capacity
	virtual ~TemplatedArray();							// frees array space as object is deleted
	void add(const T* item);							// adds item, increments size, doubles capacity as necessary
	void addAt(const T* item, unsigned long index);		// adds item at index, shifts following, doubles capacity as necessary
	void replaceAt(const T* item, unsigned long index);	// replaces item at index, otherwise unchanged
	void removeAt(unsigned long index);					// removes item at index, shifts following back
	T get(unsigned long index) const;					// returns (copy of) item at index
	unsigned long getCapacity() const;					// returns the current capacity of the array
	unsigned long getSize() const;						// returns the current number of items in the array
	T operator[](unsigned long index) const;			// returns (copy of) item at index
};


// Private Methods:

template<typename T>
void TemplatedArray<T>::doubleCapacity()
{
	//iterate through the current array to copy data onto the new array
	T* temp = new T[this->capacity * 2]; //create a new array of double capacity
	if (temp == NULL)
	{
		throw new ExceptionMemoryNotAvailable();
	}
	for (unsigned long i = 0; i < this->capacity; ++i)
	{
		temp[i] = this->get(i); //copy the old array element to the new one
	}
	delete[] data; // delete the old one
	this->data = temp; //point data to the new array
	this->capacity *= 2; //double the capacity variable
}

template<typename T>
void TemplatedArray<T>::halveCapacity()
{
	//iterate through the current array to copy data onto the new array
	T* temp = new T[this->capacity / 2]; //create a new array of half capacity
	if (temp == NULL)
	{
		throw new ExceptionMemoryNotAvailable();
	}
	for (unsigned long i = 0; i < this->capacity / 2; ++i)
	{
		temp[i] = this->get(i); //copy the old array element to the new one
	}
	delete[] data; // delete the old one
	this->data = temp; //point data to the new array
	this->capacity /= 2; //half the capacity variable
}

// Public Methods:

template<typename T>
TemplatedArray<T>::TemplatedArray() // constructor
{
	data = new T[DEFAULT_ARRAY_CAPACITY];
	if (data == NULL)
	{
		throw new ExceptionMemoryNotAvailable();
	}
}

template<typename T>
TemplatedArray<T>::TemplatedArray(unsigned long capacity) // constructor with starting capacity
{
	data = new T[capacity];
	if (data == NULL)
	{
		throw new ExceptionMemoryNotAvailable();
	}
	this->capacity = capacity;
}

template<typename T>
TemplatedArray<T>::~TemplatedArray() // destructor
{
	delete[] data;
	data = NULL;
}

template<typename T>
void TemplatedArray<T>::add(const T* item) // add an item to the end of the array, resizes if the array is at capacity
{
	if (this->size != this->capacity) //check that it's not at capacity
	{
		this->data[size] = *item;
	}
	else
	{
		this->doubleCapacity(); //double the capacity
		this->data[size] = *item; //add the item
	}
	++this->size; //increase size
}

template<typename T>
void TemplatedArray<T>::addAt(const T* item, unsigned long index) // add an item to the array at a specific location, resizes if the array is at capacity
{
	if (index > size) //check that the index is valid. if index == size, its the same as add()
	{
		throw new ExceptionIndexOutOfRange();
	}
	if (this->size != this->capacity) //check that it's not at capacity
	{
		//shift all the items one space to the right to open a space for the new item
		for (unsigned long i = this->size; i > index; --i)
		{
			this->data[i] = this->data[i - 1];
		}
		this->data[index] = *item; //add the item
	}
	else
	{
		this->doubleCapacity(); //double the capacity
		//shift all the items one space to the right to open a space for the new item
		for (unsigned long i = this->size; i > index; --i)
		{
			this->data[i] = this->data[i - 1];
		}
		this->data[index] = *item; //add the item
	}
	++this->size; //increase size
}

template<typename T>
void TemplatedArray<T>::replaceAt(const T* item, unsigned long index) // replaces an item in the array at a specified index
{
	if (index >= size && size != 0) //check that the index is in range
	{
		throw new ExceptionIndexOutOfRange();
	}
	this->data[index] = *item;
}

template<typename T>
void TemplatedArray<T>::removeAt(unsigned long index) // removes an item in the array at a specified index
{
	if (index >= this->size) //check that the index is in range
	{
		throw new ExceptionIndexOutOfRange();
	}
	for (unsigned long i = index; i < this->size - 1; ++i) //loop changes each element to be the element after it
	{
		this->data[i] = this->data[i + 1];
	}
	--this->size; //the last element cannot be accessed
	if (this->size <= this->capacity / 2) // if the size is less than or equal to half the capacity, then halve the capacity
	{
		this->halveCapacity();
	}
}

template<typename T>
T TemplatedArray<T>::get(unsigned long index) const // get a copy of an item from the array at a specified index
{
	if (index >= size) //check that the index is in range
	{
		throw new ExceptionIndexOutOfRange();
	}
	return this->data[index];
}

template<typename T>
unsigned long TemplatedArray<T>::getCapacity() const //return the capacity
{
	return this->capacity;
}

template<typename T>
unsigned long TemplatedArray<T>::getSize() const //return the size
{
	return this->size;
}

template<typename T>
T TemplatedArray<T>::operator[](unsigned long index) const //implements [] to be functionally the same as the get method
{
	if (index >= this->size) //check that the index is in range
	{
		throw new ExceptionIndexOutOfRange();
	}
	return this->data[index];
}

#endif