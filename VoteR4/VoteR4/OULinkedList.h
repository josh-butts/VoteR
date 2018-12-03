#ifndef OU_LINKED_LIST
#define OU_LINKED_LIST

#include "OULink.h"
#include "Comparator.h"
#include "OULinkedListEnumerator.h"

// OULinkedList stands for Ordered, Unique Linked List. It is a linked list that is always maintained in
// order (based on the comparator provided to it when the list is created) and that only contains unique
// items (that is, duplicates are not allowed)
template <typename T>
class OULinkedList {
	template <typename U> //CHANGING T TO U SUPRESSES AN ERROR IN ZYLABS
	friend class OULinkedListEnumerator;
private:
	Comparator<T>* comparator = NULL;				// used to determine list order and item equality
	unsigned long size = 0;							// actual number of items currently in list
	OULink<T>* first = NULL;						// pointer to first link in list
	OULink<T>* last = NULL;							// pointer to last link in list
public:
	OULinkedList(Comparator<T>* comparator);		// creates empty linked list with comparator
	virtual ~OULinkedList();						// deletes all links and their data items

	// if an equivalent item is not already present, insert item in order and return true
	// if an equivalent item is already present, leave list unchanged and return false
	bool insert(const T* item);

	// if item is greater than item at last, append item at end and return true
	// if item is less than or equal to item at last, leave list unchanged and return false
	bool append(const T* item);

	// if an equivalent item is already present, replace item and return true
	// if an equivalent item is not already present, leave list unchanged and return false
	bool replace(T* item);

	// if an equivalent item is already present, remove item and return true
	// if an equivalent item is not already present, leave list unchanged and return false
	bool remove(T* item);

	// if any items are present, return a copy of the first item
	// if no items are present, throw new ExceptionLinkedListAccess
	T get() const;

	// if an equivalent item is present, return a copy of the first such item
	// if an equivalent item is not present, throw a new ExceptionLinkedListAccess
	T find(const T* item) const;

	unsigned long getSize() const;					// returns the current number of items in the list

	OULinkedListEnumerator<T> enumerator() const;	// create an enumerator for this linked list
};

template <typename T>
OULinkedList<T>::OULinkedList(Comparator<T>* comparator)
{
	this->comparator = comparator;
}

template <typename T>
OULinkedList<T>::~OULinkedList()
{
	comparator = NULL;
	OULink<T>* current = first;
	OULink<T>* next = first->next;
	while (next != NULL)
	{
		delete current;
		current = next;
		next = next->next;
	}

}

template <typename T>
bool OULinkedList<T>::insert(const T* item)
{
	OULink<T>* linkItem = new OULink<T>(item);
	if (first == NULL) //empty list
	{
		first = linkItem;
		last = linkItem;
		++size;
		return true;
	}
	if (comparator->compare(*first->data, *item) > 0) //insert before first node
	{
		linkItem->next = first; //the new node's next node is the old first node
		first = linkItem; //the first node is updated to the new node
		++size;
		return true;
	}
	OULink<T>* current = first;
	while (current->next != NULL) //instert between middle nodes
	{
		if (comparator->compare(*current->data, *linkItem->data) < 0
			&& comparator->compare(*current->next->data, *linkItem->data) > 0)
		{
			linkItem->next = current->next; //new node's next points to current node's next
			current->next = linkItem; //current node's next points to new node
			++size;
			return true;
		}
		current = current->next;
	}
	if (comparator->compare(*last->data, *linkItem->data) < 0) //insert after last node
	{
		last->next = linkItem; //point the old last item's next to the new last item
		last = linkItem; //update old last to the new last
		++size;
		return true;
	}
	else //item already exists in the list
	{
		return false;
	}
}

template <typename T>
bool OULinkedList<T>::append(const T* item)
{
	if (first == NULL) //empty
	{
		OULink<T>* linkItem = new OULink<T>(item); //link we want to add
		first = linkItem;
		last = linkItem;
		++size;
		return true;
	}
	if (comparator->compare(*last->data, *item) >= 0) //item is not bigger than the last data
	{
		return false;
	}
	else //item is bigger than the last data
	{
		OULink<T>* linkItem = new OULink<T>(item); //link we want to add
		last->next = linkItem; //point the old last item's next to the new last item
		last = linkItem; //update old last to the new last
		++size;
		return true;
	}
}

template <typename T>
bool OULinkedList<T>::replace(T* item)
{
	/*
	OULink<T>* current = first;
	while (current != NULL)
	{
		if (comparator->compare(*current->data, *item) == 0) //if they match
		{
			OULink<T>* linkItem = new OULink<T>(item);
			current = linkItem; //replace it
			return true;
		}
		current = current->next;
	}
	return false; //item not found
	*/
	if (remove(item))
	{
		insert(item);
		return true;
	}
	return false;
}

template <typename T>
bool OULinkedList<T>::remove(T* item)
{
	if (comparator->compare(*first->data, *item) == 0) //first node
	{
		first = first->next;
		--size;
		return true;
	}
	OULink<T>* current = first->next;
	OULink<T>* previous = first;
	while (current->next != NULL) //middle nodes
	{
		if (comparator->compare(*current->data, *item) == 0)
		{
			previous->next = current->next; //previous's next no longer points to current. It points to current's next
			current->next = NULL; //nothing points to current and current points to nothing
			--size;
			return true;
		}
		previous = current;
		current = current->next;
	}
	if (comparator->compare(*last->data, *item) == 0) //last node
	{
		last = previous; //previous is the next to last node
		--size;
		return true;
	}
	return false;
}

template <typename T>
T OULinkedList<T>::get() const
{
	if (first == NULL) //empty
	{
		throw new ExceptionLinkedListAccess();
	}
	else //not empty
	{
		return *first->data; //return the data of the first link
	}
}

template <typename T>
T OULinkedList<T>::find(const T* item) const
{
	OULink<T>* current = first; 
	while (current != NULL) //stops after last node is checked
	{
		if (comparator->compare(*current->data, *item) == 0) //if its the same, return it
		{
			return *current->data;
		}
		else //if it isnt the same, advance to the next node
		{
			current = current->next;
		}
	}
	//item wasn't found
	throw new ExceptionLinkedListAccess();
}

template <typename T>
unsigned long OULinkedList<T>::getSize() const
{
	return size;
}

template <typename T>
OULinkedListEnumerator<T> OULinkedList<T>::enumerator() const
{
	return OULinkedListEnumerator<T>(first);
}

#endif // !OU_LINKED_LIST