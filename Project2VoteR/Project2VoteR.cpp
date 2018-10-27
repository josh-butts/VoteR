/*
Josh Butts
Project 2 VoteR 2.0
Driver
*/

#include "Exceptions.h"
#include "TemplatedArray.h"
#include "NvraRecord.h"
#include "NvraComparator.h"
#include "Sorter.h"
#include "Search.h"
#include "OULink.h"
#include "OULinkedList.h"
#include "OULinkedListEnumerator.h"
#include "Enumerator.h"
#include <iostream>
#include <fstream>
#include <string>
// HELLO ME
// checks if the item points to null and throws the proper exception
template <typename T>
void checkMemory(T* item)
{
	if (item == NULL)
	{
		throw new ExceptionMemoryNotAvailable();
	}
}

void getUserFile(OULinkedList<NvraRecord>* mainList, metadata* myMetadata)
{
	std::string userInput;
	std::ifstream* myFile;
	bool dataReadIn = false;
	while (!dataReadIn) //exit loop if we've read in at least one record
	{
		std::cout << "Enter data file name: ";
		getline(std::cin, userInput);
		myFile->open(userInput); //attempt to open the file
		if (!myFile->is_open()) //check if the file was successfully opened
		{
			dataReadIn = false;
			std::cout << "File is not available.\n";
		}
		else
		{
			if (!readFile(myFile, mainList, myMetadata)) //file yeilds no valid records
			{
				std::cout << "No valid records found.";
			}
			else //records were stored
			{
				dataReadIn = true;
			}
		}
	}
}

//Reads from a file into a linked list. Returns true if at least 1 record was read in.
bool readFile(std::ifstream* file, OULinkedList<NvraRecord>* list, metadata* metadata)
{

	return true;
}

void output()
{

}

void sort()
{

}

void find()
{

}

void merge()
{

}

void purge()
{

}

void records()
{

}

struct metadata //Data about all the file data
{
	unsigned long lineCount = 0;	//number of lines attempted
	unsigned long validCount = 0;	//number of lines with valid data
	unsigned long memoryCount = 0;	//number of lines in memory
};

struct recordData //Data used to construct a record when reading from a file
{
	unsigned long elementCount = 0; //the element currently being read in
	bool storeInMemory = true;		//whether the record should be stored. It's set false if there is invalid data or a duplicate
	bool isValid = true;			//whether the record contains all valid data (ignoring duplicates)
};

int main()
{
	//TODO: make methods for id procedure, mid elements procedure, last element procedure
	metadata* myMetadata = new metadata;
	NvraComparator* compareID = new NvraComparator(0); //comparator based on the id column
	OULinkedList<NvraRecord>* mainList = new OULinkedList<NvraRecord>(compareID); //holds all the lines
	checkMemory(mainList);
	TemplatedArray<unsigned int>* ids = new TemplatedArray<unsigned int>; //holds ids
	checkMemory(ids);
	std::string userInput; //holds the user's input to the console
	std::ifstream myFile;
	getUserFile(mainList, myMetadata);
	while (userInput != "q")
	{
		if (userInput == "o")
		{
			//output method
			output();
		}
		if (userInput == "s")
		{
			//sort method
			sort();
		}
		if (userInput == "f")
		{
			//find method
			find();
		}
		if (userInput == "m")
		{
			//merge method
			merge();
		}
		if (userInput == "p")
		{
			//purge method
			purge();
		}
		if (userInput == "r")
		{
			//records method
			records();
		}
		std::cout << "Enter (o)utput, (s)ort, (f)ind, (m)erge, (p)urge, (r)ecords, or (q)uit: ";
		getline(std::cin, userInput);
	}
	std::cout << "Thanks for using VoteR.\n";
	return 0;
}
