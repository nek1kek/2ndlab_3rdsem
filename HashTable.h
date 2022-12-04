#pragma once
#include "LinkedList.h"
#include "DynamicArray.h"
#include <iostream>



/*�������� ������ ��� � ����������� �� ���*/



template<typename _Key, typename _Value, class _Hash = std::hash<_Key>, class _cmp = std::equal_to<_Key>>
class  HashTable
{
private:
	_cmp comparator;
	_Hash hasher;//_Hash ��� �����, ������� ������� ���� 1 ������ ������� ����� ����������, ������ �� ����� ������ � ���� ����������� ��� ������ ������ ISorter

	size_t size;
	size_t hash_table_capacity = 1;
	const float max_load_factor = 0.75;
	float max_in_linkedlist = 3;//��������� �������� �� ��� 1

	DynamicArray<LinkedList>


	struct Node {
		size_t hash;
		std::pair<const _Key, _Value> data;//���� ���� ��������

		Node() = default;
		Node(const Node& other) : data(other.data)//��������� - � �������� ����� � ���� ���� � �������
		Node(const _Key& key, const _Value& value) : data(key, value) {}
	
		
	};


private:


public:
	//������� � ��������
	void insert(const _Key key, const _Value value) {   // ��������� ����, ���� ��� �������� �����, ������� ���������

	};
	void remove(_Key key) {		// ������� �������� ����

	}
	//�����
	Node* search(_Key key) {

	}

	//��� ������(���)
	void print() {		// ���������� ������

	}

	};
