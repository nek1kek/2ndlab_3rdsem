#pragma once
#include "exception.h"
#include "LinkedList.h"

using namespace std;

template<typename T, typename V>//��� � ��� ���� ��������
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

	bool operator > (const my_pair<T, V>& other) const {//���� ����� �������, ��� �� ������ �� ������ � ������, ���� �� ��� � ������, �� ��������� �� ����������� ���
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

template<typename T>
struct MyComparator {
	short int operator()(const T& first, const T& second) const noexcept {
		if (first < second) return -1;
		if (first > second) return 1;
		return 0;
	}
};


template<typename _Key, typename _Value, bool IsMulti = false, class _cmp = MyComparator<_Key>>
class  RBTree
{
protected:
	enum _Color { BLACK, RED, None };
	struct Node {
		Node* left;
		Node* right;
		Node* parent;
		std::pair<const _Key, LinkedList<_Value>> data;//���� ���� ������ ��������
		_Color color;

		Node() {
			right = nullptr;
			left = nullptr;
			parent = nullptr;
		}

		Node(const Node& other) : data(other.data) {//��������� - � �������� ����� � ���� ���� � �������
			this->color = other.color;
			left = nullptr;
			right = nullptr;
			parent = nullptr;
		}
		Node(const _Key& key,const  _Value& value, const _Color& color, Node* p, Node* l, Node* r) : data(key, LinkedList<_Value>()) {
			this->color = color;
			left = p;
			right = l;
			parent = r;
			data.second.Append(value);
		}
		Node(const Node* other) : data(other->data) {}


	};

	//_cmp comparator; // comparator(cur->data.first, key)>0 ��� ������������ ��� ���������, ���� ����� ������ ����� ���������� ����� ����� ����� �������� 

	Node* root;//���� ���������� ����� ������ �������
	size_t _size;

protected://������� �������� �� �����, ��������������� ��� ������������
	void leftRotate(Node*& root, Node* x) {// ����� �������
		Node* y = x->right;
		x->right = y->left;
		if (x->right != NULL)
			x->right->parent = x;

		y->parent = x->parent;
		if (y->parent == NULL)
			root = y;
		else {
			if (x == x->parent->left)
				x->parent->left = y;//����� ��������� � �������� ���� 'y' ��� � ��� ��� ���
			else
				x->parent->right = y;
		}
		y->left = x;
		x->parent = y;
	};
	void rightRotate(Node*& root, Node* y) {// ������ �������
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

	void insert(Node*& root, Node* node) {	// ��������� ����, ���������� ���������
		//��� ����, ������ ������� �� ������� �������� � ���������, �������� � �������
		Node* x = root;
		Node* y = NULL;
		while (x != NULL){
			y = x;
			if (node->data.first > x->data.first)
				x = x->right;
			else if (node->data.first < x->data.first)
				x = x->left;
			else if (node->data.first == x->data.first) {
				if (IsMulti == true) {
					x->data.second.Append(node->data.second[0]);
				}
				else
				{
					x->data.second[0] = node->data.second[0];
					_size--;
				}
				return;
			}
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
		//�������� ������������, �� ����� ��, �� � ������ ���� �������...
		//https://www.youtube.com/watch?v=RnQYXltlkrI
		Node* parent = node->parent;
		while (node != RBTree::root && parent->color == RED)//(���������� ������� �� ����� ��� �������� ����� �������) �������, ��� ���� ������� 
		{
			Node* gparent = parent->parent;
			if (gparent->left == parent)
			{
				Node* uncle = gparent->right;
				if (uncle != NULL && uncle->color == RED)//������� ��� ������� ���� (� ����� ��� ������� 3�)
				{
					parent->color = BLACK;
					uncle->color = BLACK;
					gparent->color = RED;
					node = gparent;//���� ���������� � ����, ��� � ����� ����
					parent = node->parent;//��� ��� ����� ����� ��������� �����������
				}
				else//���� ������, ���� �������
				{
					if (parent->right == node)//� ����� ������� ������
					{
						leftRotate(root, parent);//����� ��), � ����� ������� �����
						swap(node, parent);
					}
					rightRotate(root, gparent);//� ����� ������� �����
					gparent->color = RED;
					parent->color = BLACK;
					break;//� ��� ��� �� ����� ����� ����������, ��� ��� ������ ����, � ������ ���� ������� ������)
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
					gparent->color = RED;//��� ��� ������� �������(+swap), �� ����� ������� ������ ���
					break;//� ��� ��� �� ����� ����� ����������, ��� ��� ������ ����, � ������ ���� ������� ������)
				}
			}
		}
		root->color = BLACK;
	}

	void destroy(Node*& node) {		//������� ��� ��������� � ����� ����
		if (node == nullptr)
			return;
		destroy(node->left);
		destroy(node->right);
		delete node;
		node = nullptr;
	}
	void remove(Node*& root, Node* node){

		Node* child, * parent;
		_Color color;
		// ���� 2 �������
		if (node->left != NULL && node->right != NULL)
		{
			Node* replace = node;
			// ����� ����� ������ ����� ���� ������� ��������� �������� ����
			replace = node->right;
			while (replace->left != NULL)
			{
				replace = replace->left;
			}
			// ������, ����� ��������� ���� �� �������� �������� �����
			if (node->parent != NULL)
			{//���� ������� ���� ��������� ��� ���� ������
				if (node->parent->left == node)
					node->parent->left = replace;
				else
					node->parent->right = replace;
			}
			// �������� � �������� �����
			else
				root = replace;
			// child - ��� ���������� ����, ������� �������� ��� � �������� �����, ������� ������� ����������� �������������
			//��� ��� �� �������� �� ����� ����� ������ NODE, �� � ���� (REPLACE) ����� ���� ������ ������ �����
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

			// ��������� ���� �������� ������������ ����� ����������� ���� (replace)
			if (parent == node)
				parent = replace;
			else
			{	//� ���� �� ��������� ���� ����� � ����� ��� ����� �������, ����� ��������))))
				// ������������� ��������� ����
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
			if (color == BLACK)//�� ��������, ��� �� �������� ���� ���, ������� � �*� � ���)))
				removeFixUp(root, child, parent);//�� �������� ������� � ����

			delete node;
			return;
		}
		// ����� 1 �������, ������� �������� ���� ���������� ����
		if (node->left != NULL)
			child = node->left;
		else
			child = node->right;

		//� ����� ���� ����� �� �����, �� ���� ����
		parent = node->parent;
		color = node->color;

		if (child) child->parent = parent;

		// ��������� ���� �� �������� �������� �����
		if (parent)
		{
			if (node == parent->left)
				parent->left = child;
			else
				parent->right = child;
		}
		// ��������� ���� �������� �������� �����
		else
			RBTree::root = child;// � ��� ���� ��������� �������� ��������� ���������� ��� �� ��� ���������� ����� � ���� ������)))
		if (root == nullptr){
			delete node;
			return;
		}

		if (color == BLACK)
		{
			removeFixUp(root, child, parent);//�� �������� ������� � ���� � ���������� �������� ����� ��������
		}
		delete node;

	}
	void removeFixUp(Node*& root, Node* node, Node* parent){
		Node* othernode;//���� ������
		while ((!node) || node->color == BLACK && node != RBTree::root)
		{
			if (parent->left == node)
			{
				othernode = parent->right;
				if (othernode->color == RED)//������ �) � �����
				{
					othernode->color = BLACK;
					parent->color = RED;
					leftRotate(root, parent);
					othernode = parent->right;//����� ������� � ������ �) � �����
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
			return node;//�� ���� ������ ����-������, ���� ���������
		else
			if (key > node->data.first)
				return search(node->right, key);
			else
				return search(node->left, key);
	}

	void print_values(Node* node) const {
		cout << "His keys: ";
		typename LinkedList<_Value>::iterator it = node->data.second.begin();
		while ( it != node->data.second.end()) {
			cout << *it << "\t";
			it++;
		}
		cout << endl << endl;
	
	}

	void print(Node* node)const {  //���������� ��� � ������-�� ����
		if (node == NULL)
			return;
		if (node->parent == NULL){
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
		if (cur->data.first != other->data.first) return false;
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


public://������� �������� ��� ����� - ���������
	RBTree() {		//�����������
		root = nullptr;
		_size = 0;
	}
	RBTree(const RBTree<_Key, _Value>& other) {		//����������� ����������� ������ ��� ��������� ������� �������
		root = new Node(*other.root);
		_size = other._size;
		make_tree(root, other.root);
	}
	~RBTree() {		// ����������
		destroy(root);
	}

	//������� � ��������
	void insert(const _Key key, const _Value value) {   // ��������� ����, ���� ��� �������� �����, ������� ���������
		Node* z = new Node(key, value, RED, NULL, NULL, NULL);
		insert(root, z);
		_size++;

	};
	void remove(_Key key){		// ������� �������� ����
		Node* deletenode = search(root, key);
		if (deletenode != NULL) {
			remove(root, deletenode);
			_size--;
		}
	}

	void remove(_Key key, _Value value){
		Node* deletenode = search(root, key);
		if (deletenode != NULL) {
			if (IsMulti && deletenode->data.second.GetLength() > 1) {
				typename LinkedList<_Value>::iterator it = deletenode->data.second.find(deletenode->data.second.begin(), deletenode->data.second.end(), value);//����� �������� �� ������
				if (it == deletenode->data.second.end()) throw SetException(NoSuchElement);//���� ���, �� ��������
				deletenode->data.second.del_item(it);//� � ����� ������
				//������� �������� � ���
				_size--;
				return;
			}
			else if (deletenode->data.second[0] == value) remove(key);//���� ����� ���� ������� � ����� � �� ����� ��������
			else return;//�� ���� ���� ��� �� �� ����� ������ �����-��������
		}
	}

	//�����
	Node* search(_Key key){
		return search(root, key);
	}

	//��� ������(���)
	void print() {		// ���������� ������
		if (root == NULL)
			cout << "empty RBtree";
		else
			print(root);
	}

	//��� ����� ��� ������
	void preOrder(){			// ������-�����-������ �����
		if (root == NULL)
			cout << "empty RBtree";
		else
			preOrder(root);
	};
	void inOrder(){   // �����-������-������ �����
		if (root == NULL)
			cout << "empty RBtree";
		else
			inOrder(root);
	};
	void postOrder(){   // �����-������-������ �����
		if (root == NULL)
			cout << "empty RBtree";
		else
			postOrder(root);
	};

	void print_values(_Key key) {
		Node* node = search(root, key);
		if (node) {
			cout << "Key: " << key << endl;
			print_values(node);
		}
	}

	//��������� ��������:
	bool Equals(const RBTree<_Key, _Value>& other) const {
		return Equals(this->root, other.root);
	}

	bool operator == (const RBTree<_Key, _Value>& other) const {
		return this->Equals(other);
	}
	bool operator!=(const RBTree<_Key, _Value>& other) const {
		return !this->Equals(other);
	}
	
	//����� ���-�� ��������� ����-��������
	size_t amount() {
		return _size;
	}


	LinkedList<_Value>& get(const _Key& key) {
		Node* cur = root;
		while (cur != nullptr) {

			if (cur->data.first < key) {
				cur = cur->right;
			}
			else if (cur->data.first > key) {
				cur = cur->left;
			}
			else {
				return cur->data.second;
			}
		}
		if (cur == nullptr) throw SetException(NoSuchElement);
	}



	void erase(Node* cur ) {
		if (cur != nullptr) {
			erase(cur->left);
			erase(cur->right);
			delete cur;
		}
	}

	void erase() {
		Node* cur = root;
		root = nullptr;
		erase(cur);
		_size = 0;
	}

};


















