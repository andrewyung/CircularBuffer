#pragma once

#include <memory>
#include <iostream>

template <typename BufferType>

class CircularBuffer
{
public:
	CircularBuffer(unsigned int capacity = 10)
	{
		_capacity = capacity;

		std::unique_ptr<BufferType[]> pointerArray(new BufferType[capacity]);

		std::unique_ptr<int[]> indexToMemIndexTable(new int[capacity]); // -1 means not used
		std::unique_ptr<int[]> freeMemIndexStack(new int[capacity]);  //  -1 means occupied slot
	}

	void add(BufferType toAdd) 
	{
		int availableIndex = _freeMemIndexStack[_currentFreeMemIndex];
		_pointerArray[availableIndex] = toAdd;
		_indexToMemIndexTable[_size] = availableIndex;
		
		_currentFreeMemIndex++;
		_size++;
	}
	void insert(BufferType toAdd, int atIndex) 
	{
	}
	void remove() {}
	BufferType get(int atIndex) 
	{
		return _pointerArray[_indexToMemIndexTable[atIndex]];
	}
	unsigned int size() 
	{
		return _size;
	}

private:
	std::unique_ptr<BufferType[]> _pointerArray;
	std::unique_ptr<int[]> _indexToMemIndexTable, _freeMemIndexStack;

	int _currentFreeMemIndex;
	unsigned int _capacity;
	unsigned int _size;

	void resize()
	{

	}
};
