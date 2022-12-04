#pragma once
#include "LinkedList.h"
#include "DynamicArray.h"
#include <iostream>



/*начинаем делать его и гистограмму на нем*/



template<typename _Key, typename _Value, class _Hash = std::hash<_Key>, class _cmp = std::equal_to<_Key>>
class  HashTable
{
private:
	_cmp comparator;
	_Hash hasher;//_Hash это класс, который создает лишь 1 объект который умеет хешировать, потому не нужно ничего к нему присваивать как объект класса ISorter

	size_t size;
	size_t hash_table_capacity = 1;
	const float max_load_factor = 0.75;
	float max_in_linkedlist = 3;//ПРОВЕРИТЬ РАБОТАЕТ ЛИ ПРИ 1

	DynamicArray<LinkedList>


	struct Node {
		size_t hash;
		std::pair<const _Key, _Value> data;//пара ключ значение

		Node() = default;
		Node(const Node& other) : data(other.data)//двоеточие - а запихнуй сразу в дата инфу с другого
		Node(const _Key& key, const _Value& value) : data(key, value) {}
	
		
	};


private:


public:
	//вставка и удаления
	void insert(const _Key key, const _Value value) {   // Вставляем узел, ключ это значение ключа, внешний интерфейс

	};
	void remove(_Key key) {		// Удалить ключевой узел

	}
	//поиск
	Node* search(_Key key) {

	}

	//для печати(КЛП)
	void print() {		// напечатать дерево

	}

	};
