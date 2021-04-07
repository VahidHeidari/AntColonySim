#ifndef RING_BUFFER_H_
#define RING_BUFFER_H_

#include <vector>

template <typename T, int MAX_ITEMS=10>
class RingBuffer
{
public:
	typedef std::vector<T> Vect;

	RingBuffer()
		: start(0)
		, end(0)
		, buffer(MAX_ITEMS)
	{}

	void PushBack(const T& t)
	{
		buffer[end] = t;
		end = (end + 1) % MAX_ITEMS;
		if (end == start)
			start = (start + 1) % MAX_ITEMS;
	}

	int start;
	int end;
	Vect buffer;
};

#endif
