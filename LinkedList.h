#include "stdafx.h"

using namespace std;

#pragma once

template<typename T>
class LinkedList
{
private:
	class Node
	{
	public:
		T data;
		Node *next;
		Node *last;
		Node(T data): data(data), next(NULL), last(NULL){}
		Node(T data, Node *next): data(data), last(NULL)
		{
			connect(this, next);
		}
		Node(Node *last, T data): data(data), next(NULL)
		{
			connect(last, this);
		}
		Node(Node *last, T data, Node *next): data(data)
		{
			connect(last, this);
			connect(this, next);
		}
		~Node()
		{
			next = last = NULL;
		}
		void connect(Node* first, Node* second);
		template<typename T> friend class LinkedList;
	};
	int count;
	int currentIndex;
	Node *firstNode, *currentNode, *lastNode;
	void setCurrentNodeToIndex(const int& index);
public:
	LinkedList<T>():count(0),currentIndex(0),firstNode(NULL),currentNode(NULL),lastNode(NULL){}
	LinkedList<T>(const LinkedList<T>& copy):count(0),currentIndex(0),firstNode(NULL),currentNode(NULL),lastNode(NULL)
	{
		for(Iterator i = copy.begin();  i; i++)
		{
			add(*i);
		}
	}
	~LinkedList<T>()
	{
		/*while(count > 0)
		{
			removeFirst();
		}*/
	}
	LinkedList<T> copy()
	{
		LinkedList<T> returnList (*this);
		return returnList;
	}
	int size()const{return count;};
	void setToFirst(){currentNode = firstNode;currentIndex = 0;}
	void setToMiddle();
	void setToLast(){currentNode = lastNode;currentIndex = count - 1;}
	void insert(const int& index, const T& data);
	void swap(const int& index1, const int& index2);
	void add(const T& data);
	void insertFirst(const T& data){insert(0, data);}
	void insertLast(const T& data){add(data);}
	bool operator++();
	bool operator++(int);
	bool operator--();
	bool operator--(int);
	LinkedList<T> operator+(const LinkedList<T>& rightOperand) const;
	void operator+=(const T& data);
	void operator+=(const LinkedList<T>& rightOperand);
	class Iterator
	{
	private:
		Node* currentNode;
		bool failedMovement;
		int index;
	public:
		Iterator():index(0),currentNode(NULL),failedMovement(false){}
		Iterator(int index, Node* currentNode):index(index),currentNode(currentNode),failedMovement(false){}
		Iterator operator++();
		Iterator operator++(int);
		Iterator operator--();
		Iterator operator--(int);
		Iterator operator+(const int&)const;
		Iterator operator-(const int&)const;
		bool operator==(const Iterator& rightOperand) const;
		bool operator!=(const Iterator& rightOperand) const;
		int getIndex(){return index;}
		operator bool()
		{
			return ((!failedMovement)&&currentNode!=NULL);
		}
		T& operator*(){return currentNode->data;}
		template<typename T> friend class LinkedList;
	};
	Iterator begin() const
	{
		Iterator returnIterator (0, firstNode);
		return returnIterator;
	}
	Iterator end() const
	{
		Iterator returnIterator (count-1,lastNode);
		return returnIterator;
	}
	T* toArray()const;
	T& currentData()const{return currentNode->data;}
	bool contains(const T& data);
	T replaceAt(const int& index, const T& data);
	T removeLast();
	T removeFirst();
	T removeWhere(const T& data);
	T removeAllOfOccurence(const T& data);
	T removeSmallest();
	T removeCurrent(){return remove(currentIndex);}
	T remove(const int& index);
	T at(const int& index){return (*this)[index];}
	T& operator[](const int& index);
	void deleteNode(const T&){removeWhere(T);}
	void deleteSmallest(){removeSmallest();}  
	void deleteAllOfOccurrence(const T& data){removeAllOfOccurence(data);}
	T& getKthElement(const int item){return (*this)[item-1];}
	void deleteKthElement(const int item){remove(item-1);}
};

template<typename T>
void LinkedList<T>::Node::connect(Node* first, Node* second)
{
	if(first != NULL)
	{
		first->next = second;
	}
	if(second != NULL)
	{
		second->last = first;
	}
}

template<typename T>
void LinkedList<T>::setCurrentNodeToIndex(const int& index)
{
	if(firstNode!=NULL&&0<=index&&index<count)
	{
		bool goesForward = true;
		if(currentIndex < index)
		{
			int fromCurrentToFind = index - currentIndex;
			int fromFindToEnd = count - index - 1;
			if(fromCurrentToFind > fromFindToEnd)
			{
				this->setToLast();
				goesForward = false;
			}
		}
		else
		{
			int fromFindToCurrent = currentIndex - index;
			if(index >= fromFindToCurrent)
			{
				goesForward = false;
			}
			else
			{
				this->setToFirst();
			}
		}
		while(currentIndex != index)
		{
			if(goesForward)
			{
				(*this)++;
			}
			else
			{
				(*this)--;
			}
		}
	}
}

template<typename T>
void LinkedList<T>::setToMiddle()
{
	int halfway;
	if(count % 2 == 0)
	{
		halfway = count/2;
	}
	else
	{
		halfway = (count-1)/2;
	}
	setCurrentNodeToIndex(halfway);
}

template<typename T>
void LinkedList<T>::insert(const int& index, const T& data)
{
	if(index == count)
	{
		add(data);
	}
	else if(index == 0)
	{
		firstNode = new Node(data, firstNode);
		count++;
	}
	else
	{
		setCurrentNodeToIndex(index);
		new Node(currentNode->last, data, currentNode);
		currentIndex++;
		count++;
	}
}

template<typename T>
void LinkedList<T>::swap(const int& index1, const int& index2)
{
	Node *firstIndexCurrent, *secondIndexCurrent;
	setCurrentNodeToIndex(index1);
	firstIndexCurrent = currentNode;
	setCurrentNodeToIndex(index2);
	secondIndexCurrent = currentNode;
	T temp = firstIndexCurrent->data;
	firstIndexCurrent->data = secondIndexCurrent->data;
	secondIndexCurrent->data = temp;
	/*(*this)[index1] = temp2;
	(*this)[index2] = temp1;*/
}

template<typename T>
void LinkedList<T>::add(const T& data)
{
	if(currentNode != NULL)
	{
		lastNode = new Node(lastNode, data);
	}
	else
	{
		firstNode = new Node(data);
		currentNode = firstNode;
		lastNode = firstNode;
	}
	count++;
}

template<typename T>
bool LinkedList<T>::operator++()
{
	if(currentNode->next != lastNode)
	{
		currentNode = currentNode->next;
		++currentIndex;
		return true;
	}
	else
	{
		return false;
	}
}

template<typename T>
bool LinkedList<T>::operator++(int)
{
	if(currentNode != lastNode)
	{
		currentNode = currentNode->next;
		currentIndex++;
		return true;
	}
	else
	{
		return false;
	}
}

template<typename T>
bool LinkedList<T>::operator--()
{
	if(currentNode != firstNode)
	{
		currentNode = currentNode->last;
		--currentIndex;
		return true;
	}
	else
	{
		return false;
	}
}

template<typename T>
bool LinkedList<T>::operator--(int)
{
	if(currentNode != firstNode)
	{
		currentNode = currentNode->last;
		currentIndex--;
		return true;
	}
	else
	{
		return false;
	}
}

template<typename T>
typename LinkedList<T> LinkedList<T>::operator+(const LinkedList<T>& rightOperand) const
{
	LinkedList<T> returnList = *this;
	if(returnList.size() == 0)
	{
		return rightOperand;
	}
	else if(rightOperand.size() == 0)
	{
		return returnList;
	}
	else
	{
		lastNode->connect(returnList.lastNode, rightOperand.firstNode);
		returnList.lastNode = rightOperand.lastNode;
		returnList.count += rightOperand.count;
	}
	return returnList;
}

template<typename T>
void LinkedList<T>::operator+=(const T& data)
{
	add(data);
}

template<typename T>
void LinkedList<T>::operator+=(const LinkedList<T>& rightOperand)
{
	for(Iterator i = rightOperand.begin(); i; i++)
	{
		add(*i);
	}
}

template<typename T>
typename LinkedList<T>::Iterator LinkedList<T>::Iterator::operator++()
{
	if(currentNode->next != NULL)
	{
		currentNode = currentNode->next;
		index++;
	}
	else
	{
		failedMovement = true;
	}
	return *this;
}

template<typename T>
typename LinkedList<T>::Iterator LinkedList<T>::Iterator::operator++(int)
{
	Iterator temp = *this;
	if(currentNode->next != NULL)
	{
		currentNode = currentNode->next;
		index++;
	}
	else
	{
		failedMovement = true;
	}
	return temp;
}

template<typename T>
typename LinkedList<T>::Iterator LinkedList<T>::Iterator::operator--()
{
	if(currentNode->last != NULL)
	{
		currentNode = currentNode->last;
		index--;
	}
	else
	{
		failedMovement = true;
	}
	return *this;
}

template<typename T>
typename LinkedList<T>::Iterator LinkedList<T>::Iterator::operator--(int)
{
	Iterator temp = *this;
	if(currentNode->last != NULL)
	{
		currentNode = currentNode->last;
		index--;
	}
	else
	{
		failedMovement = true;
	}
	return temp;
}

template<typename T>
typename LinkedList<T>::Iterator LinkedList<T>::Iterator::operator+(const int& amount)const
{
	Iterator temp = *this;
	if(amount < 0)
	{
		temp = temp - (-amount);
	}
	else
	{
		for(int i = 0; i < amount; i++)
		{
			temp++;
		}
		if(!temp)
		{
			temp = *this;
		}
	}
	return temp;
}

template<typename T>
typename LinkedList<T>::Iterator LinkedList<T>::Iterator::operator-(const int& amount)const
{
	Iterator temp = *this;
	if(amount < 0)
	{
		temp = temp + (-amount);
	}
	else if(temp.index - amount >= 0)
	{
		for(int i = 0; i < amount; i++)
		{
			temp--;
		}
	}
	return temp;
}

template<typename T>
bool LinkedList<T>::Iterator::operator==(const Iterator& rightOperand) const
{
	return currentNodecurrentNode == rightOperand.currentNode;
}

template<typename T>
bool LinkedList<T>::Iterator::operator!=(const Iterator& rightOperand) const
{
	return currentNode != rightOperand.currentNode;
}

template<typename T>
T* LinkedList<T>::toArray() const
{
	T* returnArray = new T[count];
	for(int i = 0; i < count; i++)
	{
		returnArray[i] = (*this)[i];
	}
	return returnArray;
}

template<typename T>
T LinkedList<T>::replaceAt(const int& index, const T& data)
{
	T returnValue = (*this)[index];
	setCurrentNodeToIndex(index);
	currentNode->data = data;
	return returnValue;
}

template<typename T>
T LinkedList<T>::removeLast()
{
	return remove(count-1);
}

template<typename T>
T LinkedList<T>::removeFirst()
{
	return remove(0);
}

template<typename T>
bool LinkedList<T>::contains(const T& data)
{
	for(int i = 0; i < count; i++)
	{
		if((*this)[i]==data)
		{
			return true;
		}
	}
	return false;
}

template<typename T>
T LinkedList<T>::removeWhere(const T& data)
{
	setToFirst();
	do
	{
		if(this->currentData()==data)
		{
			return removeCurrent();
		}
	}
	while((*this)++);
}

template<typename T>
T LinkedList<T>::removeAllOfOccurence(const T& data)
{
	T returnValue = 0;
	if(firstNode!=NULL)
	{
		setToFirst();
		bool continuing = true;
		while(continuing)
		{
			if(data == currentNode->data)
			{
				returnValue = remove(currentIndex);
				continuing = (firstNode!=NULL);
			}
			else
			{
				continuing = (*this)++;
			}
		}
	}
	return returnValue;
}

template<typename T>
T LinkedList<T>::removeSmallest()
{
	if(firstNode!=NULL)
	{
		setToFirst();
		T current = currentNode->data;
		int indexToDelete = 0;
		while((*this)++)
		{
			if(current > currentNode->data)
			{
				current = currentNode->data;
				indexToDelete = currentIndex;
			}
		}
		remove(indexToDelete);
		return current;
	}
	return 0;
}

template<typename T>
T LinkedList<T>::remove(const int& index)
{
	if(0<=index&&index<count)
	{
		setCurrentNodeToIndex(index);
		Node* nodeToBeDeleted = currentNode;
		T returnValue;
		if(firstNode != NULL)
		{
			returnValue = nodeToBeDeleted->data;
		}
		if(currentNode != lastNode)
		{
			(*this)++;
			currentIndex--;
		}
		else if(currentNode != firstNode)
		{
			(*this)--;
		}
		else
		{
			delete currentNode, firstNode, lastNode;
			currentNode = firstNode = lastNode = NULL;
			count--;
			return returnValue;
		}
		if(nodeToBeDeleted == firstNode)
		{
			firstNode = firstNode->next;
		}
		if(nodeToBeDeleted == lastNode)
		{
			lastNode = lastNode->last;
		}
		count--;
		nodeToBeDeleted->connect(nodeToBeDeleted->last, nodeToBeDeleted->next);
		delete nodeToBeDeleted;
		nodeToBeDeleted = NULL;
		return returnValue;
	}
}

template<typename T>
T& LinkedList<T>::operator[](const int& index)
{
	setCurrentNodeToIndex(index);
	return currentNode->data;
}