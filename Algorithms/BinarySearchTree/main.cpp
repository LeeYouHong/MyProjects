#include <iostream>
#include "BinarySearchTree.h"
using namespace std;

int main()
{
	BinarySearchTree *bst = new BinarySearchTree;
	int data[10] = {15, 12, 45, 4014, 2, 515, 22, 33, 1, 6};
	bst->create(data, 10);
	
	cout<<"����������Ϊ��";
	bst->show();
	cout<<endl;

	cout<<"����10��";
	Node *result = bst->search(10);
	if (result == NULL)
	{
		cout<<"No resutl"<<endl;
	}
	else
	{
		cout<<endl<<result->data<<endl;
	}

	cout<<"���ֵ��";
	Node *max = bst->max();
	cout<<max->data<<endl;

	cout<<"��Сֵ��";
	Node *min = bst->min();
	cout<<min->data<<endl;
    
	cout<<"33��ǰ����";
	Node *pre = bst->predecessor(33);
	pre==NULL ? cout<<"NULL"<<endl : cout<<pre->data<<endl;
    
	cout<<"15�ĺ���";
	Node *suc = bst->successor(15);
	suc==NULL ? cout<<"NULL"<<endl : cout<<suc->data<<endl;
    
	cout<<"ɾ��33";
	bst->del(33);
	bst->show();
	cout<<endl;

	delete bst;
	return 0;
}