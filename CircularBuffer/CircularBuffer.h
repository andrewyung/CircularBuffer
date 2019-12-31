#pragma once

#include <memory>
#include <iostream>
#include <sstream>

template <typename BufferType>

class CircularBuffer
{
public:
	CircularBuffer(unsigned int capacity = 10) 
		: _capacity(capacity), _pointerArray(new BufferType[capacity]), _indexToMemIndexTable(new int[capacity]), _freeMemIndexStack(new int[capacity])
	{
		for (int i = 0; i < capacity; i++)
		{
			_indexToMemIndexTable[i] = -1;
			// Every index is available. Populate with range of (0,(capacity - 1))
			_freeMemIndexStack[i] = i;
		}
	}

	/**
	 * Add at the tail of the buffer
	 *
	 * @param[in] toAdd the data to add to buffer
	 */
	void add(BufferType toAdd) 
	{
		// Look for free spot in memory
		int availableIndex = _freeMemIndexStack[_currentFreeMemIndex];
		// Assign to free spot
		_pointerArray[availableIndex] = toAdd;
		// Updated table (index to memory index)
		_indexToMemIndexTable[_size] = availableIndex;
		
		// "Pop" the top of the stack
		_currentFreeMemIndex++;
		_size++;
	}
	
	/**
	 * Insert data into buffer at an index.
	 *
	 * @param[in] toAdd the data to add to buffer
	 * @param[in] atIndex the index to insert data
	 */
	void insert(BufferType toAdd, int atIndex) 
	{
		if (atIndex > _size)
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

		// Insert at index and update tables
		int availableIndex = _freeMemIndexStack[_currentFreeMemIndex];
		_pointerArray[availableIndex] = toAdd;
		_indexToMemIndexTable[atIndex] = availableIndex;

		_currentFreeMemIndex++;
		_size++;
	}
	
	/**
	 * Remove and retrieve from head of the buffer.
	 */
	BufferType remove() 
	{
		if (_size == 0)
		{
			throw std::out_of_range("Can't remove from empty CircularBuffer");
		}

		int memLocationIndex = _indexToMemIndexTable[0];
		BufferType head = (BufferType) _pointerArray[_indexToMemIndexTable[0]];
		
		// Shift all elements left
		for (int i = 0; i < _size - 1; i++)
		{
			_indexToMemIndexTable[i] = _indexToMemIndexTable[i + 1];
		}
		
		_currentFreeMemIndex--;
		_size--;
		// Include index at end back into free index stack
		_freeMemIndexStack[_currentFreeMemIndex] = memLocationIndex;

		return head;
	}
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
	std::unique_ptr<int[]> _freeMemIndexStack; // Assume 0 is top of stack and n is bottom

	int _currentFreeMemIndex = 0;
	unsigned int _capacity;
	unsigned int _size = 0;

	void resize()
	{

	}
};
