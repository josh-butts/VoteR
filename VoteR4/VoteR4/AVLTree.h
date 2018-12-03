#ifndef AVL_TREE
#define AVL_TREE

#include "Comparator.h"

template <typename T>
class AVLTree {
	template <typename U>
	friend class AVLTreeEnumerator;
private:
	Comparator<T>* comparator = NULL;				// used to determine tree order and item equality
	unsigned long size = 0;							// actual number of items currently in tree
	T* data = NULL;									// pointer to data at this node
	int diff = 0;									// height of right minus height of left
	AVLTree<T>* left = NULL;						// pointer to left subtree
	AVLTree<T>* right = NULL;						// pointer to right subtree
	void right();									// right rotation
	void left();									// left rotation
	void rightLeft();								// left rotation on left subtree, followed by right rotation
	void leftRight();								// right rotation on right subtree, followed by left rotation
	void rebalance();								// check for and rebalance this node, if needed
public:
	AVLTree(Comparator<T>* comparator);				// creates empty linked tree with comparator
	virtual ~AVLTree();								// deletes all links and their data items

	// if an equivalent item is not already present, insert item in order and return true
	// if an equivalent item is already present, leave tree unchanged and return false
	bool insert(const T* item);

	// if an equivalent item is already present, replace item and return true
	// if an equivalent item is not already present, leave tree unchanged and return false
	bool replace(const T* item);

	// if an equivalent item is already present, remove item and return true
	// if an equivalent item is not already present, leave tree unchanged and return false
	bool remove(const T* item);

	// if an equivalent item is present, return a copy of the item
	// if an equivalent item is not present, throw a new ExceptionLinkedtreeAccess
	T find(const T* item) const;

	unsigned long getSize() const;				// returns the current number of items in the tree
};

//Public:
template <typename T>
AVLTree<T>::AVLTree(Comparator<T>* comparator)
{
	this->comparator = comparator;

}

template <typename T>
AVLTree<T>::~AVLTree()
{

}

template <typename T>
bool AVLTree<T>::insert(const T* item)
{

}

template <typename T>
bool AVLTree<T>::replace(const T* item)
{

}

template <typename T>
bool AVLTree<T>::remove(const T* item)
{

}

template <typename T>
T AVLTree<T>::find(const T* item) const
{

}

template <typename T>
unsigned long AVLTree<T>::getSize() const
{

}

//Private:
template <typename T>
void AVLTree<T>::right()
{

}

template <typename T>
void AVLTree<T>::left()
{

}

template <typename T>
void AVLTree<T>::rightLeft()
{

}

template <typename T>
void AVLTree<T>::leftRight()
{

}

template <typename T>
void AVLTree<T>::rebalance()
{

}

#endif // !AVL_TREE
