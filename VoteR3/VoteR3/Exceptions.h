#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

class Exception {};
class ExceptionIndexOutOfRange : Exception {};
class ExceptionMemoryNotAvailable : Exception {};
class ExceptionLinkedListAccess : Exception {};
//thrown by next or peek when hasNext is false (regardless of enumerated data structure)
class ExceptionEnumerationBeyondEnd : Exception {};
//thrown by find in HashTable if item not found; also by HashTableEnumerator for creating a HashTableEnumerator on an empty HashTable
class ExceptionHashTableAccess : Exception {};

#endif
