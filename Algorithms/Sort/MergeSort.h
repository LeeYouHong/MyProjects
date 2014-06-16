#pragma once
#include "isort.h"

class MergeSort : public ISort
{
public:
	MergeSort(void);
	virtual ~MergeSort(void);

	virtual void run(int data[], int num);

private:
	void fun1(int data[], int p, int r);
	void Merge(int data[], int p, int q, int r);
};
