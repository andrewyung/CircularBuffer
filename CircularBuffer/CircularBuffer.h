#pragma once

#include <memory>
#include <iostream>
#include <sstream>
#include <mutex>
#include <algorithm>

template <typename BufferType>

class CircularBuffer
{
public:
	CircularBuffer(int capacity = 10) 
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
		std::lock_guard<std::mutex> lock(bufferMutex);

		if (_size = _capacity)
		{
			resize(capacityIncrease());
		}

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
	 *
	 * @throws std::out_of_range Thrown if "atIndex" is less than 0 or greater than size+1
	 */
	void insert(BufferType toAdd, int atIndex) 
	{
		// Exception out of bounds
		if (atIndex < 0 || atIndex > _size)
		{
			std::stringstream ss;
			ss << "Insert at " << atIndex << " is out of bounds. Size: " << _size;
			throw std::out_of_range(ss.str());
		}

		std::lock_guard<std::mutex> lock(bufferMutex);

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
	 * Remove and retrieve from head of the buffer
	 *
	 * @throws std::out_of_range Thrown if buffer is empty
	 */
	BufferType remove() 
	{
		// Exception empty buffer
		if (_size == 0)
		{
			throw std::out_of_range("Can't remove from empty CircularBuffer");
		}

		//std::lock_guard<std::mutex> lock(bufferMutex);

		// Keep head of buffer as we will return this
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

	/*
	* Returns data at index
	* 
	* @param[in] atIndex the index to retrieve at
	*
	* @throws std::out_of_range Thrown if "atIndex" is less than 0 or greater than size
	*/
	BufferType get(int atIndex) 
	{
		// Exception out of bounds
		if (atIndex < (unsigned int) 0 || atIndex > _size - 1)
		{
			std::stringstream ss;
			ss << "Get at " << atIndex << " is out of bounds. Size: " << _size;
			throw std::out_of_range(ss.str());
		}
		
		std::lock_guard<std::mutex> lock(bufferMutex);

		return _pointerArray[_indexToMemIndexTable[atIndex]];
	}

	/*
	* Returns the number of elements in buffer
	*/
	unsigned int size() 
	{
		std::lock_guard<std::mutex> lock(bufferMutex);

		return _size;
	}

private:
	std::mutex bufferMutex;

	std::shared_ptr<BufferType[]> _pointerArray;
	std::shared_ptr<int[]> _indexToMemIndexTable; // -1 means index not used 
	std::shared_ptr<int[]> _freeMemIndexStack; // Assume 0 is top of stack and n is bottom

	int _currentFreeMemIndex = 0;
	int _capacity;
	int _size = 0;

	/*
	* Calculates the new capacity based on multiplier
	*
	* @param[in] multiplier is the multiplicative increase
	*/
	int capacityIncrease(int multiplier = 2)
	{
		return _capacity * multiplier;
	}

	// TODO: implementation
	void resize(int newCapacityLimit)
	{
		//_pointerArray = std::make_shared<BufferType[]>(newCapacityLimit);

		//_indexToMemIndexTable = std::make_shared<int[]>(newCapacityLimit);

		//_freeMemIndexStack = std::make_shared<int[]>(newCapacityLimit);

		_capacity = newCapacityLimit;
	}
};
