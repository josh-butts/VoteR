/*
Josh Butts
Project 4 VoteR 4.0
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
#include "HashTable.h"
#include "NvraHasher.h"
#include "HashTableEnumerator.h"
#include "AVLTree.h"
#include "AVLTreeEnumerator.h"
#include "AVLTreeOrder.h"
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

//Returns a list made from an array
OULinkedList<NvraRecord>* arrayToList(TemplatedArray<NvraRecord>* array)
{
	NvraComparator* compareID = new NvraComparator(0); //comparator based on the id column
	OULinkedList<NvraRecord>* list = new OULinkedList<NvraRecord>(compareID);
	for (unsigned int i = 0; i < array->getSize(); ++i)
	{
		list->insert(new NvraRecord(array->get(i)));
	}
	return list;
}

//Returns an array made from a list
TemplatedArray<NvraRecord>* listToArray(OULinkedList<NvraRecord>* list)
{
	TemplatedArray<NvraRecord>* array = new TemplatedArray<NvraRecord>(list->getSize());
	OULinkedListEnumerator<NvraRecord> listEnum = list->enumerator();
	while (listEnum.hasNext())
	{
		array->add(new NvraRecord(listEnum.next()));
	}
	return array;
}

//Determines whether an element is a number or string
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
void addRecord(NvraRecord* record, AVLTree<NvraRecord>* tree, TemplatedArray<unsigned int>* ids, recordData* recordData,
	metadata* metadata)
{
	if (recordData->isValid && !recordData->hasDuplicate)
	{
		tree->insert(record);
		ids->add(new unsigned int(record->getNum(0)));
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
			std::cout << "Invalid data at line " << metadata->lineCount + 1 << ".\n"; //print invalid data error
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
bool readFile(std::ifstream* file, AVLTree<NvraRecord>* tree, metadata* mainMetadata, TemplatedArray<unsigned int>* ids)
{
	metadata* fileMetadata = new metadata;
	NvraRecord* record = new NvraRecord; //holds a line of processed data
	checkMemory(record);
	recordData* myRecordData = new recordData;
	getline(*file, myRecordData->element); //throw out header
	while (getline(*file, myRecordData->element, ',')) //gets elements separated by commas, the last element is a special case
	{
		setElementState(myRecordData);
		addElement(record, fileMetadata, myRecordData);
		++myRecordData->elementCount;
		if (myRecordData->elementCount == 23) //last element
		{
			getline(*file, myRecordData->element, '\n'); //gets last element
			addElement(record, fileMetadata, myRecordData);
			for (unsigned long i = 0; i < ids->getSize(); ++i) //check for duplicate ids
			{
				if (ids->get(i) == record->getNum(0)) //if the id matches another id
				{
					std::cout << "Duplicate record ID " << ids->get(i) << " at line " << fileMetadata->lineCount + 1 << ".\n";
					myRecordData->hasDuplicate = true;
				}
			}
			addRecord(record, tree, ids, myRecordData, fileMetadata); //attempt to store the record
			//reset for next line:
			record = new NvraRecord;
			myRecordData->elementCount = 0;
			myRecordData->isValid = true;
			myRecordData->hasDuplicate = false;
		}
	}
	//update main metadata:
	mainMetadata->lineCount += fileMetadata->lineCount;
	mainMetadata->validCount += fileMetadata->validCount;
	mainMetadata->memoryCount += fileMetadata->memoryCount;
	delete record;
	delete myRecordData;
	delete fileMetadata;
	if (tree->getSize() > 0) return true; //return true if a record was read in
	return false;
}

//Attempts to get and open a file, returns false if the user presses enter only
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
int getUserFile(AVLTree<NvraRecord>* tree, metadata* myMetadata)
{
	std::string userInput;
	std::ifstream* myFile = new std::ifstream;
	bool dataReadIn = false;
	while (!dataReadIn) //exit loop if we've read in at least one record
	{
		if (openFile(myFile))
		{
			TemplatedArray<unsigned int>* ids = new TemplatedArray<unsigned int>; //holds ids
			checkMemory(ids);
			if (!readFile(myFile, tree, myMetadata, ids)) //file yeilds no valid records
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

//Outputs data line by line, with stats at the end
void output(AVLTree<NvraRecord>* tree, metadata* metadata) //TODO: make an output file if the user's file doesn't exist???
{
	AVLTreeEnumerator<NvraRecord>* treenumerator = new AVLTreeEnumerator<NvraRecord>(tree, AVLTreeOrder::inorder);
	std::string userInput;
	std::cout << "Enter output file name: ";
	getline(std::cin, userInput);
	if (userInput == "") //user presses enter, print to standard out
	{
		while (treenumerator->hasNext())
		{
			std::cout << treenumerator->next() << "\n"; //print to console
		}
		std::cout << "Data lines read: " << metadata->lineCount << "; Valid NVRA records read: " << metadata->validCount
			<< "; NVRA records in memory: " << metadata->memoryCount << "\n";
	}
	else //user inputs a string
	{
		std::ofstream outFile;
		outFile.open(userInput);
		while (!outFile.is_open())
		{
			std::cout << "File is not available." << std::endl; //display error
			std::cout << "Enter output file name: ";
			getline(std::cin, userInput);
		}
		while (treenumerator->hasNext())
		{
			outFile << treenumerator->next() << "\n"; //print to file
		}
		outFile << "Data lines read: " << metadata->lineCount << "; Valid NVRA records read: " << metadata->validCount
			<< "; NVRA records in memory: " << metadata->memoryCount << "\n";
		outFile.close(); //done with file
	}
	delete treenumerator;
}

TemplatedArray<NvraRecord>* treeToArray(AVLTree<NvraRecord>* tree)
{
	TemplatedArray<NvraRecord>* array = new TemplatedArray<NvraRecord>(tree->getSize()); //initialize with proper size to prevent resizing
	AVLTreeEnumerator<NvraRecord>* treenumerator = new AVLTreeEnumerator<NvraRecord>(tree, AVLTreeOrder::inorder);
	while (treenumerator->hasNext())
	{
		array->add(new NvraRecord(treenumerator->next())); //add the items
	}
	return array;
}

AVLTree<NvraRecord>* arrayToTree(TemplatedArray<NvraRecord>* array, NvraComparator* column)
{
	AVLTree<NvraRecord>* tree = new AVLTree<NvraRecord>(column);
	for (unsigned long i = 0; i < array->getSize(); ++i)
	{
		tree->insert(new NvraRecord(array->get(i))); //add the items
	}
	return tree;
}

//Returns the column sorted by
unsigned int sort(AVLTree<NvraRecord>* tree)
{
	std::string userInput;
	std::cout << "Enter sort field (0-23): \n";
	getline(std::cin, userInput);
	unsigned int userNum = stoi(userInput);
	while (userNum > 23) //while input is invalid
	{
		std::cout << "Invalid entry.\nEnter sort field (0-23): \n";
		getline(std::cin, userInput);
	}
	TemplatedArray<NvraRecord>* array; //make an array
	array = treeToArray(tree); //give the array the tree's data
	NvraComparator* column = new NvraComparator(userNum);
	Sorter<NvraRecord>::sort(*array, *column); //sort array
	delete tree;
	tree = arrayToTree(array, column); //create tree with specified comparator
	return userNum;
}

//Prompts the user to enter the string to search for
std::string getSearchString()
{
	std::string userInput;
	std::cout << "Enter exact text on which to search: \n";
	getline(std::cin, userInput);
	return userInput;
}

//Prompts the user to enter the number to search for
unsigned int getSearchNumber()
{
	std::string userInput;
	std::cout << "Enter non-negative field value: " << std::endl;
	getline(std::cin, userInput);
	return stoi(userInput);
}

//Returns the index used for getString or getNum
unsigned int getSearchIndex(unsigned int column)
{
	if (column == 3) //string
	{
		return 0;
	}
	else if (column == 11) //string
	{
		return 1;
	}
	else if (column == 12) //string
	{
		return 2;
	}
	else if (column < 3) //num
	{
		return column;
	}
	else if (column < 11) //num
	{
		return column - 1;
	}
	else //num
	{
		return column - 3;
	}
}

//Uses linear search to find and print records
void linearSearch(TemplatedArray<NvraRecord>* array, unsigned int column)
{
	unsigned int recordsFound = 0;
	unsigned int index = getSearchIndex(column);
	if (column == 3 || column == 11 || column == 12) //string
	{
		std::string searchTerm = getSearchString();
		for (unsigned long i = 0; i < array->getSize(); ++i)
		{
			if (array->get(i).getString(index) == searchTerm)
			{
				std::cout << array->get(i) << "\n";
				++recordsFound;
			}
		}
		std::cout << "NVRA records found: " << recordsFound << ".\n";
	}
	else //number
	{
		unsigned int searchNum = getSearchNumber();
		for (unsigned long i = 0; i < array->getSize(); ++i)
		{
			if (array->get(i).getNum(index) == searchNum)
			{
				std::cout << array->get(i) << "\n";
				++recordsFound;
			}
		}
		std::cout << "NVRA records found: " << recordsFound << ".\n";
	}
}

//Uses binary search to find and print records
void binarySearch(TemplatedArray<NvraRecord>* array, unsigned int column)
{
	unsigned int recordsFound = 0;
	unsigned int index = getSearchIndex(column);
	if (column == 3 || column == 11 || column == 12) //string
	{
		std::string searchTerm = getSearchString();
		NvraRecord searchRecord; //record to search for
		searchRecord.setString(searchTerm, index);
		Comparator<NvraRecord>* compareColumn = new NvraComparator(column);
		int location = (int)binarySearch(searchRecord, *array, *compareColumn);
		if (location >= 0) //found
		{
			std::cout << array->get(location) << "\n";
			++recordsFound;
			while (location < (int)array->getSize() - 1
				&& array->get(location).getString(index) == array->get(location + 1).getString(index)) //check the next record
			{
				std::cout << array->get(location + 1) << "\n";
				++location;
				++recordsFound;
			}
		}
		std::cout << "NVRA records found: " << recordsFound << ".\n";
	}
	else //number
	{
		unsigned int searchNum = getSearchNumber();
		NvraRecord searchRecord; //record to search for
		searchRecord.setNum(searchNum, index);
		Comparator<NvraRecord>* compareColumn = new NvraComparator(column);
		int location = (int)binarySearch(searchRecord, *array, *compareColumn);
		if (location >= 0) //found
		{
			std::cout << array->get(location) << "\n";
			++recordsFound;
			while (location < (int)array->getSize() - 1
				&& array->get(location).getNum(index) == array->get(location + 1).getNum(index)) //check the next record
			{
				std::cout << array->get(location + 1) << "\n";
				++location;
				++recordsFound;
			}
		}
		std::cout << "NVRA records found: " << recordsFound << ".\n";
	}
}

//Prints records based on search field and search term
void find(AVLTree<NvraRecord>* tree, unsigned int sortedColumn)
{
	//TODO: if searching by id, use hash table
	std::string userInput;
	std::cout << "Enter search field (0-23): " << std::endl;
	getline(std::cin, userInput);
	unsigned int userNum = stoi(userInput);
	while (userNum > 23) //while input is invalid
	{
		std::cout << "Invalid entry.\nEnter search field (0-23): \n";
		getline(std::cin, userInput);
		userNum = stoi(userInput);
	}
	if (userNum == sortedColumn) //binary search
	{
		//binarySearch(array, userNum);
	}
	else //linear search
	{
		//linearSearch(array, userNum);
	}
}

//updates the table to be accurate with the array
HashTable<NvraRecord>* arrayToTable(TemplatedArray<NvraRecord>* mainArray)
{
	Hasher<NvraRecord>* hasher = new NvraHasher();
	Comparator<NvraRecord>* htComparator = new NvraComparator(0);
	HashTable<NvraRecord>* newTable = new HashTable<NvraRecord>(htComparator, hasher, mainArray->getSize());
	for (unsigned long i = 0; i < mainArray->getSize(); ++i)
	{
		newTable->insert(new NvraRecord(mainArray->get(i)));
	}
	return newTable;
}

//Merges the data from secArray into mainArray
void mergeArrays(TemplatedArray<NvraRecord>* mainArray, TemplatedArray<NvraRecord>* secArray, TemplatedArray<unsigned int>* secIds,
	metadata* mainMetadata)
{
	Comparator<NvraRecord>* idCompare = new NvraComparator(0);
	NvraRecord searchRecord;
	int replaceIndex;
	for (unsigned long i = 0; i < secIds->getSize(); ++i)
	{
		searchRecord = secArray->get(i); //get a record from the array
		replaceIndex = (int)binarySearch(searchRecord, *mainArray, *idCompare); //see if its id exists in the mainArray
		if (replaceIndex >= 0) //record found with matching id in mainArray
		{
			mainArray->replaceAt(new NvraRecord(searchRecord), (unsigned long)replaceIndex); //replace matching record
			//memoryCount starts out assuming that all records will be added, so if one is replaced then memoryCount should decrement
			--mainMetadata->memoryCount;
		}
		else //record isn't in mainArray
		{
			mainArray->add(new NvraRecord(searchRecord)); //add it
		}
	}
}

//Gets a file from the user, reads the data, merges the data with the main data
bool merge(AVLTree<NvraRecord>* mainTree, metadata* metadata)
{
	std::ifstream* myFile = new std::ifstream;
	NvraComparator* compareID = new NvraComparator(0);
	AVLTree<NvraRecord>* fileTree = new AVLTree<NvraRecord>(compareID); //holds data to be merged
	TemplatedArray<unsigned int>* ids = new TemplatedArray<unsigned int>; //holds ids
	checkMemory(ids);
	if (!openFile(myFile)) //if the user just presses enter
	{
		return false;
	}
	readFile(myFile, fileTree, metadata, ids);
	//mergeArrays(mainArray, array, ids, metadata);
	//if (mainArray->getSize() > 0)
	//{
	//	NvraComparator* column = new NvraComparator(0);
	//	Sorter<NvraRecord>::sort(*mainArray, *column); //sort by id
	//}
	delete myFile;
	return true;
}

//Purges the data in mainArray that matches data in secArray
void purgeArrays(TemplatedArray<NvraRecord>* mainArray, TemplatedArray<NvraRecord>* secArray, TemplatedArray<unsigned int>* secIds,
	metadata* mainMetadata)
{
	Comparator<NvraRecord>* idCompare = new NvraComparator(0);
	NvraRecord searchRecord;
	int replaceIndex;
	for (unsigned long i = 0; i < secIds->getSize(); ++i)
	{
		searchRecord = secArray->get(i); //get a record from the array
		replaceIndex = (int)binarySearch(searchRecord, *mainArray, *idCompare); //see if its id exists in the mainArray
		if (replaceIndex >= 0) //record found with matching id in mainArray
		{
			mainArray->removeAt((unsigned long)replaceIndex); //remove the matching record
			--mainMetadata->memoryCount;
		}
	}
}

//Gets a file from the user, reads the data, purges any records from mainArray that match those in the user's file
bool purge(TemplatedArray<NvraRecord>* mainArray, metadata* metadata)
{
	std::ifstream* myFile = new std::ifstream;
	unsigned long startingMemoryCount = metadata->memoryCount;
	TemplatedArray<NvraRecord>* array = new TemplatedArray<NvraRecord>; //holds data to be merged
	checkMemory(array);
	TemplatedArray<unsigned int>* ids = new TemplatedArray<unsigned int>; //holds ids
	checkMemory(ids);
	if (!openFile(myFile)) //if the user just presses enter
	{
		return false;
	}
	//readFile(myFile, array, metadata, ids);
	//readFile assumes that the data will be stored so memoryCount must be restored to before the file was read in
	metadata->memoryCount = startingMemoryCount;
	purgeArrays(mainArray, array, ids, metadata);
	if (mainArray->getSize() > 0)
	{
		NvraComparator* column = new NvraComparator(0);
		Sorter<NvraRecord>::sort(*mainArray, *column); //sort by id
	}
	delete myFile;
	return true;
}

//Prints the records line by line and data about the hash table
void printHashTable(HashTable<NvraRecord>* table, metadata* metadata)
{
	std::string userInput;
	std::cout << "Enter output file name: ";
	getline(std::cin, userInput);
	if (userInput == "") //user presses enter, print to standard out
	{
		table->printTable();
		std::cout << "Data lines read: " << metadata->lineCount << "; Valid NVRA records read: " << metadata->validCount
			<< "; NVRA records in memory: " << metadata->memoryCount << "\n";
	}
	else
	{
		std::ofstream* outFile = new std::ofstream;
		outFile->open(userInput);
		while (!outFile->is_open())
		{
			std::cout << "File is not available." << std::endl; //display error
			std::cout << "Enter output file name: ";
			getline(std::cin, userInput);
		}
		table->printTable(outFile);
		*outFile << "Data lines read: " << metadata->lineCount << "; Valid NVRA records read: " << metadata->validCount
			<< "; NVRA records in memory: " << metadata->memoryCount << "\n";
		outFile->close(); //done with file
		delete outFile;
	}
}

//Prompt the user for what action they want
std::string mutatorLoopPrompt()
{
	std::string userInput;
	std::cout << "Enter (o)utput, (s)ort, (f)ind, (m)erge, (p)urge, (h)ash table, " 
		<< "(pre)order, (in)order, (post)order, or (q)uit: ";
	getline(std::cin, userInput);
	return userInput;
}

int main()
{
	NvraComparator* compareID = new NvraComparator(0);
	AVLTree<NvraRecord>* tree = new AVLTree<NvraRecord>(compareID);
	metadata* mainMetadata = new metadata; //contains the metadata of running the program
	//TemplatedArray<NvraRecord>* mainArray = new TemplatedArray<NvraRecord>; //holds all the lines
	TemplatedArray<unsigned int>* mainIds = new TemplatedArray<unsigned int>; //holds ids
	checkMemory(mainIds);
	Hasher<NvraRecord>* hasher = new NvraHasher(); //used for hash function
	checkMemory(hasher);
	Comparator<NvraRecord>* htComparator = new NvraComparator(0); //used in constructing hash tables
	checkMemory(htComparator);
	//HashTable<NvraRecord>* hTable; //contains every record, used for fast searching by id
	std::string userInput; //contains what the user types into the console
	unsigned int sortedColumn = 0; //the column the data is sorted on, id is default
	if (getUserFile(tree, mainMetadata) == 1) return 0; //get the initial file, if the user just presses enter then close voteR
	//hTable = new HashTable<NvraRecord>(htComparator, hasher, mainArray->getSize()); //initialize table
	//hTable = arrayToTable(mainArray); //update the table
	userInput = mutatorLoopPrompt(); //priming read
	while (userInput != "q")
	{
		if (userInput == "o") output(tree, mainMetadata); //output
		else if (userInput == "s") sortedColumn = sort(tree); //sort
		else if (userInput == "f") find(tree, sortedColumn); //find
		else if (userInput == "m") //merge
		{
			merge(tree, mainMetadata);
			//hTable = arrayToTable(mainArray);
		}
		else if (userInput == "p") //purge
		{
			//purge(tree, mainMetadata);
			//hTable = arrayToTable(mainArray);
		}
		//else if (userInput == "h") printHashTable(hTable, mainMetadata); //hash table
		else
		{
			std::cout << "Your input did not match any option.\n";
		}
		userInput = mutatorLoopPrompt();
	}
	std::cout << "Thanks for using VoteR.\n";
	return 0;
}
