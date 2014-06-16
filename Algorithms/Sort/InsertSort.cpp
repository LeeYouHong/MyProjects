#include "InsertSort.h"

InsertSort::InsertSort(void)
{
}

InsertSort::~InsertSort(void)
{
}

void InsertSort::run( int data[], int num )
{
 //   fun_swap(data, num);
	fun2(data, num);
}

void InsertSort::fun_swap( int data[], int num )
{
	for (int i = 1; i < num; i++)
	{
		int j = i;
		while (j > 0 && data[j-1] > data[j])
		{
			int temp = data[j-1];
			data[j-1] = data[j];
			data[j] = temp;

			j--;
		}
	}
}

void InsertSort::fun2( int data[], int num )
{
	for (int i = 1; i < num; i++)
	{
		int temp = data[i];
		int j = i-1;
		while (j >= 0 && data[j] > temp)
		{
			data[j+1] = data[j];
			j--;
		}
		data[j+1] = temp;
	}
}
