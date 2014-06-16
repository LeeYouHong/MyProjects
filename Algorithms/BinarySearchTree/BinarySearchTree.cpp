#include <iostream>
#include "BinarySearchTree.h"

using namespace std;

BinarySearchTree::BinarySearchTree(void)
:root(NULL)
{

}

BinarySearchTree::~BinarySearchTree(void)
{
    earse(root);
}

void BinarySearchTree::create( int data[], int num )
{
	for (int i = NULL; i < num; i++)
	{
		insert(data[i]);
	}
}

void BinarySearchTree::insert( int key )
{
	Node *add = new Node;
	add->data = key;
	add->parent = NULL;
	add->l_child = NULL;
	add->r_child = NULL;
    if (root == NULL)
    {
        root = add;
    }
	else
	{
		Node *temp = root;
		Node *parent = NULL;
		while (temp != NULL)
		{
			parent = temp;
			if (key < temp->data)
			{
				temp = temp->l_child;
			}
			else
			{
				temp = temp->r_child;
			}
		}
		if (key < parent->data)
		{
			parent->l_child = add;
			add->parent = parent;
		}
		else
		{
			parent->r_child = add;
			add->parent = parent;
		}
	}
}

void BinarySearchTree::midlle_show(Node *head)
{
    if (head != NULL)
    {
		midlle_show(head->l_child);
		cout<<head->data<<ends;
		midlle_show(head->r_child);
    }
}

void BinarySearchTree::show()
{
    midlle_show(root);
}

Node *BinarySearchTree::search( int value )
{
    return search_iteration(root, value);
}

Node * BinarySearchTree::search_recursive( Node *head, int value )
{
	if (head == NULL || value == head->data)
	{
		return head;
	}

	if (value < head->data)
	{
		return search_recursive(head->l_child, value);
	}
	else
	{
		return search_recursive(head->r_child, value);
	}
}

Node * BinarySearchTree::search_iteration(Node *head, int value)
{
	Node *temp = head;
    while (temp != NULL && temp->data != value)
    {
	    if (value < temp->data)
	    {
			temp = temp->l_child;
	    }
		else
		{
			temp = temp->r_child;
		}
    }

	return temp;
}

Node * BinarySearchTree::min()
{
    return min1(root);
}

Node * BinarySearchTree::max()
{
    return max1(root);
}

void BinarySearchTree::earse(Node *head)
{
    if (head != NULL)
    {
		earse(head->l_child);
		earse(head->r_child);
		delete head;
    }
}

void BinarySearchTree::del_( Node *node )
{
	Node *temp1 = NULL;
	Node *temp2 = NULL;
    if (node->l_child == NULL || node->r_child == NULL)
    {
        temp1 = node;
    }
	else
	{
		temp1 = successor_(node);
	}
	if (temp1->l_child != NULL)
	{
        temp2 = temp1->l_child;
	}
	else
	{
		temp2 = temp1->r_child;
	}
	if (temp2 != NULL)
	{
		temp2->parent = temp1->parent;
	}
    
	if (temp1->parent == NULL)
	{
		root = temp2;
	}
	else if (temp1 == temp1->parent->l_child)
	{
		temp1->parent->l_child = temp2;
	}
	else
	{
		temp1->parent->r_child = temp2;
	}
	if (temp1 != temp2)
	{
		node->data = temp1->data;
	}
}

void BinarySearchTree::del( int value )
{
    Node *temp = search(value);
	del_(temp);
}

Node * BinarySearchTree::successor_( Node *node )
{
    if (node->r_child != NULL)
    {
		return min1(node->r_child);
    }
	Node *result = node->parent;
	Node *temp = node;
	while (result != NULL && temp == result->r_child)
	{
		temp = result;
        result = result->parent;
	}
	return result;
}

Node * BinarySearchTree::successor( int value )
{
    Node *temp = search(value);
	return successor_(temp);
}

Node * BinarySearchTree::min1( Node *head )
{
	while(head != NULL && head->l_child != NULL)
	{
		head = head->l_child;
	}
	return head;
}

Node * BinarySearchTree::max1( Node *head )
{
	while(head != NULL && head->r_child != NULL)
	{
		head = head->r_child;
	}
	return head;
}

Node * BinarySearchTree::predecessor_( Node *node )
{
	Node *parent = node->parent;
    if (parent != NULL && node == parent->r_child && node->l_child == NULL)
    {
        return parent;
    }
	return max1(node->l_child);
}

Node * BinarySearchTree::predecessor( int value )
{
     Node * temp = search(value);
	 return predecessor_(temp);
}
