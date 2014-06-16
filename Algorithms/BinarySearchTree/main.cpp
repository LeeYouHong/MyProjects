#include <iostream>
#include "BinarySearchTree.h"
using namespace std;

int main()
{
	BinarySearchTree *bst = new BinarySearchTree;
	int data[10] = {15, 12, 45, 4014, 2, 515, 22, 33, 1, 6};
	bst->create(data, 10);
	
	cout<<"遍历二叉树为：";
	bst->show();
	cout<<endl;

	cout<<"查找10：";
	Node *result = bst->search(10);
	if (result == NULL)
	{
		cout<<"No resutl"<<endl;
	}
	else
	{
		cout<<endl<<result->data<<endl;
	}

	cout<<"最大值：";
	Node *max = bst->max();
	cout<<max->data<<endl;

	cout<<"最小值：";
	Node *min = bst->min();
	cout<<min->data<<endl;
    
	cout<<"33的前驱：";
	Node *pre = bst->predecessor(33);
	pre==NULL ? cout<<"NULL"<<endl : cout<<pre->data<<endl;
    
	cout<<"15的后驱";
	Node *suc = bst->successor(15);
	suc==NULL ? cout<<"NULL"<<endl : cout<<suc->data<<endl;
    
	cout<<"删除33";
	bst->del(33);
	bst->show();
	cout<<endl;

	delete bst;
	return 0;
}