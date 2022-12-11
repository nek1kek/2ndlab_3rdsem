#include"RBtree.h"
#include "DynamicArray.h"
#include "LinkedList.h"
#include "ISet.h"
#include "IDictionary.h"
#include<iostream>
using namespace std;

void a() {
	setlocale(0, "");
	LinkedList<int> nums{ 10,40,30,60,90,-1, 70,20,50,80,100 };
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

}



int main() {
	IDictionary<int, int> dict;
	DynamicArray<int> A;
	for (int i = 0; i < 250000; i++) {
		A.Append(rand() % 10000);
	}
	for (int i = 0; i < 250000; i++) {
		dict.insert(A[i], i);
	}
	for (int i = 0; i < 250000; i++) {
		try {
			dict.remove(A[i]);
		}
		catch (SetException e) {
			if (e.id != NoSuchElement) {
				throw e;
			}
		}
	}
	cout << dict.amount();
	dict.print();
}
