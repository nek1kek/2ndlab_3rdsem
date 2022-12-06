#pragma once
#include "LinkedList.h"
#include "DynamicArray.h"
#include <iostream>



/*
Проблема resize DynamicArray при Resize выдает ошибку типо удаляет элементы













































template<typename _Key, typename _Value, class _Hash = std::hash<_Key>, class _cmp = std::equal_to<_Key>>
class  HashTable
{
private:
	_cmp comparator;
	_Hash hasher;//_Hash это класс, который создает лишь 1 объект который умеет хешировать, потому не нужно ничего к нему присваивать как объект класса ISorter

	size_t size;
	int hash_table_capacity = 1;
	const float max_load_factor = 0.75;
	int max_in_linkedlist = 3;//ПРОВЕРИТЬ РАБОТАЕТ ЛИ ПРИ 1

	DynamicArray<LinkedList<int>> Hash_vector(1,LinkedList<int>());


	struct Node {
		size_t hash;
		std::pair<const _Key, _Value> data;//пара ключ значение

		Node() = default;
		Node(const Node& other) : data(other.data) {}//двоеточие - а запихнуй сразу в дата инфу с другого
		Node(const _Key& key, const _Value& value,size_t _hash) : data(key, value) {
			hash = _hash;
		}
	
	};


private:
	void rehash() {//вызывается при преодолении макс_лоад_фактор или максинлинкедлист//Сука О(2n)
		size_t hash_table_capacity = hash_table_capacity * 2 + 1;
		Hash_vector.Resize(hash_table_capacity)


	}

public:
	//вставка и удаления
	void insert(const _Key key, const _Value value) {   // Вставляем узел, ключ это значение ключа, внешний интерфейс
		size_t _hash = hasher(key);
		Node* (key, value, _hash);
		
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



	*/