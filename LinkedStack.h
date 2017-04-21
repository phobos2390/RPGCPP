//#include "stdafx.h"
#include "LinkedList.h"

template<typename T>
class LinkedStack
{
private:
	LinkedList<T>* stack;
public:
	LinkedStack<T>()
	{
		stack = new LinkedList<T>;
	}
	~LinkedStack<T>()
	{
		delete stack;
	}
	LinkedStack<T>& push(const T& data);
	T pop();
	int size(){return count;}
	bool isEmpty(){return count<=0;}
};

template<typename T>
LinkedStack<T>& LinkedStack<T>::push(const T& data)
{
	stack->add(data);
	return this;
}

template<typename T>
T LinkedStack<T>::pop()
{
	return stack->removeLast();
}
