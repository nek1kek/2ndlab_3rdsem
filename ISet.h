#pragma once
#include "RBtree.h"
#include "DynamicArray.h"

template<typename _Key, bool MultiSet = false, class _cmp = MyComparator<_Key>>
class ISet : protected RBTree<_Key, _Key, MultiSet, _cmp> {//если публичное, то остаетс€ все теми же уровн€ми функци€ми, если приватное, тогда все публичные стали приватные, 
protected:
	using RBT = RBTree<_Key, _Key, MultiSet, _cmp>;
	void inOrder(RBT::Node* cur)const {
		if (cur != NULL) {
			inOrder(cur->left);
			typename LinkedList<_Key>::iterator it = cur->data.second.begin();

			while (it != cur->data.second.end()) {
				cout << *it << " ";
				it++;
			}
			inOrder(cur->right);
		}
	}

public:

	/*
	ѕример того как можно брать в случае наследовани€ переменные
	void Get123() {
		cout << this->_size;//нужно говорить именно от сюда что берем
		cout << RBTree<_Key, _Key, MultiSet, _cmp>::_size;//так как шаблон зависит от параметров шаблона класс, то нужно делать так
	
		// если заюзана using RBT = RBTree<_Key, _Key, MultiSet, _cmp>;
		cout << RBT::amount();
	}
	*/

	ISet() : RBTree<_Key, _Key, MultiSet, _cmp>() {};
	ISet(const ISet<_Key, MultiSet, _cmp>& other) : RBTree<_Key, _Key, MultiSet, _cmp>(other) {};
	ISet(LinkedList<_Key> t) : RBTree<_Key, _Key, MultiSet, _cmp>() {
		typename LinkedList<_Key>::iterator it = t.begin();
		while (it != t.end()) {	
			this->insert(*it);
			it++;
		}
	};


	bool empty()
	{
		return RBT::_size == 0;
	}
	void insert(const _Key& key) {
		RBTree<_Key, _Key, MultiSet, _cmp>::insert(key, key);
	}
	void remove(const _Key& key) {
		RBTree<_Key, _Key, MultiSet, _cmp>::remove(key,key);
	}

	int count(const _Key& key) {//возращает кол-во элементов со значением таким
		try {
			return this->get(key).GetLength();
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

	void print() {
		if (RBT::root == NULL)
			cout << "empty ISet";
		else {
			cout << "Values in ISet";
			if (MultiSet) cout << "(MultiSet): ";
			else cout << "(SimpleSet): ";
			inOrder(RBT::root);
			cout << endl;
		}
	}
	size_t amount() { return this->_size; };
	_Key get_min() const {//не мен€ет объект
		 typename RBT::Node* cur = RBT::root;
		 if (cur == nullptr) {
			 throw SetException(EmptySet);
		 }
		 while (cur->left != nullptr) {
			 cur = cur->left;
		 }
		 //if (cur == nullptr) throw SetException(IncorrectRange);
		 return cur->data.second[0];
	}
	_Key get_max() const {//не мен€ет объект
		 typename RBT::Node* cur = RBT::root;//заюзал сокарщение using(в самом начале увидешь это)
		 if (cur == nullptr) {
			 throw SetException(EmptySet);
		 }
		 while (cur->right != nullptr) {
			 cur = cur->right;
		 }
		 //if (cur == nullptr) throw SetException(IncorrectRange);
		 return cur->data.second[0];
	 }

	 //оператор присваивани€ как конструктор копировани€
	 

};