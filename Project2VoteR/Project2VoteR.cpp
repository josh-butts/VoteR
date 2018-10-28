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

//Data about all the file data
struct metadata
{
	unsigned long lineCount = 0;	//number of lines attempted
	unsigned long validCount = 0;	//number of lines with valid data
	unsigned long memoryCount = 0;	//number of lines in memory
};

//Data used to construct a record when reading from a file
struct recordData
{
	std::string element;			//holds an element to be processed
	bool elementIsNum;						//true indicates that the element is a number, false for string
	unsigned long elementCount = 0; //counter for the element currently being read in
	bool hasDuplicate = false;		//whether the record has a duplicate id
	bool isValid = true;			//whether the record contains all valid data (ignoring duplicates)
};

//Checks if the item points to null and throws the proper exception
template <typename T>
void checkMemory(T* item)
{
	if (item == NULL)
	{
		throw new ExceptionMemoryNotAvailable();
	}
}

void setElementState(recordData* myRecordData)
{
	if (myRecordData->elementCount == 3 || myRecordData->elementCount == 11 || myRecordData->elementCount == 12)
	{
		myRecordData->elementIsNum = false;
	}
	else
	{
		myRecordData->elementIsNum = true;
	}
}

//Attempts to add a record to the list
void addRecord(NvraRecord* record, OULinkedList<NvraRecord>* list, TemplatedArray<unsigned int>* ids, recordData* recordData, metadata* metadata)
{
	if (recordData->isValid && !recordData->hasDuplicate)
	{
		list->insert(record);
		ids->add(new unsigned int (record->getNum(0)));
		++metadata->memoryCount;
		++metadata->validCount;
		++metadata->lineCount;
	}
	else if (recordData->isValid) //data is valid but has a duplicate
	{
		++metadata->validCount;
		++metadata->lineCount;
	}
	else //data is not valid
	{
		++metadata->lineCount;
	}
}

//Adds a single element to a record
void addElement(NvraRecord* record, metadata* metadata, recordData* recordData)
{
	if (recordData->elementIsNum) //element is a number
	{
		int tempInt = stoi(recordData->element);
		if (tempInt < 0) //if the element is negative
		{
			std::cout << "Invalid data at line " << metadata->lineCount + 1 << "." << std::endl; //print invalid data error
			recordData->isValid = false; //record contains invalid data
		}
		else
		{
			record->addNum(tempInt); //store the int
		}
	}
	else //element is a string
	{
		record->addString(recordData->element); //store the string
	}
}

//Reads from a file into a linked list. Returns true if at least 1 record was read in.
bool readFile(std::ifstream* file, OULinkedList<NvraRecord>* list, metadata* metadata)
{
	NvraRecord* record = new NvraRecord; //holds a line of processed data
	checkMemory(record);
	TemplatedArray<unsigned int>* ids = new TemplatedArray<unsigned int>; //holds ids
	checkMemory(ids);
	std::string temp;
	recordData* myRecordData = new recordData;
	getline(*file, myRecordData->element); //throw out header
	while (getline(*file, myRecordData->element, ',')) //gets elements separated by commas, the last element is a special case
	{
		setElementState(myRecordData);
		addElement(record, metadata, myRecordData);
		++myRecordData->elementCount;
		if (myRecordData->elementCount == 23) //last element
		{
			getline(*file, myRecordData->element, '\n'); //gets last element
			addElement(record, metadata, myRecordData);
			for (unsigned long i = 0; i < ids->getSize(); ++i) //check for duplicate ids
			{
				if (ids->get(i) == record->getNum(0)) //if the id matches another id
				{
					std::cout << "Duplicate record ID " << ids->get(i) << " at line " << metadata->lineCount + 1 << ".\n";
					myRecordData->hasDuplicate = true;
				}
			}
			addRecord(record, list, ids, myRecordData, metadata); //attempt to store the record
			//reset for next line:
			record = new NvraRecord;
			myRecordData->elementCount = 0;
			myRecordData->isValid = true;
			myRecordData->hasDuplicate = false;
		}
	}
	delete myRecordData;
	if (list->getSize() > 0) //return true if a record was read in
	{
		return true;
	}
	else
	{
		return false;
	}
}

//attempts to get and open a file, returns false if the user presses enter only
bool openFile(std::ifstream* file)
{
	while (!file->is_open())
	{
		std::string userInput;
		std::cout << "Enter data file name: ";
		getline(std::cin, userInput);
		if (userInput == "") //check if the user just presses enter
		{
			return false;
		}
		file->open(userInput); //attempt to open the file
		if (!file->is_open()) //check if the file was successfully opened
		{
			std::cout << "File is not available.\n";
		}
	}
	return true;
}


//Gets the initial file from the user
int getUserFile(OULinkedList<NvraRecord>* mainList, metadata* myMetadata)
{
	std::string userInput;
	std::ifstream* myFile = new std::ifstream;
	bool dataReadIn = false;
	while (!dataReadIn) //exit loop if we've read in at least one record
	{
		if (openFile(myFile))
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
		else //user presses enter only
		{
			return 1;
		}
	}
	delete myFile;
	return 0;
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

void mergeLists(OULinkedList<NvraRecord>* mainList, OULinkedList<NvraRecord>* list)
{
	//TODO: use an enumerator to add and replace things in the mainlist with things in list
}

void merge(OULinkedList<NvraRecord>* mainList, metadata* metadata)
{
	std::string userInput;
	std::ifstream* myFile = new std::ifstream;
	NvraComparator* compareID = new NvraComparator(0); //comparator based on the id column
	OULinkedList<NvraRecord>* list = new OULinkedList<NvraRecord>(compareID); //holds all the lines
	openFile(myFile);
	readFile(myFile, list, metadata);
	mergeLists(mainList, list);



	delete myFile;
	delete compareID;
	delete list;
}

void purge()
{

}

void records()
{

}

int main()
{
	metadata* myMetadata = new metadata; //contains the metadata of running the program
	NvraComparator* compareID = new NvraComparator(0); //comparator based on the id column
	OULinkedList<NvraRecord>* mainList = new OULinkedList<NvraRecord>(compareID); //holds all the lines
	checkMemory(mainList);
	TemplatedArray<unsigned int>* ids = new TemplatedArray<unsigned int>; //holds ids
	checkMemory(ids);
	std::string userInput; //contains what the user types into the console
	if (getUserFile(mainList, myMetadata) == 1) //get the initial file, if the user just presses enter then close voteR
	{
		std::cout << "Thanks for using VoteR.\n";
		return 0;
	}
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
			merge(mainList, myMetadata);
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
