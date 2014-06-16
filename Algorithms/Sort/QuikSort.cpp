#include "QuikSort.h"

QuikSort::QuikSort(void)
{
}

QuikSort::~QuikSort(void)
{
}

int QuikSort::partition( int data[], int p, int r )
{
    int x = data[r];
	int k = p - 1;

	for (int i = p; i < r; i++)
	{
		if (data[i] <= x)
		{
			k++;
			
			int temp = data[i];
			data[i] = data[k];
			data[k] = temp;
		}
	}

	int temp = data[k+1];
	data[k+1] = data[r];
	data[r] = temp;
	
	return k+1;
}

void QuikSort::run( int data[], int num )
{
    fun1(data, 0, num-1);
}

void QuikSort::fun1( int data[], int p, int r )
{
    if (p < r)
    {
		int q = partition(data, p, r);
		fun1(data, p, q-1);
		fun1(data, q+1, r);
    }
}
