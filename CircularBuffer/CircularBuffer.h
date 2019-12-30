#pragma once

#include <memory>
#include <iostream>
#include <sstream>

template <typename BufferType>

class CircularBuffer
{
public:
	explicit CircularBuffer(unsigned int capacity = 10) 
		: _capacity(capacity), _pointerArray(new BufferType[capacity]), _indexToMemIndexTable(new int[capacity]), _freeMemIndexStack(new int[capacity])
	{
		for (int i = 0; i < capacity; i++)
		{
			_indexToMemIndexTable[i] = -1;
			// Every index is available. Populate with range of (0,(capacity - 1))
			_freeMemIndexStack[i] = i;
		}
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
