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
	cout << "Найти узел с ключом 30:";
	cout << endl << tree.search(111)->data.first << endl;
	cout << "Удалить узел с ключом 100 \n";
	tree.remove(100);
	tree.preOrder();
	cout << endl;
	cout << "\nКрасные и черные детали дерева: \n";
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
	//cout << (tree1 == tree2);
}


int main()
{
	setlocale(LC_ALL, "Rus");
	RBTree<int, int, true> tree;
	DynamicArray<int> nums{ 10,40,30,60,90,-1, 70,20,50,80,100 };
	for (auto num : nums) {
		tree.insert(1, num);
		tree.insert(num, 1);
	}
	tree.print_values(3);
	cout << tree.amount();
}

