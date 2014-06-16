#include "MergeSort.h"

MergeSort::MergeSort(void)
{
}

MergeSort::~MergeSort(void)
{
}

void MergeSort::run( int data[], int num )
{
    fun1(data, 0, num-1);
}

void MergeSort::fun1( int data[], int p, int r)
{
	if (p < r)
	{
		int q = (p+r)/2;
		fun1(data, p, q);
		fun1(data, q + 1, r);
		Merge(data, p, q, r);
	}
}

void MergeSort::Merge(int data[], int p, int q, int r)
{
    int n1 = q - p + 1;
	int n2 = r - q;

	int *L_array = new int[n1 + 1];
	int *R_array = new int[n2 + 1];
    
    for (int i = 0; i < n1; i++)
    {
		L_array[i] = data[p+i];
    }
    
	for (int i = 0; i < n2; i++)
	{
		R_array[i] = data[q+i+1];
	}

	L_array[n1] = 65535;
	R_array[n2] = 65535;

	int i = 0, j = 0;
	for (int k = p; k <= r; k++)
	{
		if (L_array[i] <= R_array[j])
		{
			data[k] = L_array[i];
			i++;
		}
		else
		{
			data[k] = R_array[j];
			j++;
		}
	}
}