#include "BubbleSort.h"

BubbleSort::BubbleSort(void)
{
}

BubbleSort::~BubbleSort(void)
{
}

void BubbleSort::fun1( int data[], int num )
{
    for (int i = 0; i < num; i++)
    {
		for (int j = i; j < num-1; j++)
		{
			if (data[j] > data[j+1])
			{
                 int temp = data[j];
				 data[j] = data[j+1];
				 data[j+1] = temp;
			}
		}
    }
}

void BubbleSort::run( int data[], int num )
{
    fun1(data,num);
}
