#pragma once
#include "isort.h"

class BubbleSort : public ISort
{
public:
	BubbleSort(void);
	~BubbleSort(void);

	virtual void run(int data[], int num);

private:
	void fun1(int data[], int num);
};
