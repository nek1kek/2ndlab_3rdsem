#include"RBtree.h"
#include "DynamicArray.h"
#include "LinkedList.h"
#include "ISet.h"
#include "IDictionary.h"
#include "alphabet_pointer.h"
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

void z() {
	setlocale(LC_ALL, "Russian");

	AlpabetPointer<false> ap("Привет, 1 меняменменяменяменяменязовут Артём!", 2);
	ap.remove("Привет");
	ap.print();
	ap.get_pages("меня");
}

int main() {
	setlocale(LC_ALL, "Russian");
	z();

	//проверить бы счет элементов в рб дереве при мульти, но мне пока в падлу)))
}
