#pragma once
#include "DynamicArray.h"
#include "IDictionary.h"

template<typename T>
class sparse_matrix {
private:
	size_t hor = 0;
	size_t ver = 0;
	IDictionary<size_t, IDictionary<size_t, T>> matrix;
public:
	sparse_matrix() = default;
	sparse_matrix(size_t hor, size_t ver) {
		this->hor = hor;
		this->ver = ver;
	}
	sparse_matrix(const std::initializer_list<std::initializer_list<T>>& list) {//{{},{},{}}
		this->hor = list.size();
		this->ver = (*list.begin()).size();
		for (auto i : list) {
			if (i.size() != ver) throw SetException(IncorrectArraySize);
		}
		size_t i_index = 0;
		for (auto i : list) {//берем список в списках
			size_t j_index = 0;
			for (auto j : i) {//берем эелемент в определенном списке
				if (j != T()) {
					this->matrix[i_index][j_index] = j;
				}
				j_index++;
			}
			i_index++;
		}
	}
	sparse_matrix(DynamicArray<DynamicArray<T>>& matrix) {
		this->hor = matrix.GetSize();
		this->ver = matrix[0].GetSize();
		for (size_t i = 0; i < matrix.GetSize(); i++) {
			if (matrix[i].GetSize() != ver) throw SetException(IncorrectArraySize);
		}
		for (size_t i = 0; i < hor; i++) {
			for (size_t j = 0; j < ver; j++) {
				if (matrix[i][j] != T()) {
					this->matrix[i][j] = matrix[i][j];
				}
			}
		}
	}
	sparse_matrix(const sparse_matrix<T>& other) : sparse_matrix(other.hor, other.ver) {
		matrix = other.matrix;
	}
	sparse_matrix(sparse_matrix<T>&& other) : sparse_matrix(other.hor, other.ver) {
		matrix = other.matrix;
	}
	~sparse_matrix() = default;

	//сет - поставить на данное место данное число
	void set(size_t row, size_t column, const T& value) {
		if (row >= hor || column >= ver) throw SetException(IncorrectRange);
		if (value != T()) {
			matrix[row][column] = value;
		}
	}
	
	//удаление строки/колонки
	void remove_row(size_t number) {//”ƒјЋя≈ћ —“–ќ ”!	
		if (number < 0 || number >= hor) throw SetException(IncorrectRange);
		matrix.remove(number);
		for (size_t i = number+1; i < hor; i++) {
			if (matrix.find(i)) {
				matrix[i - 1] = matrix[i];
				matrix.remove(i);
			}
		}
		hor--;
		if (hor == 0) ver = 0;
	}
	void remove_column(size_t number) {
		if (number < 0 || number >= ver) throw SetException(IncorrectRange);
		for (size_t i = 0; i < hor; i++) {
			matrix[i].remove(number);//если не найдет, тогда не вызовет ошибки, все норм
		}
		//осталось это подправить толькo
		for (size_t i = 0; i < hor; i++) {
			for (size_t j = number; j < ver; j++) {
				if (matrix.find(i) && matrix.get(i).find(j)) {
					matrix[i][j - 1] = matrix[i][j];
					matrix[i].remove(j);
				}
			}
		}
		ver--;
		if (ver == 0) hor = 0;
	}

	//добавление строк\столбцов
	void add_row(const std::initializer_list<T>& list) {
		if (list.size() != ver && ver != 0) throw SetException(IncorrectArraySize);
		hor++;
		if (ver == 0) ver = list.size();
		size_t pos = 0;
		for (auto i : list) {
			if (i != T()) matrix[hor - 1][pos] = i;
			pos++;
		}
	}
	void add_row(DynamicArray<T>& list) {
		if (list.GetSize() != ver && ver != 0) throw SetException(IncorrectArraySize);
		hor++;
		if (ver == 0) ver = list.GetSize();
		size_t pos = 0;
		for (auto i : list) {
			if (i != T()) matrix[hor - 1][pos] = i;
			pos++;
		}
	}
	void add_column(const std::initializer_list<T>& list) {
		if (list.size() != hor && hor != 0) throw SetException(IncorrectArraySize);
		ver++;
		if (hor == 0) hor = list.size();
		size_t pos = 0;
		for (auto i : list) {
			if (i != T()) matrix[pos][ver - 1] = i;
			pos++;
		}
	}
	void add_column(DynamicArray<T>& list) {
		if (list.GetSize() != hor && hor != 0) throw SetException(IncorrectArraySize);
		ver++;
		if (hor == 0) hor = list.GetSize();
		size_t pos = 0;
		for (auto i : list) {
			if (i != T()) matrix[pos][ver - 1] = i;
			pos++;
		}
	}


	//присваивани€ конструкторы
	sparse_matrix<T>& operator=(const sparse_matrix<T>& other) {
		this->matrix = other.matrix;
		this->hor = other.hor;
		this->ver = other.ver;
		return *this;
	}



	//сравнени€
	bool operator==(sparse_matrix<T>& other) {
		if (!(hor == other.hor && ver == other.ver)) {
			return false;
		}
		for (size_t i = 0; i < hor; i++) {
			for (size_t j = 0; j < ver; j++) {
				if (get(i, j) != other.get(i, j))
					return false;
			}
		}
		return true;
	}
	bool operator!=(sparse_matrix<T>& other) {
		if (!(hor == other.hor && ver == other.ver)) {
			return true;
		}
		for (size_t i = 0; i < hor; i++) {
			for (size_t j = 0; j < ver; j++) {
				if (get(i, j) != other.get(i, j))
					return true;
			}
		}
		return false;
	}


	//геттеры значений, кол-во строк, столбцов
	IDictionary<size_t, T>& operator[](size_t a) {
		if (a >= hor) throw SetException(IncorrectRange);
		try {
			return matrix[a];
		}
		catch (SetException e) {
			if (e.id == NoSuchElement) {
				matrix.IDictionary<size_t, IDictionary<size_t,T>>::insert(a, IDictionary<size_t, T>());
				return matrix[a];
			}
			else {
				throw e;
			}
		}

	}
	T get(size_t row, size_t column) {
		if (row >= hor) throw SetException(IncorrectRange);
		if (column >= ver) throw SetException(IncorrectRange);
		if (matrix.find(row) && matrix.get(row).find(column))
			return matrix[row][column];
		return T();
	}
	size_t get_hor() const {
		return hor;
	}
	size_t get_ver() const {
		return ver;
	}
	
	//дл€ вывода(по разному вывод€т принт и <<) 3 типа вообще сделал)))
	size_t count_not_zeros() {
		size_t a = 0;
		for (size_t i = 0; i < hor; i++) {
			for (size_t j = 0; j < ver; j++) {
				if (get(i, j) != T())
					a++;
			}
		}
		return a;
	}
	void print() {
		cout << "\nMatrix:\t";
		if (ver == hor && hor == 0) {
			cout << "Empty_Matrix";
			return;
		}
		for (size_t j = 0; j < ver; j++){
			cout << j << "\t";
		}
		cout << "\n\n";

		for (size_t i = 0; i < hor; i++) {
			cout  << i << "\t";
			for (size_t j = 0; j <ver; j++) {
				if ( matrix.find(i) && matrix.get(i).find(j)) {
					cout << matrix[i][j] << "\t";
				}
				else cout << T() << "\t";
			}
			cout << "\n";
		}
		cout << "\n";
	}
	void print_as_IDictionary() {//везде создаст словари, если их нет
		if (matrix.amount() == 0) {
			cout << "SparseMatrix:{All zeroes!}\n";
			return;
		}
		cout << "\nSparseMatrix:{\n";
		for (size_t i = 0; i < hor; i++) {
			cout << "\t";
			if (matrix.find(i)) this->matrix[i].IDictionary<size_t, T>::print();
		}
		cout << "}\n";
	}
	template<typename U>
	friend std::ostream& operator<<(std::ostream&, sparse_matrix<U>&);
};

template<typename T>
std::ostream& operator<<(std::ostream& os, sparse_matrix<T>& matrix) {
	os << "\n";
	for (size_t i = 0; i < matrix.hor; i++) {
		for (size_t j = 0; j < matrix.ver; j++) {
			if (matrix.matrix.find(i) && matrix.matrix.get(i).find(j)) {
				os << matrix.matrix[i][j] << " ";
			}
			else os << T() << " ";
		}
		os << "\n";
	}
	os << "\n";
	return os;
}






