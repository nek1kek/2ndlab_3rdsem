#pragma once
#include "RBTree.h"
#include "LinkedList.h"

template<typename _Key, typename _Value, bool ManyValuesDict = false, class _cmp = MyComparator<_Key>>
class IDictionary : public RBTree<_Key, _Value, ManyValuesDict, _cmp> {
public:
	_Value& get(const _Key& key) {
		return RBT::get(key)[0];
	}

	LinkedList<_Value>& get_list(const _Key& key) {
		return RBT::get(key);
	}
	using RBT = RBTree<_Key, _Value, ManyValuesDict, _cmp>;

	IDictionary() : RBT() {};
	IDictionary(const IDictionary<_Key, _Value, ManyValuesDict, _cmp>& other) : RBT(other) {};
	~IDictionary() = default;

	void inOrder(RBT::Node* cur, bool& putpoint)const {//для принта
		if (cur != NULL) {
			inOrder(cur->left, putpoint);
			if (putpoint) { cout << ", "; }
			cout <<endl << cur->data.first << ": ";
			RBT::print_values(cur);
			putpoint = true;
			inOrder(cur->right, putpoint);
		}
	}
	void print() {//выводить значения
		cout << "IDictionary: {";
		bool putpoint = false;
		inOrder(RBT::root, putpoint);
		cout << "\n}" << endl;
	}
	void print_values(_Key a ) {//выводить значения ключа
		RBT::print_values(a);
	}


	int count(const _Key& key) {//возращает 1 при нахождении ключа, 0 при нет
		try {
			_Value& a = this->get(key);
			return 1;
		}
		catch (SetException e) {
			if (e.id == NoSuchElement) {
				return 0;
			}
			else {
				throw e;
			}
		}
	};
	size_t amount() { 
		return this->_size;
	};//кол-во ключей в словаре возвращает

	void remove(const _Key& key) {
		try {
			RBT::remove(key);
		}
		catch (SetException e) {
			if (e.id != NoSuchElement) {
				throw e;
			}
		}
	}

	void insert(const _Key& key,const _Value& value) {
		RBT::insert(key, value);
	}


	_Value& operator[](const _Key& key) {//для возврата значения
		//ЕБАТЬ ОН ЖЕ ВОЗВРАЩАЕТ ССЫЛКУ
		try {
			return this->get(key);//ЕБАТЬ ПОТОМУ ЧТО ОН ВОЗВРАЩАЕТ, ТО ПРОПИСАВ = ПОСЛЕ dict[_Key] он присвоит значение после знака равно 
		}
		catch (SetException e) {
			if (e.id == NoSuchElement) {
				RBT::insert(key, _Value());
				return this->get(key);
			}
			else {
				throw e;
			}
		}
	}
};
