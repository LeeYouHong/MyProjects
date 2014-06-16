#pragma once

class ISort
{
public:
	ISort(void);
	virtual ~ISort(void);

	virtual void run(int data[], int num) = 0;
};
