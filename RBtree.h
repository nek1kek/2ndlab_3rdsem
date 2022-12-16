#pragma once
#include "exception.h"
#include "LinkedList.h"

using namespace std;

template<typename T>
struct MyComparator {
	short int operator()(const T& first, const T& second) const noexcept {
		if (first < second) return -1;
		if (first > second) return 1;
		return 0;
	}
};

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


template<typename _Key, typename _Value, bool IsMulti = false, class _cmp = MyComparator<_Key>>
class RBTree {
protected:
	enum _Color { BLACK, RED, None };
	struct Node {
		Node* right;
		Node* left;
		Node* parent;
		std::pair<const _Key, LinkedList<_Value>> data;
		_Color color;


		Node() : std::pair<const _Key, LinkedList<_Value>>(_Key(), LinkedList<_Value>()) {
			right = nullptr;
			left = nullptr;
			parent = nullptr;
		}
		Node(const Node& other) : data(other.data) {
			this->color = other.color;
			left = other.left;
			right = other.right;
			parent = other.parent;
		}
		Node(const Node* other) : data(other->data) {}
		Node(const _Key& key, const _Value& value, const _Color& color) : data(key, LinkedList<_Value>()) {
			this->color = color;
			data.second.Append(value);
			left = nullptr;
			right = nullptr;
			parent = nullptr;
		}
		~Node() = default;
	};

	_cmp comparator;// comparator(cur->data.first, key)>0 так используется для сравнения, если все же захочу юзать компаратор нужно будет везде заменить 
	Node* root = nullptr;//туда вставляется самый первый элемент
	size_t _size;

	
protected://функции закрытые от людей, вспомогательные для интерфейсных
	void clear(Node* cur) noexcept {//очистка дерева
		if (cur == nullptr) return;
		Node* right = cur->right;
		Node* left = cur->left;

		delete cur;
		clear(right);
		clear(left);
	}

	Node* get_uncle(const Node* cur) noexcept {
		if (cur->parent->parent == nullptr) {
			return nullptr;
		}
		if (cur->parent == cur->parent->parent->left)
			return cur->parent->parent->right;
		return cur->parent->parent->left;
	}
	Node* getbrother(const Node* p) noexcept {
		return isRight(p) ? p->parent->left : p->parent->right;
	}

	void rotateRight(Node* cur) noexcept {
		Node* p = cur->left;
		if (!p) return;
		if (cur == root) root = p;
		cur->left = p->right;
		if (p->right)
			p->right->parent = cur;
		if (isRight(cur) && cur->parent != nullptr) {
			cur->parent->right = p;
		}
		else if (cur->parent != nullptr) {
			cur->parent->left = p;
		}
		p->right = cur;
		p->parent = cur->parent;
		cur->parent = p;
	}
	void rotateLeft(Node* cur) noexcept {
		Node* p = cur->right;
		if (!p) return;
		if (cur == root) root = p;
		cur->right = p->left;
		if (p->left)
			p->left->parent = cur;
		if (isRight(cur) && cur->parent != nullptr) {
			cur->parent->right = p;
		}
		else if (cur->parent != nullptr) {
			cur->parent->left = p;
		}
		p->left = cur;
		p->parent = cur->parent;
		cur->parent = p;
	}

	Node* nearest_key(const Node* cur) noexcept {//вправо и влево до конца
		Node* res = cur->right;
		while (res->left != nullptr) {
			res = res->left;
		}
		return res;
	}
	bool isRight(const Node* cur) noexcept {
		if (cur->parent == nullptr) {
			return false;
		}
		return cur->parent->right == cur;
	}
	_Color GetChildColors(const Node* cur) noexcept {//вернет None, если разные цвета, либо один общий
		_Color res = BLACK;
		if (cur->right != nullptr) {
			if (cur->right->color == RED) {
				res = RED;
			}
			else {
				res = BLACK;
			}
		}
		if (cur->left != nullptr) {
			if (cur->left->color != res) {
				res = None;
			}
		}
		return res;
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

	bool Equals(const Node* cur, const Node* other) const noexcept {
		if (cur == other && cur == nullptr) return true;
		if (cur == nullptr) return false;
		if (other == nullptr) return false;
		if (cur->data.first != other->data.first) return false;
		if (cur->data.second != other->data.second) return false;
		if (cur->color != other->color) return false;
		return Equals(cur->left, other->left) && Equals(cur->right, other->right);
	}


	void fixNode(Node* cur) noexcept {
		while (cur->parent != nullptr && cur->parent->color == RED) {
			Node* uncle = get_uncle(cur);
			if (!isRight(cur->parent)) {
				if (uncle != nullptr && uncle->color == RED) {//в заметках 3а вставка
					cur->parent->color = BLACK;
					uncle->color = BLACK;
					cur->parent->parent->color = RED;
					cur = cur->parent->parent;
				}
				else {//3б вставкае
					if (isRight(cur)) {
						cur = cur->parent;
						rotateLeft(cur);
					}
					cur->parent->color = BLACK;
					cur->parent->parent->color = RED;
					rotateRight(cur->parent->parent);
				}
			}
			else {
				if (uncle != nullptr && uncle->color == RED) {
					cur->parent->color = BLACK;
					uncle->color = BLACK;
					cur->parent->parent->color = RED;
					cur = cur->parent->parent;
				}
				else {
					if (!isRight(cur)) {
						cur = cur->parent;
						rotateRight(cur);
					}
					cur->parent->color = BLACK;
					cur->parent->parent->color = RED;
					rotateLeft(cur->parent->parent);
				}
			}
		}
		this->root->color = BLACK;
	}
	Node* move_Node(Node* first, Node* second) {//копирует из второго в первый
		//скопируем всю инфу первого
		Node* parent = first->parent;
		Node* left = first->left;
		Node* right = first->right;
		_Color clr = first->color;
		bool check = (first->parent) == nullptr;
		bool isright = isRight(first);
		delete first;
		
		//переписываем все со второго в первый
		first = new Node(second);
		if (check) root = first;

		first->left = left;
		if (first->left != nullptr) {
			first->left->parent = first;
		}

		first->right = right;
		if (first->right != nullptr) {
			first->right->parent = first;
		}

		first->parent = parent;
		if (first->parent != nullptr) {
			if (isright)
				first->parent->right = first;
			else
				first->parent->left = first;
		}
		first->color = clr;
		return first;
	}


	void delete_Node(Node* cur) noexcept {

		//0 children - just delete this shit
		if (cur->left == nullptr && cur->right == nullptr) {
			if (cur->parent == nullptr) {//root
				root = nullptr;
				delete cur;
				return;
			}
			if (cur->color == RED) { //red
				if (isRight(cur)) {
					cur->parent->right = nullptr;
				}
				else {
					cur->parent->left = nullptr;
				}
				delete cur;
				return;
			}

			//black Node deleting
			fixdelete(cur);//only there we use fixdelete
			if (isRight(cur)) cur->parent->right = nullptr;
			else cur->parent->left = nullptr;
			delete cur;
			return;
		}

		//1 child
		if ((cur->left == nullptr) ^ (cur->right == nullptr)) {//see i know cool programming i use XOR))
			if (cur->parent == nullptr) { //cur - root, easy level
				this->root = cur->right != nullptr ? cur->right : cur->left;
				if (cur->right != nullptr) {
					cur->right->color = BLACK;
					cur->right->parent = nullptr;
				}
				else {
					cur->left->color = BLACK;
					cur->left->parent = nullptr;
				}
				delete cur;
				return;
			}

			//red impossible the fourth principle is violated(see in notes we must have equal black nodes)

			//black Node deleting
			if (cur->right != nullptr) {
				cur = move_Node(cur, cur->right);//we just place son on his place
				delete_Node(cur->right);//now we must delete same child
				return;
			}
			else {
				cur = move_Node(cur, cur->left);//we just place son on his place
				delete_Node(cur->left);//now we must delete same child 
				return;
			}
		}

		//2 children
		Node* nearest = nearest_key(cur);//right and down left to the end
		cur = move_Node(cur, nearest);//replace
		delete_Node(nearest);//delete nearest
		return;
	}
	void fixdelete(Node* p) noexcept {
		if (p == root) return;
		Node* bro = getbrother(p);
		if (bro->color == BLACK) { //black bro 2b in notes
			if (GetChildColors(bro) == BLACK) { //2бI case in notes
				bro->color = RED;
				if (p->parent->color == RED) {
					p->parent->color = BLACK;
					return;
				}
				else {
					fixdelete(p->parent);
					return;
				}
			}
			if (!isRight(p)) {
				if (bro->right != nullptr && bro->right->color == RED) { //right child is red
					bro->color = p->parent->color;
					if (bro->right != nullptr)
						bro->right->color = BLACK;
					bro->parent->color = BLACK;
					rotateLeft(bro->parent);
					return;
				}
				if (bro->left->color == RED && (bro->right == nullptr || bro->right->color == BLACK)) { //left child is red and right is black
					std::swap(bro->left->color, bro->color);//changing colors
					rotateRight(bro);
					fixdelete(p);
					return;
				}
			}
			else {//the same situation but inverse in Right/left
				if (bro->left != nullptr && bro->left->color == RED) { //right child is red
					bro->color = p->parent->color;
					if (bro->left != nullptr)
						bro->left->color = BLACK;
					bro->parent->color = BLACK;
					rotateRight(bro->parent);
					return;
				}
				if (bro->right->color == RED && (bro->left == nullptr || bro->left->color == BLACK)) { //left child is red and right is black
					std::swap(bro->right->color, bro->color);
					rotateLeft(bro);
					fixdelete(p);
					return;
				}
			}
		}
		
		//red bro 2a in notes
		p->parent->color = RED;
		bro->color = BLACK;
		if (!isRight(p)) {
			rotateLeft(p->parent);
		}
		else {
			rotateRight(p->parent);
		}
		fixdelete(p);//go to 2б in notes
		return;
	}


	void print_values(Node* node) const {
		//cout << "His keys: ";
		typename LinkedList<_Value>::iterator it = node->data.second.begin();
		if (node->data.second.GetLength() > 0){
			cout << *it;
			it++;
		}
		//вывели первый, а дальше уже запятая и элемент и погнали 
		while (it != node->data.second.end()) {
			cout << ", " << *it;
			it++;
		}

	}
	void print(Node* node)const {  //напечатать уже с какого-то узла
		if (node == NULL)
			return;
		if (node->parent == NULL) {
			cout << node->data.first << "(" << node->color << ") is root" << endl;
			print_values(node);
		}
		else if (node->parent->left == node)
		{
			cout << node->data.first << "(" << node->color << ") is " << node->parent->data.first << "'s " << "left child" << endl;
			print_values(node);
		}
		else
		{
			cout << node->data.first << "(" << node->color << ") is " << node->parent->data.first << "'s " << "right child" << endl;
			print_values(node);
		}
		print(node->left);
		print(node->right);
	}
	Node* search(Node* node, _Key key) const {
		if (node == NULL || node->data.first == key)
			return node;//он либо вернет нуль-листье, либо найденный
		else
			if (key > node->data.first)
				return search(node->right, key);
			else
				return search(node->left, key);
	}


public://функции открытые для людей - интерфейс
	//конструкторы и деструкторы
	RBTree(){	
		root = nullptr;
		_size = 0;
	}
	RBTree(const RBTree<_Key, _Value>& other)  {
		if (other.root == nullptr) return;
		root = new Node(*other.root);
		_size = other._size;
		make_tree(root, other.root);
	}
	RBTree(RBTree<_Key, _Value>&& other) {
		root = other.root;
		_size = other._size;
		other.root = nullptr;
	}
	~RBTree() {
		clear(root);
	}


	//добыть список по ключу
	LinkedList<_Value>& get(const _Key& key) {
		Node* cur = root;
		while (cur != nullptr) {
			if (comparator(cur->data.first, key) < 0) {
				cur = cur->right;
			}
			else if (comparator(cur->data.first, key) > 0) {
				cur = cur->left;
			}
			else {
				return cur->data.second;
			}
		}
		if (cur == nullptr) throw SetException(NoSuchElement);
	}




	//посмотреть сайз!!! -  это кол-во элементов все-таки а не ключей

	void insert(const _Key& key, const _Value& value) {
		if (this->root == nullptr) {
			this->root = new Node(key, value, BLACK);
			_size++;
			return;
		}
		Node* cur = this->root;
		Node* parent = cur;
		while (cur != nullptr) {
			if (comparator(cur->data.first, key) > 0) {
				parent = cur;
				cur = cur->left;
			}
			else if (comparator(cur->data.first, key) < 0) {
				parent = cur;
				cur = cur->right;
			}
			else {
				if (IsMulti) {
					cur->data.second.Append(value);
					_size++;
				}
				else cur->data.second[0] = value;
				return;
			}
		}
		_size++;
		if (comparator(parent->data.first, key) < 0) {
			parent->right = new Node(key, value, RED);
			parent->right->parent = parent;
			fixNode(parent->right);
		}
		else {
			parent->left = new Node(key, value, RED);
			parent->left->parent = parent;
			fixNode(parent->left);
		}
	}
	void remove(const _Key& key) {
		Node* cur = root;
		while (cur != nullptr) {
			if (comparator(cur->data.first, key) > 0) {
				cur = cur->left;
			}
			else if (comparator(cur->data.first, key) < 0) {
				cur = cur->right;
			}
			else {
				_size--;
				delete_Node(cur);
				return;
			}
		}
		throw SetException(NoSuchElement);
	}
	void remove(const _Key& key, const _Value& val) {
		Node* cur = root;
		while (cur != nullptr) {
			if (comparator(cur->data.first, key) > 0) {
				cur = cur->left;
			}
			else if (comparator(cur->data.first, key) < 0) {
				cur = cur->right;
			}
			else {
				_size--;
				//it-указатель на нужный val
				typename LinkedList<_Value>::iterator it = cur->data.second.find(cur->data.second.begin(), cur->data.second.end(), val);
				if (it == cur->data.second.end()) throw SetException(NoSuchElement);
				cur->data.second.del_item(it);

				if (cur->data.second.GetLength() == 0) {
					delete_Node(cur);
				}
				return;
			}
		}
		throw SetException(NoSuchElement);
	}


	/*удалить если не пригодится
	RBTree<_Key, _Value, IsMulti, _cmp>& operator=(const RBTree<_Key, _Value, IsMulti, _cmp>& other) {
		if (other.root == nullptr) return *this;
		root = new Node(*other.root);
		_size = other._size;
		make_tree(root, other.root);
		return *this;
	}

	RBTree<_Key, _Value, IsMulti, _cmp>& operator=(RBTree<_Key, _Value, IsMulti, _cmp>&& other) {
		root = other.root;
		_size = other._size;
		other.root = nullptr;
		return *this;
	}
	*/


	//поиск
	bool find(const _Key& key) const noexcept {
		Node* cur = root;
		while (cur != nullptr) {
			if (comparator(key, cur->data.first) < 0) {
				cur = cur->left;
			}
			else if (comparator(key, cur->data.first) > 0) {
				cur = cur->right;
			}
			else {
				return true;
			}
		}
		return false;
	}
	Node* search(_Key key) {
		return search(root, key);
	}

	//функции сравнения
	bool Equals(const RBTree<_Key, _Value, IsMulti, _cmp>& other) const noexcept {
		return Equals(this->root, other.root);
	}
	bool operator==(const RBTree<_Key, _Value, IsMulti, _cmp>& other) const noexcept {
		return this->Equals(other);
	}
	bool operator!=(const RBTree<_Key, _Value, IsMulti, _cmp>& other) const noexcept {
		return !this->Equals(other);
	}
	
	//доп хуйня
	size_t amount() {
		return _size;
	}
	void erase() {
		_size = 0;
		clear(root);
		root = nullptr;
	}
	void print() {		// напечатать дерево
		if (root == NULL)
			cout << "empty RBtree";
		else
			print(root);
	}
	void print_values(_Key key) {
		Node* node = search(root, key);
		if (node) {
			//cout << "Key: " << key << endl;
			print_values(node);
		}
	}



};

