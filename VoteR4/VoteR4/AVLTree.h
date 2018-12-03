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
	void zig();										// right rotation
	void zag();										// left rotation
	void zigzag();									// left rotation on left subtree, followed by right rotation
	void zagzig();									// right rotation on right subtree, followed by left rotation
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

// add public functions here


// add additional private functions here

#endif // !AVL_TREE
