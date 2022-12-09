//test.cpp
#include"Rbtree.h"
#include "DynamicArray.h"
#include "LinkedList.h"
#include "ISet.h"
#include<iostream>
#include<vector>
using namespace std;
#include <map>;

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
	cout << endl << tree.search(30)->data.first << endl;
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


void c()
{
	/*
	ISet<int> a;
	a.Get123();
	a.insert(1);*/
	LinkedList<double> b{ 1.2,1.3,1.2 };
	ISet<double,true> Z(b);
	cout << Z.get_min() << endl;
	Z.print();
	cout << Z.amount() << endl;

}

int main() {

}
