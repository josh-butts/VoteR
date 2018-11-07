#ifndef OU_LINK_H
#define OU_LINK_H

template <typename T>
class OULink {
	template <typename U> //CHANGING T TO U SUPRESSES AN ERROR IN ZYLABS
	friend class OULinkedList;
	template <typename U> //CHANGING T TO U SUPRESSES AN ERROR IN ZYLABS
	friend class OULinkedListEnumerator;
private:
	T* data = NULL;										// pointer to data item of any type
	OULink* next = NULL;								// pointer to next link
public:
	OULink(const T* item);
	virtual ~OULink();
};

// Implementation goes here
template <typename T>
OULink<T>::OULink(const T* item)
{
	data = new T(*item);
}

template <typename T>
OULink<T>::~OULink()
{
	delete data;
	next = NULL;
}

#endif // !OU_LINK_H