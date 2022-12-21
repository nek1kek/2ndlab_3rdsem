#pragma once

#include"RBtree.h"
#include "DynamicArray.h"
#include "LinkedList.h"
#include "ISet.h"
#include "IDictionary.h"
#include "alphabet_pointer.h"
#include "Sparse_matrixx.h"
#include "helping_foo.h"
#include "menu.h"
#include "random.h"
#include <Windows.h>
#include<iostream>
#include <chrono>

using namespace std;


template<typename T>
void time() {
	DynamicArray<double> time_d;
	DynamicArray<double> time_d_del;
	for (int cnt = 1; cnt <= 10; cnt++) {
		double average_insert_d = 0;
		double average_insert_d_del = 0;
		for (int k = 0; k < 5; k++) {//чтобы взять среднее по 5
			DynamicArray<T> da;
			for (int i = 0; i < cnt * 500; i++) {
				da.Append(random_unit<T>::generate());
			}

			IDictionary<T, int> dict;
			auto start = std::chrono::system_clock::now();
			for (int i = 0; i < cnt * 500; i++) {
				dict.insert(da[i], 1);
			}
			auto end = std::chrono::system_clock::now();
			
			average_insert_d += std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
			
			start = std::chrono::system_clock::now();
			for (int i = 0; i < cnt * 500; i++) {
				if (dict.find(da[i]))
					dict.remove(da[i]);
			}
			end = std::chrono::system_clock::now();
			average_insert_d_del += std::chrono::duration_cast<std::chrono::microseconds> (end - start).count();
			}
		time_d.Append(average_insert_d / 5);
		time_d_del.Append(average_insert_d_del / 5);
	}
	cout << "\nRBT Dictionary:\n\t";
	for (int i = 0; i < 10; i++) {
		cout << "Count: " << (i + 1) * 500 << "\tInsert: " << time_d[i] << "\t\tDeleting:" << time_d_del[i];
		cout << "\n\n\t";
	}

}

void help() {
	cout << "\tСписок команд:\n\n";
	cout << "\t=======================================\n";
	cout << "\tmatrix - к меню разреженной матрицы\n";
	cout << "\tapointer - к меню алфавитного указателя\n";
	cout << "\ttime {-t тип(string,float,int)} - сравнительный замер времени работы RBT - словаря в реальном времени microsec\n";
	cout << "\tback - выход из подпрограммы\n";
	cout << "\texit - выход\n";
	cout << "\t=======================================\n\n";
}

int main() {
	//для нормального отображения русского
	setlocale(LC_ALL, "Russian");	
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	//для рандома
	srand(time(0));
	string s;
	cout << "[Main menu]:\n>> ";
	getline(cin, s);
	while (s != "exit") {
		try {
			string cmd_t = _GetCommand(s);

			if (cmd_t == "apointer") {
				if (s.find(' ') == string::npos) {
					s = AlpabetPointer_menu::receive("");
				}
				else {
					throw SetException(UnknownCommand);
				}

			}
			else if (cmd_t == "matrix") {
				if (s.find(' ') == string::npos) {
					s = SparseMatrix_menu::receive("");
				}
				else {
					throw SetException(UnknownCommand);
				}
			}
			else if (cmd_t == "time") {
				DynamicArray<Argument<string>>* da = _MakeArgumentList<string>(s, false);
				if (da->GetSize() > 1) throw SetException(UnknownFlag);
				int type = 0;
				if (da->GetSize() == 1) {
					if (da->Get(0).GetFlag() == 't') {
						if (da->Get(0).Getvalue() == "int") {
							type = 0;
						}
						else if (da->Get(0).Getvalue() == "float") {
							type = 1;
						}
						else if (da->Get(0).Getvalue() == "string") {
							type = 2;
						}
						else throw SetException(IncorrectValue);
					}
					else throw SetException(UnknownFlag);
				}
				if (type == 0) {
					time<int>();
				}
				else if (type == 1) {
					time<double>();
				}
				else {
					time<string>();
				}
			}

			else if (cmd_t == "help") {
				help();
			}
			else {
				throw SetException(UnknownCommand);
			}
		}
		catch (SetException e) {
			cout << e.message();
		}
		if (s == "exit") return 0;
		cout << "\n[Main menu]:\n>> ";
		getline(cin, s);
	}

	return 0;
	
}
