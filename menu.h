#pragma once
#include "DynamicArray.h"
#include <string>
#include "random.h"
#include "alphabet_pointer.h"
#include "Sparse_matrixx.h"
#include "helping_foo.h"

string _GetCommand(string s) {
	string res;
	for (int i = 0; i < s.size() && i < s.find(' '); i++) {
		res += s[i];
	}
	return res;
}

namespace AlpabetPointer_menu {
	void help() {
		cout << "\n";
		cout << "\tСписок команд: \n\n\t[] - обязательные аргументы, {} - опциональные аргументы\n\n";
		cout << "\t====================================== = \n\n";
		cout << "\tcreate{-s(размер в символах)} {-e(создать пустой указатель)} - создать указатель\n";
		cout << "\tprint - вывести указатель\n";
		cout << "\tadd[-i слово] - добавить слово в указатель\n";
		cout << "\tremove[-i слово] - удалить слово из указателя\n";
		cout << "\tadd_text - добавить текст в указатель\n";
		cout << "\tremove_text - удалить текст из указателя(прошу заметить, что подстрока должна быть целиком в тексте)\n\n";
		cout << "\tget_pages - удалить текст из указателя\n\n";

		cout << "\t====================================== = \n\n";
	}

	template<bool IsWords>
	class APMenu {
	private:
		AlpabetPointer<IsWords>* ap = nullptr;
	public:
		//конструктор f - непустым ли создаем
		APMenu(bool f) {
			string str;
			cout << "Enter page size:\n>> ";
			int page_size = 0;
			getline(cin, str);
			page_size = _TypeCast<int>::cast(str);
			if (page_size < 1) throw SetException(IncorrectValue);
			str = "";
			if (f) {
				cout << "Enter text:\n>> ";
				getline(cin, str);
			}
			ap = new AlpabetPointer<IsWords>(str, page_size);
		}
		void Process(string cmd, DynamicArray<Argument<string>>* arr) {
			if (cmd == "print") {
				if (arr->GetSize() != 0) throw SetException(UnknownFlag);
				cout << endl;
				ap->print();
				cout << endl;

			}
			else if (cmd == "add") {
				if (arr->GetSize() > 1) throw SetException(UnknownFlag);
				if (arr->Get(0).GetFlag() != 'i') throw SetException(UnknownFlag);
				ap->add(arr->Get(0).Getvalue());
			}
			else if (cmd == "add_text") {
				if (arr->GetSize() > 0) throw SetException(UnknownFlag);
				string str; 
				cout << "Enter text:\n>> ";
				getline(cin, str);
				ap->add(str);
			}
			else if (cmd == "remove") {
				if (arr->GetSize() > 1) throw SetException(UnknownFlag);
				if (arr->Get(0).GetFlag() != 'i') throw SetException(UnknownFlag);
				ap->remove(arr->Get(0).Getvalue());
			}
			else if (cmd == "help") {
				help();
			}
			else if (cmd == "remove_text") {
				if (arr->GetSize() > 0) throw SetException(UnknownFlag);
				string str;
				cout << "Enter text:\n>> ";
				getline(cin, str);
				ap->remove(str);
			}
			else if (cmd == "get_pages") {
				if (arr->GetSize() > 0) throw SetException(UnknownFlag);
				string str;
				cout << "Enter text:\n>> ";
				getline(cin, str);
				LinkedList<size_t> ls = ap->get_pages(str);
				cout << "<";
				for (LinkedList<size_t>::const_iterator it = ls.cbegin(); it != ls.cend(); it++) {
					cout << *it;
					if (it != ls.cend() - 1) cout << ", ";
				}
				cout << ">\n";
			}
			else {
				throw SetException(UnknownCommand);
			}
		}

		~APMenu() = default;
	};

	template<bool IsWords>
	string main_listener(bool f) {
		APMenu<IsWords> apm(f);
		string cmd;
		cout << "[AlpabetPointer menu]:\n>> ";
		getline(cin, cmd);
		string cmd_t = _GetCommand(cmd);
		while (cmd_t != "back" && cmd_t != "create" && cmd_t != "exit") {//в целом крутимся тут
			DynamicArray<Argument<string>>* arr = _MakeArgumentList<string>(cmd, false);
			try {
				apm.Process(cmd_t, arr);
			}
			catch (SetException e) {
				cout << e.message();
			}
			cout << "[AlpabetPointer menu]:\n>> ";
			getline(cin, cmd);
			cmd_t = _GetCommand(cmd);
		}
		return cmd;
	}

	string receive(string cmd) {
		while (_GetCommand(cmd) != "back" && _GetCommand(cmd) != "exit") {
			DynamicArray<Argument<string>>* arr = _MakeArgumentList<string>(cmd, false);
			if (_GetCommand(cmd) == "create") {
				enum getter { Sym, Words };
				getter get = Words;
				bool f = true;//непустой ли
				for (int i = 0; i < arr->GetSize(); i++) {
					if (arr->Get(i).GetFlag() == 's') {
						if (arr->Get(i).Getvalue() != "") throw SetException(IncorrectValue);
						get = Sym;
					}
					else if (arr->Get(i).GetFlag() == 'e') {
						if (arr->Get(i).Getvalue() != "") throw SetException(IncorrectValue);
						f = false;
					}
					else {
						throw SetException(UnknownFlag);
					}
				}
				cmd = "";
				while (_GetCommand(cmd) != "back" && _GetCommand(cmd) != "create" && _GetCommand(cmd) != "exit") {//если вернутся они, тогда вернемся в еще выше
					try {
						if (get == Words) {
							cmd = main_listener<true>(f);
						}
						else if (get == Sym) {
							cmd = main_listener<false>(f);
						}
					}
					catch (SetException e) {
						cout << e.message();
					}
				}
			}
			else if (cmd == "") {
				cout << "[AlpabetPointer menu]:\n>> ";
				getline(cin, cmd);
			}
			else if (cmd == "help") {
				help();
				cout << "[AlpabetPointer menu]:\n>> ";
				getline(cin, cmd);
			}
			else {
				throw SetException(EmptyAP);
			}
		}
		return cmd;
	}
}








namespace SparseMatrix_menu {

	void help() {
		cout << "\n";
		cout << "\tСписок команд: \n\n\t[] - обязательные аргументы, {} - опциональные аргументы\n";
		cout << "\n\t======================================= \n\n";
		cout << "\tcreate{-t тип(int,double)} {-e создать пустую матрицу } {-r заполнить рандомными числами} - создать матрицу\n";
		cout << "\tset[-i на что заменить] - заменить элемент матрицы\n";
		cout << "\tprint - вывести матрицу\n";
		cout << "\tadd_col - добавить столбец в матрицу\n";
		cout << "\tremove_col[-n номер] - удалить столбец из матрицы(начиная с 0)\n";
		cout << "\tadd_row - добавить строку в матрицу\n";
		cout << "\tremove_row[-n номер] - удалить строку из матрицы(начиная с 0)\n\n";
		cout << "\t====================================== = \n\n";
	}


	//для разбиения строки (split), заполняет da элементами типа fillT(то есть какими типа нужно заполнить массив)
	template<typename fillT>
	void fill_bins(DynamicArray<fillT>& da, string str) {
		string cur;
		for (int i = 0; i < str.size(); i++) {
			if (str[i] != ' ') cur += str[i];
			else {
				da.Append(_TypeCast<fillT>::cast(cur));
				cur = "";
			}
		}
		if (cur != "")
			da.Append(_TypeCast<fillT>::cast(cur));
	}

	//создание матрицы: f-заполняет чел, Rand - рандомно заполнить, hor, ver - угадай)))
	template<typename T>
	sparse_matrix<T>* make_matrix(bool f, int hor, int ver, bool isRand) {
		if (hor < 0) throw SetException(IncorrectValue);
		if (ver < 0) throw SetException(IncorrectValue);
		if (f && isRand) throw SetException(MutuallyExclusive);
		sparse_matrix<T>* sm = new sparse_matrix<T>(hor, ver);
		string str = "";
		if (isRand) {
			for (int i = 0; i < hor; i++) {
				for (int j = 0; j < ver; j++) {
					sm->set(i, j, rand() % 5 == 0 ? random_unit<T>::generate() : T());
				}
			}
			return sm;
		}
		if (f) {
			for (int i = 0; i < hor; i++) {
				DynamicArray<T> row;
				cout << "Enter row:\n>> ";
				getline(cin, str);
				fill_bins<T>(row, str);//чтение строки и перевод в массив
				if (row.GetSize() != ver) throw SetException(IncorrectRange);
				for (int j = 0; j < ver; j++) {
					sm->set(i, j, row[j]);
				}
			}
		}
		return sm;
	}


	template<typename T>
	class SMMenu {
	private:
		sparse_matrix<T>* sm = nullptr;

	public:
		SMMenu(bool f, bool isRand) {
			cout << "\nEnter size:\n>> ";
			DynamicArray<T> arr;
			string str;
			getline(cin, str);
			fill_bins(arr, str);
			if (arr.GetSize() != 2) throw SetException(IncorrectValue);
			int hor = arr[0];
			int ver = arr[1];
			sm = make_matrix<T>(f, hor, ver, isRand);
		}
		void Process(string cmd, DynamicArray<Argument<string>>* arr) {
			if (cmd == "print") {
				if (arr->GetSize() != 0) throw SetException(UnknownFlag);
				sm->print();
			}
			else if (cmd == "set") {
				if (arr->GetSize() > 1) throw SetException(UnknownFlag);
				if (arr->GetSize() == 0) throw SetException(NoRequiredArgument);
				if (arr->Get(0).GetFlag() != 'i') throw SetException(UnknownFlag);
				cout << "\nEnter coordinates:\n>> ";
				DynamicArray<T> coords;
				string str;
				getline(cin, str);
				fill_bins(coords, str);
				if (coords.GetSize() != 2) throw SetException(IncorrectValue);
				sm->set(coords[0], coords[1], _TypeCast<T>::cast(arr->Get(0).Getvalue()));
			}
			else if (cmd == "add_row") {
				if (arr->GetSize() > 0) throw SetException(UnknownFlag);
				string str;
				cout << "Enter row:\n>> ";
				getline(cin, str);
				DynamicArray<T> row;
				fill_bins<T>(row, str);
				sm->add_row(row);
			}
			else if (cmd == "add_col") {
				if (arr->GetSize() > 0) throw SetException(UnknownFlag);
				string str;
				cout << "Enter column:\n>> ";
				getline(cin, str);
				DynamicArray<T> row;
				fill_bins<T>(row, str);
				sm->add_column(row);
			}
			else if (cmd == "remove_col") {
				if (arr->GetSize() != 1) throw SetException(UnknownFlag);
				if (arr->Get(0).GetFlag() != 'n') throw SetException(UnknownFlag);
				sm->remove_column(_TypeCast<int>::cast(arr->Get(0).Getvalue()));
			}
			else if (cmd == "remove_row") {
				if (arr->GetSize() != 1) throw SetException(UnknownFlag);
				if (arr->Get(0).GetFlag() != 'n') throw SetException(UnknownFlag);
				sm->remove_row(_TypeCast<int>::cast(arr->Get(0).Getvalue()));
			}
			else if (cmd == "help") {
				help();
			}
			else {
				throw SetException(UnknownCommand);
			}
		}
		~SMMenu() = default;
	};

	template<typename T>
	string main_listener(bool f, bool isRand) {
		SMMenu<T> apm(f, isRand);
		string cmd;
		cout << "[Sparse matrix menu]:\n>> ";
		getline(cin, cmd);
		string cmd_t = _GetCommand(cmd);
		while (cmd_t != "back" && cmd_t != "create" && cmd_t != "exit") {
			DynamicArray<Argument<string>>* arr = _MakeArgumentList<string>(cmd, false);
			try {
				apm.Process(cmd_t, arr);
			}
			catch (SetException e) {
				cout << e.message();
			}
			cout << "[Sparse matrix menu]:\n>> ";
			getline(cin, cmd);
			cmd_t = _GetCommand(cmd);
		}
		return cmd;
	}

	string receive(string cmd) {
		while (_GetCommand(cmd) != "back" && _GetCommand(cmd) != "exit") {
			DynamicArray<Argument<string>>* arr = _MakeArgumentList<string>(cmd, false);
			if (_GetCommand(cmd) == "create") {
				enum type { Int, Float };
				type T = Int;
				bool f = true;//заполнять или нет говорить
				bool isRand = false;
				for (int i = 0; i < arr->GetSize(); i++) {
					if (arr->Get(i).GetFlag() == 't') {
						if (arr->Get(i).Getvalue() == "int") {
							T = Int;
						}
						else if (arr->Get(i).Getvalue() == "double") {
							T = Float;
						}
						else {
							throw SetException(IncorrectValue);
						}
					}
					else if (arr->Get(i).GetFlag() == 'r') {
						if (arr->Get(i).Getvalue() != "") throw SetException(IncorrectValue);
						f = false;
						isRand = true;
					}
					else if (arr->Get(i).GetFlag() == 'e') {
						if (arr->Get(i).Getvalue() != "") throw SetException(IncorrectValue);
						f = false;
					}
					else {
						throw SetException(UnknownFlag);
					}
				}
				cmd = "";
				try {
					if (T == Int) {
						cmd = main_listener<int>(f, isRand);
					}
					else if (T == Float) {
						cmd = main_listener<double>(f, isRand);
					}
				}
				catch (SetException e) {
					cout << e.message();
				}
			}
			else if (cmd == "") {
				cout << "\n[Sparse matrix menu]:\n>> ";
				getline(cin, cmd);
			}
			else if (cmd == "help") {
				help();
				cout << "[Sparse matrix menu]:\n>> ";
				getline(cin, cmd);
			}
			else {
				throw SetException(EmptySM);
			}
		}
		return cmd;
	}
}
