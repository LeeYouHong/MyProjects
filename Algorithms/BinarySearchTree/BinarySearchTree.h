#pragma once

class Node
{
public:
	int data;
	Node *parent;
	Node *l_child;
	Node *r_child;
};

class BinarySearchTree
{
public:
	BinarySearchTree(void);
	~BinarySearchTree(void);

public:
	void create(int data[], int num);
	void show();
	Node *search(int value);
	Node *min();
	Node *max();
	void del(int value);
	Node *successor(int value);
	Node *predecessor(int value);
private:
	void insert(int key);
	void midlle_show(Node *head);
    Node *search_recursive(Node *head, int value);
	Node *search_iteration(Node *head, int value);
	void earse(Node *head);
	Node *min1(Node *head);
	Node *max1(Node *head);
	Node *successor_(Node *node);
	Node *predecessor_(Node *node);
	void del_(Node *node);

private:
    Node *root;
};
