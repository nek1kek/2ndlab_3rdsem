#pragma once
#include "LinkedList.h"
#include "DynamicArray.h"
#include <iostream>



/*
�������� resize DynamicArray ��� Resize ������ ������ ���� ������� ��������













































template<typename _Key, typename _Value, class _Hash = std::hash<_Key>, class _cmp = std::equal_to<_Key>>
class  HashTable
{
private:
	_cmp comparator;
	_Hash hasher;//_Hash ��� �����, ������� ������� ���� 1 ������ ������� ����� ����������, ������ �� ����� ������ � ���� ����������� ��� ������ ������ ISorter

	size_t size;
	int hash_table_capacity = 1;
	const float max_load_factor = 0.75;
	int max_in_linkedlist = 3;//��������� �������� �� ��� 1

	DynamicArray<LinkedList<int>> Hash_vector(1,LinkedList<int>());


	struct Node {
		size_t hash;
		std::pair<const _Key, _Value> data;//���� ���� ��������

		Node() = default;
		Node(const Node& other) : data(other.data) {}//��������� - � �������� ����� � ���� ���� � �������
		Node(const _Key& key, const _Value& value,size_t _hash) : data(key, value) {
			hash = _hash;
		}
	
	};


private:
	void rehash() {//���������� ��� ����������� ����_����_������ ��� ����������������//���� �(2n)
		size_t hash_table_capacity = hash_table_capacity * 2 + 1;
		Hash_vector.Resize(hash_table_capacity)


	}

public:
	//������� � ��������
	void insert(const _Key key, const _Value value) {   // ��������� ����, ���� ��� �������� �����, ������� ���������
		size_t _hash = hasher(key);
		Node* (key, value, _hash);
		
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



	*/