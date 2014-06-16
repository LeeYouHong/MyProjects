#include "HeapSort.h"

HeapSort::HeapSort(void)
{
}

HeapSort::~HeapSort(void)
{
}

void HeapSort::max_heapify( int data[], int i , int num)
{
    int l = i << 1;
	int r = (i << 1) + 1;
    
	int largest;
	if (l < num && data[l] > data[i])
	{
        largest = l;
	}
	else
	{
		largest = i;
	}

    if (r < num && data[r] > data[largest])
    {
		largest = r;
    }

	if (largest != i)
	{
		int temp = data[i];
		data[i] = data[largest];
		data[largest] = temp;

		max_heapify(data, largest, num);
	}
}

void HeapSort::build_max_heap( int data[], int num )
{
    int size = num;
	for (int i = size/2; i >= 0; i--)
	{
         max_heapify(data, i, num);
	}
}

void HeapSort::run( int data[], int num )
{
    build_max_heap(data, num);
	for (int i = num-1; i > 0; i--)
	{
		int temp = data[0];
		data[0] = data[i];
		data[i] = temp;

        max_heapify(data, 0, --num);
	}
}
