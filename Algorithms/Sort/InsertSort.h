#pragma once
/**
@brief:��������ѭ���ѵ�i��Ԫ�ز��뵽������a[0]-a[i-1]�е���ȷλ�á�
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
