#ifndef OU_LINKED_LIST_ENUMERATOR
#define OU_LINKED_LIST_ENUMERATOR

#include "Enumerator.h"
#include "OULink.h"

template <typename T>
class OULinkedListEnumerator : public Enumerator<T>
{
private:
	OULink<T>* current;
public:
	OULinkedListEnumerator(OULink<T>* first);
	bool hasNext() const;
	T next();
	T peek() const;
};

// Implementation goes here
template <typename T>
OULinkedListEnumerator<T>::OULinkedListEnumerator(OULink<T>* first)
{
	current = first;
}

template <typename T>
bool OULinkedListEnumerator<T>::hasNext() const
{
	if (current == NULL) //doesn't exist
	{
		return false;
	}
	else //next exists
	{
		return true;
	}
}

template <typename T>
T OULinkedListEnumerator<T>::next()
{
	if (current != NULL)
	{
		T temp = *current->data; //store what we want to return
		current = current->next; //advance to the next node
		return temp; //return the node we advanced from
	}
	else
	{
		throw new ExceptionEnumerationBeyondEnd();
	}
}

template <typename T>
T OULinkedListEnumerator<T>::peek() const
{
	if (current != NULL)
	{
		return *current->data; //return the node data without advancing
	}
	else
	{
		throw new ExceptionEnumerationBeyondEnd();
	}
}

#endif // !OU_LINKED_LIST_ENUMERATOR