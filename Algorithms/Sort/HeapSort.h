#pragma once

#include "ISort.h"

class HeapSort : public ISort
{
public:
	HeapSort(void);
	virtual ~HeapSort(void);

	virtual void run(int data[], int num);

private:
	//void fun1();
	void build_max_heap(int data[], int num);
	void max_heapify(int data[], int i, int num);
};
