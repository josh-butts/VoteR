#ifndef AVL_TREE_ENUMERATOR
#define AVL_TREE_ENUMERATOR

#include <stack>
#include "Enumerator.h"
#include "AVLTreeOrder.h"
#include "AVLTree.h"

template <typename T>
class AVLTreeEnumerator : public Enumerator<T>
{
private:
	AVLTreeOrder order;
	std::stack<const AVLTree<T>*> traversalStack;
	void buildTraversalStack(const AVLTree<T>* current);
	void inOrderPush(const AVLTree<T>* node);
	void postOrderPush(const AVLTree<T>* node);
	void preOrderPush(const AVLTree<T>* node);
public:
	AVLTreeEnumerator(const AVLTree<T>* root, AVLTreeOrder order);
	virtual ~AVLTreeEnumerator();
	bool hasNext() const;
	T next();						// throws ExceptionEnumerationBeyondEnd if no next item is available
	T peek() const;					// throws ExceptionEnumerationBeyondEnd if no next item is available
	AVLTreeOrder getOrder();		// returns the order of this enumerator (preorder, inorder, or postorder)
};

//Public:
template <typename T>
AVLTreeEnumerator<T>::AVLTreeEnumerator(const AVLTree<T>* root, AVLTreeOrder order)
{
	if (root == NULL || root->data == NULL) throw new ExceptionAVLTreeAccess();
	if (order != AVLTreeOrder::inorder && order != AVLTreeOrder::postorder
		&& order != AVLTreeOrder::preorder) throw new ExceptionAVLTreeAccess();
	this->order = order;
	buildTraversalStack(root);
}

template <typename T>
AVLTreeEnumerator<T>::~AVLTreeEnumerator()
{

}

template <typename T>
bool AVLTreeEnumerator<T>::hasNext() const
{
	if (traversalStack.empty())
	{
		return false;
	}
	if (traversalStack.top() != NULL && traversalStack.top()->data != NULL)
	{
		return true;
	}
	return false;
}

template <typename T>
T AVLTreeEnumerator<T>::next()
{
	T item = T(*traversalStack.top()->data); //store data;
	traversalStack.pop(); //advance to next
	return item;
}

template <typename T>
T AVLTreeEnumerator<T>::peek() const
{
	return T(*traversalStack.top()->data);
}

template <typename T>
AVLTreeOrder AVLTreeEnumerator<T>::getOrder()
{
	return order;
}

//Private:
template <typename T>
void AVLTreeEnumerator<T>::buildTraversalStack(const AVLTree<T>* current)
{
	if (current == NULL) throw new ExceptionAVLTreeAccess();
	if (order == AVLTreeOrder::inorder)
	{
		inOrderPush(current);
	}
	else if (order == AVLTreeOrder::postorder)
	{
		postOrderPush(current);
	}
	else if (order == AVLTreeOrder::preorder)
	{
		preOrderPush(current);
	}
}

template <typename T>
void AVLTreeEnumerator<T>::inOrderPush(const AVLTree<T>* node) //Adapted from ZyBooks
{
	if (node == NULL || node->data == NULL) return;
	inOrderPush(node->right);
	traversalStack.push(node);
	inOrderPush(node->left);
}

template <typename T>
void AVLTreeEnumerator<T>::postOrderPush(const AVLTree<T>* node)
{
	if (node == NULL || node->data == NULL) return;
	traversalStack.push(node);
	postOrderPush(node->right);
	postOrderPush(node->left);
}

template <typename T>
void AVLTreeEnumerator<T>::preOrderPush(const AVLTree<T>* node)
{
	if (node == NULL || node->data == NULL) return;
	preOrderPush(node->right);
	preOrderPush(node->left);
	traversalStack.push(node);
}

#endif // !AVL_TREE_ENUMERATOR
