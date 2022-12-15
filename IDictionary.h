#pragma once
#include "RBTree.h"

template<typename _Key, typename _Value, bool ManyValuesDict = false, class _cmp = MyComparator<_Key>>
class IDictionary : protected RBTree<_Key, _Value, ManyValuesDict, _cmp> {
protected:
	_Value& get(const _Key& key) {
		return RBT::get(key)[0];
	}
public:
	using RBT = RBTree<_Key, _Value, ManyValuesDict, _cmp>;

	IDictionary() : RBT() {};
	IDictionary(const IDictionary<_Key, _Value, ManyValuesDict, _cmp>& other) : RBT(other) {};
	~IDictionary() = default;

	void inOrder(RBT::Node* cur, bool& putpoint)const {//��� ������
		if (cur != NULL) {
			inOrder(cur->left, putpoint);
			if (putpoint) { cout << ", "; }
			cout << cur->data.first << ": ";
			RBT::print_values(cur);
			putpoint = true;
			inOrder(cur->right, putpoint);
		}
	}
	void print() {//�������� ��������
		cout << "IDictionary: {";
		bool putpoint = false;
		inOrder(RBT::root, putpoint);
		cout << "}" << endl;
	}


	int count(const _Key& key) {//��������� 1 ��� ���������� �����, 0 ��� ���
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
	};//���-�� ������ � ������� ����������

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


	_Value& operator[](const _Key& key) {//��� �������� ��������
		//����� �� �� ���������� ������
		try {
			return this->get(key);//����� ������ ��� �� ����������, �� �������� = ����� dict[_Key] �� �������� �������� ����� ����� ����� 
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
