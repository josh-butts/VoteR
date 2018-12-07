/*
VoteR4-2
*/

#include "Exceptions.h"
#include "TemplatedArray.h"
#include "NvraRecord.h"
#include "NvraComparator.h"
#include "Sorter.h"
#include "Search.h"
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

//Determines whether an element is a number or string
void setElementState(recordData* myRecordData)
{
	if (myRecordData->elementCount == 3 || myRecordData->elementCount == 11 
		|| myRecordData->elementCount == 12) myRecordData->elementIsNum = false;
	else myRecordData->elementIsNum = true;
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
		else record->addNum(tempInt); //store the int
	}
	else record->addString(recordData->element); //store the string
}

//Attempts to add a record to the list
void addRecord(NvraRecord* record, AVLTree<NvraRecord>* tree, recordData* recordData, metadata* metadata)
{
	if (recordData->isValid && !recordData->hasDuplicate)
	{
		bool succ = tree->insert(record);
		if (!succ) std::cout << "\naddRecord failed\n";
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

//Reads from a file into a linked list. Returns true if at least 1 record was read in.
bool readFile(std::ifstream* file, AVLTree<NvraRecord>* tree, metadata* mainMetadata)
{
	metadata* fileMetadata = new metadata;
	checkMemory(fileMetadata);
	NvraRecord* record = new NvraRecord; //holds a line of processed data
	checkMemory(record);
	recordData* myRecordData = new recordData;
	checkMemory(myRecordData);
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
			try
			{
				tree->find(record);
				std::cout << "Duplicate record ID " << record->getNum(0) << " at line " << fileMetadata->lineCount + 1 << ".\n";
				myRecordData->hasDuplicate = true;
			}
			catch (ExceptionAVLTreeAccess*)
			{}
			addRecord(record, tree, myRecordData, fileMetadata); //attempt to store the record
			//reset for next line:
			record = new NvraRecord;
			checkMemory(record);
			myRecordData->elementCount = 0;
			myRecordData->isValid = true;
			myRecordData->hasDuplicate = false;
		}
	}
	//update main metadata:
	mainMetadata->lineCount += fileMetadata->lineCount;
	mainMetadata->validCount += fileMetadata->validCount;
	mainMetadata->memoryCount += fileMetadata->memoryCount;
	delete record, myRecordData, fileMetadata;
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
int getInitialFile(AVLTree<NvraRecord>* tree, metadata* myMetadata)
{
	std::string userInput;
	std::ifstream* myFile = new std::ifstream;
	checkMemory(myFile);
	bool dataReadIn = false;
	while (!dataReadIn) //exit loop if we've read in at least one record
	{
		if (openFile(myFile))
		{
			if (!readFile(myFile, tree, myMetadata)) std::cout << "No valid records found."; //file yeilds no valid records
			else dataReadIn = true; //records were stored
		}
		else return 1; //user presses enter only
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
		while (treenumerator->hasNext()) std::cout << treenumerator->next() << "\n"; //print to console
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
		while (treenumerator->hasNext()) outFile << treenumerator->next() << "\n"; //print to file
		outFile << "Data lines read: " << metadata->lineCount << "; Valid NVRA records read: " << metadata->validCount
			<< "; NVRA records in memory: " << metadata->memoryCount << "\n";
		outFile.close(); //done with file
	}
	delete treenumerator;
}

//set an uninitialized array equal to this
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

//Returns the column sorted by
unsigned int sort(TemplatedArray<NvraRecord>* array)
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
	NvraComparator* column = new NvraComparator(userNum);
	Sorter<NvraRecord>::sort(*array, *column); //sort array
	return userNum;
}

//Prompts the user to enter the number to search for
unsigned int getSearchNumber()
{
	std::string userInput;
	std::cout << "Enter non-negative field value: " << std::endl;
	getline(std::cin, userInput);
	return stoi(userInput);
}

//Prompts the user to enter the string to search for
std::string getSearchString()
{
	std::string userInput;
	std::cout << "Enter exact text on which to search: \n";
	getline(std::cin, userInput);
	return userInput;
}

//Returns the index used for getString or getNum
unsigned int getSearchIndex(unsigned int column)
{
	if (column == 3) return 0; //string
	else if (column == 11) return 1; //string
	else if (column == 12) return 2; //string
	else if (column < 3) return column; //num
	else if (column < 11) return column - 1; //num
	else return column - 3; //num
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
void find(TemplatedArray<NvraRecord>* array, unsigned int sortedColumn)
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
		binarySearch(array, userNum);
	}
	else //linear search
	{
		linearSearch(array, userNum);
	}
}

//Returns a tree of the resulting merge of the trees
AVLTree<NvraRecord>* mergeTrees(AVLTree<NvraRecord>* mainTree, AVLTree<NvraRecord>* fileTree, metadata* mainMetadata)
{
	//keep metadata in mind when merging
	//Following code adapted from Cameron Bost, the best TA in existence
	NvraComparator* compareID = new NvraComparator(0);
	AVLTree<NvraRecord>* mergeTree = new AVLTree<NvraRecord>(compareID);
	AVLTreeEnumerator<NvraRecord>* mainEnum = new AVLTreeEnumerator<NvraRecord>(mainTree, AVLTreeOrder::inorder);
	AVLTreeEnumerator<NvraRecord>* fileEnum = new AVLTreeEnumerator<NvraRecord>(fileTree, AVLTreeOrder::inorder);
	mainMetadata->memoryCount = 0;
	while (mainEnum->hasNext() && fileEnum->hasNext()) //while they both have items
	{
		if (compareID->compare(mainEnum->peek(), fileEnum->peek) == 0) //if the two match
		{
			mergeTree->insert(new NvraRecord(fileEnum->next())); //take the file's item
			mainEnum->next(); //skip the main's item
			++mainMetadata->memoryCount;
		}
		else if (compareID->compare(mainEnum->peek(), fileEnum->peek) == -1) //if main is less than file
		{
			mergeTree->insert(new NvraRecord(mainEnum->next())); //take the main's item
			++mainMetadata->memoryCount;
		}
		else if (compareID->compare(mainEnum->peek(), fileEnum->peek) == 1) //if file is less than main
		{
			mergeTree->insert(new NvraRecord(fileEnum->next())); //take the file's item
			++mainMetadata->memoryCount;
		}
	}
	while (mainEnum->hasNext())
	{
		mergeTree->insert(new NvraRecord(mainEnum->next()));
		++mainMetadata->memoryCount;
	}
	while (fileEnum->hasNext())
	{
		mergeTree->insert(new NvraRecord(fileEnum->next()));
		++mainMetadata->memoryCount;
	}
	return mergeTree;
}

//Gets a file from the user, reads the data, merges the data with the main data
bool merge(AVLTree<NvraRecord>* mainTree, metadata* metadata)
{
	std::ifstream* myFile = new std::ifstream;
	NvraComparator* compareID = new NvraComparator(0);
	AVLTree<NvraRecord>* fileTree = new AVLTree<NvraRecord>(compareID); //holds data to be merged
	if (!openFile(myFile)) return false; //if the user just presses enter
	readFile(myFile, fileTree, metadata); //read into the fileTree
	mainTree = mergeTrees(mainTree, fileTree, metadata); //merge the trees
	delete myFile, fileTree;
	return true;
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

int mutatorLoop(std::string userInput, AVLTree<NvraRecord>* mainTree, metadata* mainMetadata,
	TemplatedArray<NvraRecord>* mainArray, unsigned int sortedColumn)
{
	userInput = mutatorLoopPrompt(); //priming read
	while (userInput != "q")
	{
		if (userInput == "o")
		{
			output(mainTree, mainMetadata);
		}
		else if (userInput == "s")
		{
			sortedColumn = sort(mainArray);
		}
		else if (userInput == "f")
		{
			find(mainArray, sortedColumn);
		}
		else if (userInput == "m")
		{
			merge(mainTree, mainMetadata);
		}
		else if (userInput == "p")
		{
			std::cout << "\nnope\n";
		}
		else if (userInput == "h")
		{
			std::cout << "\nnope\n";
		}
		else if (userInput == "pre")
		{

		}
		else if (userInput == "in")
		{

		}
		else if (userInput == "post")
		{

		}
		else
		{
			std::cout << "Your input did not match any option.\n";
		}
		userInput = mutatorLoopPrompt();
	}
	std::cout << "Thanks for using VoteR.\n";
	return 0;
}

int main()
{
	metadata* mainMetadata = new metadata; //contains the metadata from running the program
	checkMemory(mainMetadata);
	NvraComparator* compareID = new NvraComparator(0);
	checkMemory(compareID);
	AVLTree<NvraRecord>* mainTree = new AVLTree<NvraRecord>(compareID);
	checkMemory(mainTree);
	TemplatedArray<NvraRecord>* mainArray = new TemplatedArray<NvraRecord>;
	checkMemory(mainArray);
	unsigned int sortedColumn = 0; //the column the data is sorted on in mainArray, id is default
	std::string userInput; //contains what the user types into the console

	if (getInitialFile(mainTree, mainMetadata) == 1) return 0; //get the initial file, if the user just presses enter then close voteR
	mainArray = treeToArray(mainTree); //update mainArray

	return mutatorLoop(userInput, mainTree, mainMetadata, mainArray, sortedColumn);
}
