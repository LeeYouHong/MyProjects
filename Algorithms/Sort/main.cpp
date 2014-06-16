#include <iostream>
#include "ISort.h"
#include "InsertSort.h"
#include "BubbleSort.h"
#include "MergeSort.h"
#include "HeapSort.h"
#include "QuikSort.h"

using namespace std;

int main()
{
	int data[10] = {2, 1, 4, 5,  8, 6, 10, 7, 3, 9};
    
	//ISort *sort = new InsertSort;
	//ISort *sort = new BubbleSort;
	//ISort *sort = new MergeSort;
	//ISort *sort = new HeapSort;
	ISort *sort = new QuikSort;

	sort->run(data, 10);
	
	for(int i = 0; i < 10; i++)
	{
        cout<<data[i]<<ends;
	}
	return 0;
}