//test.cpp
#include"Rbtree.h"
#include "DynamicArray.h"
#include "LinkedList.h"

#include<iostream>
#include<vector>
using namespace std;


void a() {
	vector<int> nums{ 10,40,30,60,90,-1, 70,20,50,80,100 };
	/*
				  60	
				 / \
				/	\
			   /	 \
			  /		  \
			 /		   \
			/			\
		  30			 80
		 /	\			/	\
	  10	  40	 70       90
	  /\	  /\	 /\		  /\
	-1	20	-	50	-	-	-	100
	
	*/



	RBTree<int, int> tree;
	for (auto num : nums)
		tree.insert(num, 1);
	tree.preOrder();
	cout << endl;
	tree.inOrder();
	cout << endl;
	tree.postOrder();
	cout << endl;
	cout << "����� ���� � ������ 30: \n";
	cout << endl << tree.search(30)->data.first << endl;
	cout << "������� ���� � ������ 100 \n";
	tree.remove(100);
	tree.preOrder();
	cout << endl;
	cout << "\ n������� � ������ ������ ������: \n";
	tree.print();
	cin.get();
}

void b() {
	RBTree<int, int> tree1,tree2;
	vector<int> nums{ 10,40};
	for (auto num : nums) {
		tree1.insert(num, 1);
		tree2.insert(num, 2);
	}
	cout << (tree1 == tree2);
}


int main()
{
	a();
}

