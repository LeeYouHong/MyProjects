#pragma once
/**
@brief:插入排序，循环把第i个元素插入到已序列a[0]-a[i-1]中的正确位置。
*/

#include "ISort.h"

class InsertSort : public ISort
{
public:
	InsertSort(void);
	virtual ~InsertSort(void);

	virtual void run(int data[], int num);

private:
	void fun_swap(int data[], int num);
	void fun2(int data[], int num);

};
