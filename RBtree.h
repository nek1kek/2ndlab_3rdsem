#pragma once
#include "exception.h"
#include "LinkedList.h"

using namespace std;

template<typename T, typename V>//это у нас ключ значение
struct my_pair {
	T first;
	V second;
	my_pair() = default;
	~my_pair() = default;

	my_pair(const my_pair<T, V>& other) {
		this->first = other.first;
		this->second = other.second;
	}
	my_pair(T a, V b) {
		first = a;
		second = b;
	}

	bool operator > (const my_pair<T, V>& other) const {//этот конст говорит, что мы ничего не меняем в классе, если бы был в начале, то возвращал бы константный бул
		return this->first > other->first;
	}
	bool operator < (const my_pair<T, V>& other) const {
		return this->first < other->first;
	}
	bool operator ==  (const my_pair<T, V>& other) const {
		return this->first == other->first;
	}
	bool operator != (const my_pair<T, V>& other) const {
		return this->first != other->first;
	}
};



template<typename _Key, typename _Value, bool CanChangeValue = true>
class  RBTree
{
private:
	enum _Color { BLACK, RED, None };
	struct Node {
		Node* left;
		Node* right;
		Node* parent;
		std::pair<const _Key, _Value> data;//пара ключ значение
		_Color color;

		Node() {
			right = nullptr;
			left = nullptr;
			parent = nullptr;
		}

		Node(const Node& other) : data(other.data) {//двоеточие - а запихнуй сразу в дата инфу с другого
			this->color = other.color;
			left = nullptr;
			right = nullptr;
			parent = nullptr;
		}
		Node(const _Key& key, const _Value& value, const _Color& color, Node* p, Node* l, Node* r) : data(key, value) {
			this->color = color;
			left = p;
			right = l;
			parent = r;
		}

	};

	Node* root;//туда всавляется самый первый элемент
	size_t _size;

private://функции закрытые от людей, вспомогательные для интерфейсных
	void leftRotate(Node*& root, Node* x) {// левый поворот
		Node* y = x->right;
		x->right = y->left;
		if (x->right != NULL)
			x->right->parent = x;

		y->parent = x->parent;
		if (y->parent == NULL)
			root = y;
		else {
			if (x == x->parent->left)
				x->parent->left = y;//чтобы поставить у будущего отца 'y' что у это сын его
			else
				x->parent->right = y;
		}
		y->left = x;
		x->parent = y;
	};
	void rightRotate(Node*& root, Node* y) {// правый поворот
		Node* x = y->left;
		y->left = x->right;
		if (x->right != NULL)
			x->right->parent = y;

		x->parent = y->parent;
		if (y->parent == NULL)
			root = x;
		else {
			if (y == y->parent->right)
				y->parent->right = x;
			else
				y->parent->left = x;
		}
		x->right = y;
		y->parent = x;
	};

	void insert(Node*& root, Node* node) {	// Вставляем узел, внутренний интерфейс
		//тут тупо, просто доходим до нужного элемента и вставляем, покрасив в красный
		Node* x = root;
		Node* y = NULL;
		while (x != NULL)
		{
			y = x;
			if (node->data.first > x->data.first)
				x = x->right;
			else
				x = x->left;
		}
		node->parent = y;
		if (y != NULL)
		{
			if (node->data.first > y->data.first)
				y->right = node;
			else
				y->left = node;
		}
		else
			root = node;
		node->color = RED;
		InsertFixUp(root, node);
	};
	void InsertFixUp(Node*& root, Node* node){
		//Владимир Владимирович, не бейте пж, но я лекции МФТИ смотрел...
		//https://www.youtube.com/watch?v=RnQYXltlkrI
		Node* parent = node->parent;
		while (node != RBTree::root && parent->color == RED)//(рекурсивно доходим до корня при условиях везде красный) условие, что отец красный 
		{
			Node* gparent = parent->parent;
			if (gparent->left == parent)
			{
				Node* uncle = gparent->right;
				if (uncle != NULL && uncle->color == RED)//условие при красном дяде (в тетке под пунктом 3а)
				{
					parent->color = BLACK;
					uncle->color = BLACK;
					gparent->color = RED;
					node = gparent;//идем рекурсивно к деду, как и писал выше
					parent = node->parent;//тут еще нужно будет проверить вышестоящих
				}
				else//дядя черный, отец красный
				{
					if (parent->right == node)//в тетке рисунок справа
					{
						leftRotate(root, parent);//Хитро да), в тетке рисунок слева
						swap(node, parent);
					}
					rightRotate(root, gparent);//в тетке рисунок спрва
					gparent->color = RED;
					parent->color = BLACK;
					break;//а тут уже не нужно будет рекурсивно, так как черным стал, а черным хоть сколько подряд)
				}
			}
			else
			{
				Node* uncle = gparent->left;
				if (uncle != NULL && uncle->color == RED)
				{
					gparent->color = RED;
					parent->color = BLACK;
					uncle->color = BLACK;

					node = gparent;
					parent = node->parent;
				}
				else
				{
					if (parent->left == node)
					{
						rightRotate(root, parent);
						swap(parent, node);
					}
					leftRotate(root, gparent);
					parent->color = BLACK;
					gparent->color = RED;//так как сделали поворот(+swap), то самый верхний черный уже
					break;//а тут уже не нужно будет рекурсивно, так как черным стал, а черным хоть сколько подряд)
				}
			}
		}
		root->color = BLACK;
	}

	void destroy(Node*& node) {		//удалить все поддерево с этого узла
		if (node == NULL)
			return;
		destroy(node->left);
		destroy(node->right);
		delete node;
		node = nullptr;
	}
	void remove(Node*& root, Node* node){
		Node* child, * parent;
		_Color color;
		// Есть 2 ребенка
		if (node->left != NULL && node->right != NULL)
		{
			Node* replace = node;
			// Найти самый нижний левый узел правого поддерева текущего узла
			replace = node->right;
			while (replace->left != NULL)
			{
				replace = replace->left;
			}
			// Случай, когда удаленный узел не является корневым узлом
			if (node->parent != NULL)
			{//тупо смотрим куда вставлять для отца ссылку
				if (node->parent->left == node)
					node->parent->left = replace;
				else
					node->parent->right = replace;
			}
			// Ситуация с корневым узлом
			else
				root = replace;
			// child - это правильный узел, который заменяет наш и является узлом, который требует последующей корректировки
			//ТАК КАК МЫ ЗАМЕНИЛИ НА САМЫЙ ЛЕВЫЙ СПРАВА NODE, ТО У НЕГО (REPLACE) МОЖЕТ БЫТЬ ТОЛЬКО ПРАВЫЙ СЫНОК
			/*
				   node
					/ \
				  ...   *
					   /\
					...	 ...		
					/   \
				replace  ...
				  /  \
			   None  child
			*/
			
			
			child = replace->right;
			parent = replace->parent;
			color = replace->color;

			// Удаленный узел является родительским узлом замещающего узла (replace)
			if (parent == node)
				parent = replace;
			else
			{	//я сука на следующий день встал и забыл как ночью дописал, легче рисовать))))
				// Существование дочернего узла
				if (child != NULL)
					child->parent = parent;
				parent->left = child;

				replace->right = node->right;
				node->right->parent = replace;
			}
			replace->parent = node->parent;
			replace->color = node->color;
			replace->left = node->left;
			node->left->parent = replace;
			if (color == BLACK)//не забываем, что на красного всем пох, удалили и х*й с ним)))
				removeFixUp(root, child, parent);//мы передаем ребенка и отца

			delete node;
			return;
		}
		// Когда 1 ребенок, найдите дочерний узел удаленного узла
		if (node->left != NULL)
			child = node->left;
		else
			child = node->right;

		//в целом если сирот не будет, то тоже норм
		parent = node->parent;
		color = node->color;

		if (child) child->parent = parent;

		// Удаленный узел не является корневым узлом
		if (parent)
		{
			if (node == parent->left)
				parent->left = child;
			else
				parent->right = child;
		}
		// Удаленный узел является корневым узлом
		else
			RBTree::root = child;// а это было гениально называть локальную переменную так же как глобальную хуярь с этим теперь)))

		if (color == BLACK)
		{
			removeFixUp(root, child, parent);//мы передаем ребенка и отца у удаленного элемента прошу заметить
		}
		delete node;

	}
	void removeFixUp(Node*& root, Node* node, Node* parent){
		Node* othernode;//брат короче
		while ((!node) || node->color == BLACK && node != RBTree::root)
		{
			if (parent->left == node)
			{
				othernode = parent->right;
				if (othernode->color == RED)//случай а) в тетке
				{
					othernode->color = BLACK;
					parent->color = RED;
					leftRotate(root, parent);
					othernode = parent->right;//чтобы перейти в случай б) в тетке
				}
				else
				{
					if (!(othernode->right) || othernode->right->color == BLACK)
					{
						othernode->left->color = BLACK;
						othernode->color = RED;
						rightRotate(root, othernode);
						othernode = parent->right;
					}
					othernode->color = parent->color;
					parent->color = BLACK;
					othernode->right->color = BLACK;
					leftRotate(root, parent);
					node = root;
					break;
				}
			}
			else
			{
				othernode = parent->left;
				if (othernode->color == RED)
				{
					othernode->color = BLACK;
					parent->color = RED;
					rightRotate(root, parent);
					othernode = parent->left;
				}
				if ((!othernode->left || othernode->left->color == BLACK) && (!othernode->right || othernode->right->color == BLACK))
				{
					othernode->color = RED;
					node = parent;
					parent = node->parent;
				}
				else
				{
					if (!(othernode->left) || othernode->left->color == BLACK)
					{
						othernode->right->color = BLACK;
						othernode->color = RED;
						leftRotate(root, othernode);
						othernode = parent->left;
					}
					othernode->color = parent->color;
					parent->color = BLACK;
					othernode->left->color = BLACK;
					rightRotate(root, parent);
					node = root;
					break;
				}
			}
		}
		if (node)
			node->color = BLACK;
	}

	Node* search(Node* node, _Key key) const{
		if (node == NULL || node->data.first == key)
			return node;//он либо вернет нуль-листье, либо найденный
		else
			if (key > node->data.first)
				return search(node->right, key);
			else
				return search(node->left, key);
	}

	void print(Node* node)const {  //напечатать уже с какого-то узла
		if (node == NULL)
			return;
		if (node->parent == NULL)
			cout << node->data.first << "(" << node->color << ") is root" << endl;
		else if (node->parent->left == node)
		{
			cout << node->data.first << "(" << node->color << ") is " << node->parent->data.first << "'s " << "left child" << endl;
		}
		else
		{
			cout << node->data.first << "(" << node->color << ") is " << node->parent->data.first << "'s " << "right child" << endl;
		}
		print(node->left);
		print(node->right);
	}

	void preOrder(Node* tree)const{
		if (tree != NULL) {
			cout << tree->data.first << " ";
			preOrder(tree->left);
			preOrder(tree->right);
		}
	}
	void inOrder(Node* tree)const{
		if (tree != NULL) {
			inOrder(tree->left);
			cout << tree->data.first<< " ";
			inOrder(tree->right);
		}
	}
	void postOrder(Node* tree)const{
		if (tree != NULL) {
			postOrder(tree->left);
			postOrder(tree->right);
			cout << tree->data.first << " ";
		}
	}


	bool Equals(const Node* cur, const Node* other) const {
		if (cur == other && cur == nullptr) return true;
		if (cur->data != other->data) return false;
		if (cur->color != other->color) return false;
		return Equals(cur->left, other->left) && Equals(cur->right, other->right);
	}


	void make_tree(Node* cur, const Node* other) {
		if (other == nullptr) return;
		if (other->left != nullptr) {
			cur->left = new Node(*other->left);
			cur->left->parent = cur;
		}
		if (other->right != nullptr) {
			cur->right = new Node(*other->right);
			cur->right->parent = cur;
		}
		make_tree(cur->left, other->left);
		make_tree(cur->right, other->right);
	
	}


public://функции открытые для людей - интерфейс
	RBTree() {		//Конструктор
		root = nullptr;
		_size = 0;
	}
	RBTree(const RBTree<_Key, _Value>& other) {		//Конструктор создавающий объект при получении другого объекта
		root = new Node(*other.root);
		_size = other._size;
		make_tree(root, other.root);
	}
	~RBTree() {		// Деструктор
		destroy(root);
	}

	//вставка и удаления
	void insert(const _Key key, const _Value value) {   // Вставляем узел, ключ это значение ключа, внешний интерфейс
		Node* z = new Node(key, value, RED, NULL, NULL, NULL);
		insert(root, z);
	};
	void remove(_Key key){		// Удалить ключевой узел
		Node* deletenode = search(root, key);
		if (deletenode != NULL)
			remove(root, deletenode);
	}

	//поиск
	Node* search(_Key key){
		return search(root, key);
	}

	//для печати(КЛП)
	void print() {		// напечатать дерево
		if (root == NULL)
			cout << "empty RBtree";
		else
			print(root);
	}

	//они нужны для обхода
	void preOrder(){			// Корень-Левый-Правый обход
		if (root == NULL)
			cout << "empty RBtree";
		else
			preOrder(root);
	};
	void inOrder(){   // Левый-Корень-Правый обход
		if (root == NULL)
			cout << "empty RBtree";
		else
			inOrder(root);
	};
	void postOrder(){   // Левый-Правый-Корень обход
		if (root == NULL)
			cout << "empty RBtree";
		else
			postOrder(root);
	};

	//сравнения деревьев:
	bool Equals(const RBTree<_Key, _Value>& other) const {
		return Equals(this->root, other.root);
	}

	bool operator==(const RBTree<_Key, _Value, CanChangeValue>& other) const {
		return this->Equals(other);
	}
	bool operator!=(const RBTree<_Key, _Value, CanChangeValue>& other) const {
		return !this->Equals(other);
	}
};


















