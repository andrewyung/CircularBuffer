#pragma once

template <typename BufferType>

class CircularBuffer
{
public:
	CircularBuffer();
	CircularBuffer(unsigned int capacity);
	~CircularBuffer();

	void add();
	void get();
	void remove();
	unsigned int size();

private:
	struct DataNode
	{
		T data;
		DataNode *next;
	};

	DataNode<BufferType>* head;
	DataNode<BufferType>* tail;

	void resize();
};
