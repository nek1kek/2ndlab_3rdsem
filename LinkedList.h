﻿#pragma once
#include "exception.h"


template<typename T, bool IsConst>
class BidirectionalIterator;

template<typename T>
class LinkedList;

template<typename T>
size_t distance(typename LinkedList<T>::iterator begin, typename LinkedList<T>::iterator end) {//возвращает расстояние между объектами
	size_t res = 0;
	while (begin != end) {
		begin++;
		res++;
	}
	return res;
}



template<typename V>
struct ListIterators;

template <class T>
class LinkedList
{
	template<typename V, bool X>
	friend class BidirectionalIterator;//и означает, что к его приватным переменным может образаться bidirectional
public:
	int size = 0;
	struct Item {
		Item* next = nullptr;
		Item* prev = nullptr;
		T data = T();
		Item(T data) {
			this->data = data;
		}
		Item() {
			this->data = T();
		}
		~Item() {
		}
	};
	struct List {
		Item* head = nullptr;
		Item* tail = nullptr;
		List() {
			this->head = nullptr;
			this->tail = nullptr;
		}
		~List() {
		}
	};

	List* items = nullptr;
	List* CreateList() {
		List* list;
		try {
			list = new List();
		}
		catch (std::bad_alloc) {
			throw SetException(MemoryAllocateError);
		}
		return list;
	}

	void clear() {
		if (!items) return;
		Item* cur = items->head;
		while (cur != nullptr) {
			Item* cur2 = cur->next;
			delete cur;
			cur = cur2;
		}
		if (items) {
			delete items;
		}
	}

	void push_back(T data) {//tail показывает на следующий после последнего - ИМЕЕТ ВНУТРИ СЕБЯ ПОЛНУЮ ХУЙНЮ, для итератора так сделал
		Item* cur;
		try {
			cur = new Item(data);
		}
		catch (std::bad_alloc) {
			throw SetException(MemoryAllocateError);
		}
		if (size != 0) {
			cur->prev = items->tail->prev;
			items->tail->prev->next = cur;
			cur->next = items->tail;
			items->tail->prev = cur;
		}
		else {
			if (items->tail == nullptr) items->tail = new Item();//ТИПО ТУДА ФИКТИВНУЮ ПАРАШУ ПОЛОЖИЛИ
			cur->next = items->tail;
			items->tail->prev = cur;
			cur->prev = items->head;
			items->head = cur;
		}
		size++;
	}

	void push(T data, Item* item) {//ВСТАВИТЬ ВМЕСТО НА МЕСТО N ЭЛЕМЕНТА, А ОСТАЛЬНЫЕ ПОДВИНУТЬ

		if (size == 0) {
			push_back(data);
			return;
		}
		Item* newitem;
		try {
			newitem = new Item(data);
		}
		catch (std::bad_alloc) {
			throw SetException(MemoryAllocateError);
		}
		newitem->next = item;
		newitem->prev = item->prev;

		if (item->prev != nullptr) {
			item->prev->next = newitem;
		}
		else {
			items->head = newitem;
		}
		item->prev = newitem;
		size++;
	}

	void delete_item(Item* item) {
		if (item == nullptr) {
			throw SetException(IndexOutOfRange);
		}

		if (item->prev != nullptr && item->next != nullptr) {
			item->prev->next = item->next;
			item->next->prev = item->prev;
		}
		else if (item->prev == nullptr && item->next != nullptr) {
			items->head = item->next;
			items->head->prev = nullptr;
		}
		else {
			items->head = nullptr;
			items->tail->prev = nullptr;
		}

		delete item;
		size--;
	}

	void SetItem(int index, T item) {//поставить на N место 
		Item* cur = items->head;
		if (index < 0) {
			throw SetException(IndexOutOfRange);
		}
		for (int i = 0; cur != nullptr && i < index; i++) {
			cur = cur->next;
		}
		if (cur == nullptr) {
			throw SetException(IndexOutOfRange);
		}
		cur->data = item;
	}

	T& GetItem(int index) {
		Item* cur = items->head;
		if (index < 0) {
			throw SetException(IndexOutOfRange);
		}
		for (int i = 0; cur != nullptr && i < index; i++) {
			cur = cur->next;
		}
		if (cur == nullptr) {
			throw SetException(IndexOutOfRange);
		}
		return cur->data;
	}

	bool IsEqual(LinkedList<T>* other) {
		Item* cur = this->items->head;
		Item* cur2 = other->items->head;
		while (cur != nullptr && cur2 != nullptr) {
			if (cur->data != cur2->data) return false;
			cur = cur->next;
			cur2 = cur2->next;
		}
		if (cur != nullptr || cur2 != nullptr) {
			return false;
		}
		return true;
	}

	void push_tobegin(T data) {//вставить в начало
		if (size == 0) {
			push_back(data);
			return;
		}
		Item* cur = nullptr;
		try {
			cur = new Item(data);
		}
		catch (std::bad_alloc) {
			throw SetException(MemoryAllocateError);
		}

		cur->next = items->head;
		cur->prev = nullptr;
		items->head = cur;
		size++;
	}

public:
	//итераторы
	using iterator = BidirectionalIterator<T, false>;
	using const_iterator = BidirectionalIterator<T, true>;
	iterator begin() {
		iterator iter(this->items->head, this);
		return iter;
	}
	const_iterator cbegin() {
		const_iterator iter(this->items->head, this);
		return iter;
	}
	iterator end() {
		iterator iter(this->items->tail, this);
		return iter;
	}
	const_iterator cend() {
		const_iterator iter(this->items->tail, this);
		return iter;
	}

	//конструкторы
	LinkedList();
	LinkedList(size_t, T);
	LinkedList(T*, int);
	LinkedList(const LinkedList<T>&);
	LinkedList(LinkedList<T>&&);

	LinkedList(std::initializer_list<T>);

	//деконструктор
	~LinkedList();

	//функции
	T& GetFirst();
	T& GetLast();
	T& Get(int);
	int GetLength();
	void Append(T);
	void Prepend(T);
	void Set(int, T);
	void InsertAt(T, iterator);
	LinkedList<T>* GetSubList(iterator, iterator);
	LinkedList<T>* Concat(LinkedList<T>*);
	void del_item(iterator&);
	iterator find(iterator, iterator, T);
	bool Equals(LinkedList<T>*);
	bool IsSubList(LinkedList<T>*);

	//операторы
	T& operator[](int);
	bool operator==(LinkedList<T>&);
	bool operator!=(LinkedList<T>&);
	LinkedList<T>& operator=(const LinkedList<T>& other) {
		if (this != &other) {
			clear();
			if (!items)
				delete items;
			size = 0;
			items = CreateList();
			for (Item* it = other.items->head; it != other.items->tail; it = it->next) {
				this->push_back(it->data);
			}
			size = other.size;
		}
		return *this;
	}

	LinkedList<T>& operator=(LinkedList<T>&& other) {
		if (this != &other) {
			clear();
			if (!items)
				delete items;
			this->items = other.items;
			size = other.size;
			other.items = nullptr;
			other.size = 0;
		}

		return *this;
	}
};


template<typename T, bool IsConst>//давай забудь опять, что обозначает буловский isConst, сука ты же в сентябре делал ее, хули ты все забыл может названия читать будешь?) 
class BidirectionalIterator {
	template<typename T> friend class LinkedList;
private:// тайпнейм нужен, чтобы мы могли создать item типа Item
	using type = std::conditional_t<IsConst, const T, T>;
	typename LinkedList<T>::Item* item = nullptr;
	LinkedList<T>* arr = nullptr;
public:
	BidirectionalIterator(typename LinkedList<T>::Item* item, LinkedList<T>* arr) : arr(arr) {
		this->item = &(*item);
	}
	BidirectionalIterator(const BidirectionalIterator<T, IsConst>& other) : arr(other.arr) {
		this->item = &(*other.item);
	}
	type& operator *() {
		return item->data;
	}
	BidirectionalIterator<T, IsConst> operator ->() const {
		return *this;
	}
	BidirectionalIterator<T, IsConst>& operator++() {
		item = item->next;
		return *this;
	}
	BidirectionalIterator<T, IsConst>& operator--() {
		item = item->prev;
		return *this;
	}
	BidirectionalIterator<T, IsConst>& operator++(int) {
		item = item->next;
		return *this;
	}
	BidirectionalIterator<T, IsConst>& operator--(int) {
		item = item->prev;
		return *this;
	}

	BidirectionalIterator<T, IsConst>& operator+=(int t) {//СУКА ДОЛБОЕБ ЭТО НЕ РАНДОМАКССЕСИТЕРАТОР А НЕ БИДИРИКШЕНАЛ
		for (int i = 0; i < t; i++) {
			if (item->next == nullptr) {
				throw SetException(IncorrectRange);
			}
			item = item->next;
		}
		return *this;
	}
	BidirectionalIterator<T, IsConst>& operator-=(int t) {
		for (int i = 0; i < t; i++) {
			if (item->prev == nullptr) {
				throw SetException(IncorrectRange);
			}
			item = item->prev;
		}
		return *this;
	}
	BidirectionalIterator<T, IsConst> operator+(int t) {
		BidirectionalIterator<T, IsConst> newIterator(this->item, this->arr);// создали новый итератор и вернем новый с шагом +t
		for (int i = 0; i < t; i++) {
			newIterator.item = newIterator.item->next;
		}
		return newIterator;
	}
	BidirectionalIterator<T, IsConst> operator-(int t) {
		BidirectionalIterator<T, IsConst> newIterator(this->item, this->arr);
		for (int i = 0; i < t; i++) {
			newIterator.item = newIterator.item->prev;
		}
		return newIterator;
	}

	bool operator!=(const BidirectionalIterator<T, IsConst>& other) {
		if (this->arr != other.arr) return true;
		if (this->item != other.item) return true;
		return false;
	}
	bool operator==(const BidirectionalIterator<T, IsConst>& other) {
		if (this->arr != other.arr) return false;
		if (this->item != other.item) return false;
		return true;
	}
};


//конструкторы уже реализация
template<class T>
LinkedList<T>::LinkedList() {
	items = CreateList();
}

template<class T>
LinkedList<T>::LinkedList(size_t size, T value) {//сделать массив из одинаковых величин?
	items = CreateList();
	for (int i = 0; i < size; i++) {
		push_back(value);
	}
	this->size = size;
}

template<class T>
LinkedList<T>::LinkedList(T* items, int count) {
	this->items = CreateList();

	for (int i = 0; i < count; i++) {
		push_back(items[i]);
	}
	size = count;
}

template<class T>
LinkedList<T>::LinkedList(const LinkedList<T>& other) {
	items = CreateList();

	for (Item* it = other.items->head; it != nullptr && it != other.items->tail; it = it->next) {
		this->push_back(it->data);
	}
	size = other.size;
}

template<class T>
LinkedList<T>::LinkedList(LinkedList<T>&& other) : items(nullptr), size(0) {
	items = other.items;
	size = other.size;
	other.items = nullptr;
	other.size = 0;
}

template<class T>
LinkedList<T>::LinkedList(std::initializer_list<T> list) { //это когда задаешь через a{}
	items = CreateList();
	for (auto i : list) {
		push_back(i);
	}
	size = int(list.size());
}


//деконструктор
template<class T>
LinkedList<T>::~LinkedList() {
	clear();
}

//функции
template<class T>
T& LinkedList<T>::GetFirst() {
	return this->GetItem(0);
}

template<class T>
T& LinkedList<T>::GetLast() {
	return this->GetItem(this->size - 1);
}

template<class T>
T& LinkedList<T>::Get(int index) {
	return GetItem(index);
}

template<class T>
int LinkedList<T>::GetLength() {
	return size;
}

template<class T>
void LinkedList<T>::Append(T item) {
	push_back(item);
}

template<class T>
void LinkedList<T>::Prepend(T item) {
	push_tobegin(item);
}

template<class T>
void LinkedList<T>::Set(int index, T item) {
	SetItem(index, item);
}

template<class T>
void LinkedList<T>::InsertAt(T item, iterator it) {
	Item* cur = it.item;

	push(item, cur);
}

template<class T>
LinkedList<T>* LinkedList<T>::GetSubList(iterator start, iterator end) {
	if (distance<T>(this->begin(), start) > distance<T>(this->begin(), end)) throw SetException(NegativeRange);
	LinkedList<T>* list = new LinkedList;
	for (iterator i = start; i != end; i++) {
		list->Append(*i);
	}
	return list;
}

template<class T>
LinkedList<T>* LinkedList<T>::Concat(LinkedList<T>* list) {//сложить этот массив с каким-то другим
	LinkedList<T>* list_res = new LinkedList;
	for (int i = 0; i < size; i++) {
		list_res->Append(this->Get(i));
	}
	for (int i = 0; i < list->size; i++) {
		list_res->Append(list->Get(i));
	}
	return list_res;
}

template<class T>
void LinkedList<T>::del_item(iterator& it) {
	Item* cur = it.item;
	it.item = cur->next;
	delete_item(cur);
}

template<class T>
typename LinkedList<T>::iterator LinkedList<T>::find(iterator start, iterator end, T item) {
	if (distance<T>(begin(), start) >= distance<T>(begin(), end)) throw SetException(NegativeRange);
	for (iterator i = start; i != end; i++) {
		if (*i == item) return i;
	}
	return this->end();//вернет итератор на конец, а это у нас что?) правильно конец ахапхахп
}



template<class T>
bool LinkedList<T>::Equals(LinkedList<T>* seq) {
	if (this->GetLength() != seq->GetLength()) return false;
	for (int i = 0; i < seq->GetLength(); i++) {
		if (this->Get(i) != seq->Get(i)) return false;
	}
	return true;
}

template<class T>
bool LinkedList<T>::IsSubList(LinkedList<T>* seq) {//является ли подсписком нашего
	iterator pos = this->begin();
	for (iterator i = seq->begin(); i != seq->end() && pos != this->end(); i++) {
		pos = this->find(pos, this->end(), *i);//вернет итератор енд и если он будет таким вернем фалс
	}
	return pos != this->end();
}

//операторы
template<class T>
T& LinkedList<T>:: operator[](int index) {
	return GetItem(index);
}

template<class T>
bool LinkedList<T>:: operator==(LinkedList<T>& seq) {
	return this->Equals(&seq);
}

template<class T>
bool LinkedList<T>:: operator!=(LinkedList<T>& seq) {
	return !this->Equals(&seq);
}
