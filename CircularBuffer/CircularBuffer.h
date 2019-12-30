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
		if (atIndex >= _size)
		{
			std::stringstream ss;
			ss << "Insert at " << atIndex << " is out of bounds. Size: " << _size;
			throw std::out_of_range(ss.str());
		}

		// Shift all elements right of atIndex right by 1
		for (int i = _size; i > atIndex; i--)
		{
			_indexToMemIndexTable[i] = _indexToMemIndexTable[i - 1];
		}

		int availableIndex = _freeMemIndexStack[_currentFreeMemIndex];
		_pointerArray[availableIndex] = toAdd;
		_indexToMemIndexTable[atIndex] = availableIndex;

		_currentFreeMemIndex++;
		_size++;
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
	std::unique_ptr<int[]> _indexToMemIndexTable; // -1 means index not used 
	std::unique_ptr<int[]> _freeMemIndexStack;

	int _currentFreeMemIndex = 0;
	unsigned int _capacity;
	unsigned int _size = 0;

	void resize()
	{

	}
};
