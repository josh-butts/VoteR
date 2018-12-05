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
	void zigRight();								// right rotation
	void zagLeft();									// left rotation
	void zigRightZagLeft();							// left rotation on left subtree, followed by right rotation
	void zagLeftZigRight();							// right rotation on right subtree, followed by left rotation
	void rebalance();								// check for and rebalance this node, if needed
	int getHeight();
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
	if (right != NULL) delete right;
	if (left != NULL) delete left;
	right = NULL;
	left = NULL;
	data = NULL;
	comparator = NULL;
	size = NULL;
	diff = NULL;
}

template <typename T>
bool AVLTree<T>::insert(const T* item) //TODO: this does not contain rebalancing code
{
	if (data == NULL) //has no data, base case
	{
		data = item; //give it the data
		++size;	//increment size
		//rebalance();
		return true;
	}
	else if (comparator->compare(data, item) == -1) //subtree belongs to the right
	{
		if (right == NULL) //if there is no right subtree
		{
			right = new AVLTree<T>(comparator); //initialize it
		}
		return right->insert(item); //insert the item on the right
	}
	else if (comparator->compare(data, item) == 1) //subtree belongs to the left
	{
		if (left == NULL) //there is no left
		{
			left = new AVLTree<T>(comparator); //initialize it
		}
		return left->insert(item); //insert item on the left
	}
	else return false; //data already exists
}

template <typename T>
bool AVLTree<T>::replace(const T* item)
{
	if (data == NULL) return false; //data not found
	else if (comparator->compare(data, item) == -1) //subtree belongs to the right
	{
		if (right == NULL) return false; //no right subtree
		return right->find(item); //look on the right
	}
	else if (comparator->compare(data, item) == 1) //subtree belongs to the left
	{
		if (left == NULL) return false; //no left subtree
		return left->find(item); //look on the left
	}
	else //item matches subtree data
	{
		data = item; //replace it
		return true;
	}
}

template <typename T>
bool AVLTree<T>::remove(const T* item) //EXTRA CREDIT
{
	if (data == NULL) return false; //data not found


}

template <typename T>
T AVLTree<T>::find(const T* item) const
{
	if (data == NULL) return false; //data not found
	else if (comparator->compare(data, item) == -1) //subtree belongs to the right
	{
		if (right == NULL) return false; //no right subtree
		return right->find(item); //look on the right
	}
	else if (comparator->compare(data, item) == 1) //subtree belongs to the left
	{
		if (left == NULL) return false; //no left subtree
		return left->find(item); //look on the left
	}
	else return data; //item matches subtree data
}

template <typename T>
unsigned long AVLTree<T>::getSize() const
{
	return size;
}

//Private:
template <typename T>
void AVLTree<T>::zigRight()
{
	if (left == NULL) {  // no left, no way to zig
		return;
	}
	// keep track of diff of node and left for changes
	int gdiff = diff;
	int pdiff = left->diff;

	// modify the tree
	AVLTree<T>* olnr = left;  // olnr is "old left, new right"
	left = olnr->left;
	olnr->left = olnr->right;
	olnr->right = right;
	right = olnr;

	// note that the modifications kept the node itself in place, so we need to swap its data with its olnr's
	T* tempData = data;
	data = olnr->data;
	olnr->data = tempData;

	// update the diff fields for node and new right
	if (pdiff < 0) {  // if imbalance was outside left heavy (left-left violation)
		diff = gdiff + 2;
		right->diff = gdiff - pdiff + 1;
	}
	else {  // otherwise imbalance was inside left heavy (left-right violation)
		diff = pdiff + 1;
		right->diff = gdiff + 1;
	}
}

template <typename T>
void AVLTree<T>::zagLeft()
{
	if (right == NULL) {  // no right, no way to zig
		return;
	}
	// keep track of diff of node and right for changes
	int gdiff = diff;
	int pdiff = right->diff;

	// modify the tree
	AVLTree<T>* ornl = right;  // ornl is "old right, new left"
	right = ornl->right;
	ornl->right = ornl->left;
	ornl->left = left;
	left = ornl;

	// note that the modifications kept the node itself in place, so we need to swap its data with its ornl's
	T* tempData = data;
	data = ornl->data;
	ornl->data = tempData;

	// update the diff fields for node and new left
	if (pdiff > 0) {  // if imbalance was outside right heavy (right-right violation)
		diff = gdiff - 2;
		left->diff = gdiff - pdiff - 1;
	}
	else {  // otherwise imbalance was inside right heavy (right-left violation)
		diff = pdiff - 1;
		left->diff = gdiff - 1;
	}
}

template <typename T>
void AVLTree<T>::zigRightZagLeft()
{

}

template <typename T>
void AVLTree<T>::zagLeftZigRight()
{

}

template <typename T>
void AVLTree<T>::rebalance()
{

}

template <typename T>
int AVLTree<T>::getHeight()
{
	if (this == NULL) //subtree does not exist
	{
		return -1;
	}
	else
	{
		if (right->getHeight() > left->getHeight()) //returns 1 plus the maximum of right's height and left's height
		{
			return 1 + right->getHeight();
		}
		else
		{
			return 1 + left->getHeight();
		}
	}
}

#endif // !AVL_TREE
