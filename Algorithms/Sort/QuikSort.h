#pragma once
#include "isort.h"

class QuikSort : public ISort
{
public:
	QuikSort(void);
	virtual ~QuikSort(void);

	virtual void run(int data[], int num);

private:
	int partition(int data[], int p, int r);

	void fun1(int data[], int p, int r);

};
