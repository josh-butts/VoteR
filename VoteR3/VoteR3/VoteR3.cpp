/*
Josh Butts
Project 3 VoteR 3.0
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

//Encapsulates an integer element
class NumElement
{
private:
	int value;
	bool isValid = true;
	bool isID;
public:
	NumElement(int value, bool isID)
	{
		this->value = value;
		this->isID = isID;
		if (value < 0)
		{
			isValid = false;
		}
	}
	int getValue()
	{
		return value;
	}
	bool getValidity()
	{
		return isValid;
	}
	bool getIDStatus()
	{
		return isID;
	}
};

//Encapsulates a string element
class StrElement
{
private:
	std::string value;
public:
	StrElement(std::string value)
	{
		this->value = value;
	}
	std::string getValue()
	{
		return value;
	}
};

//Used to construct a record when reading from a file
class Line
{
private:
	NvraRecord* record;				//the record being constructed from this line
	unsigned long lineCount;		//the current line count, unique for each line object
	unsigned long elementCount = 0; //counter for the element currently being read in
	bool isValid = true;			//whether the record contains all valid data (ignoring duplicates)
public:
	Line(NvraRecord* record, unsigned long lineCount)
	{
		this->record = record;
		this->lineCount = lineCount;
	}
	void addNum(NumElement num)
	{
		if (!num.getValidity()) //invalid data
		{
			std::cout << "Invalid data at line " << lineCount + 1 << ".\n"; //print invalid data error
			isValid = false;
		}
		else //valid
		{
			record->addNum(num.getValue());
		}
	}
	void addStr(StrElement str)
	{
		record->addString(str.getValue());
	}
	NvraRecord* getRecord
};

//Encapsulates a file and its records
class File
{
private:
	OULinkedList<NvraRecord>* recordList;	//holds all the records
	std::ifstream* fileToReadFrom;			//the input file used to construct records
	struct Metadata
	{
		unsigned long linesRead = 0;		//number of lines attempted
		unsigned long validLinesRead = 0;	//number of lines with valid data
		unsigned long linesInMemory = 0;	//number of lines in memory
	};
	Metadata myMetadata;					//data about the file
public:
	//Constructor
	File()
	{
		NvraComparator* compareID = new NvraComparator(0);
		recordList = new OULinkedList<NvraRecord>(compareID);
	}
	//Attempts to get and open a file, returns false if the user presses enter only
	bool openFile()
	{
		while (!fileToReadFrom->is_open())
		{
			std::string userInput;
			std::cout << "Enter data file name: ";
			getline(std::cin, userInput);
			if (userInput == "") //check if the user just presses enter
			{
				return false;
			}
			fileToReadFrom->open(userInput); //attempt to open the file
			if (!fileToReadFrom->is_open()) //check if the file was successfully opened
			{
				std::cout << "File is not available.\n";
			}
		}
		return true;
	}
	//Set file mannually without asking the user for a file
	void setFile(std::ifstream* file)
	{
		fileToReadFrom = file;
	}
	//Add a line to the list
	void addLine(Line lineToAdd, Metadata myMetadata)
	{
		//checks for duplicates (invalid data already checked for)
	}
	//Returns the metadata of a file
	Metadata getMetaData()
	{
		return myMetadata;
	}
};

int main()
{
	//hashtable<nvrarecord>
	//ceiling inside constructor
	//expected size = given size * (1/maxloadfactor) and round up
	//expected size used to determine which element of schedule to use
	//making a table:
	//OULinkedList<T>** newTable = new OULinkedList<T>*[newBaseCapacity]; //array of pointers to linked lists
	//loop initializes the linked lists and sets them in the table
	//newTable[i] = new OULinkedList<T>(comparator); //loop over
	//newTable[bucketNumber]->insert(item);
	//sort works the same
	//find works the same for columns 1 and up, find on column 0 uses hash table
	return 0;
}
