#pragma once
#include "LinkedList.h"
#include <string>
#include "IDictionary.h"
#include "exception.h"

using namespace std;


template<bool IsWords = true>
class AlpabetPointer : protected IDictionary<string,size_t,true>
{
private:
	IDictionary<string,size_t,true> dict;//{слово: числа,где встречается}
	string text;
	const string restricted = "!\"#$%&’()*+,-./:;<=>?@[]^_`{|}~.";
	const size_t page_size;//количество слов/символов на странице
	size_t page_num = 1;
	size_t cur_size = 0;

	LinkedList<string>* split(const string::const_iterator& begin, const string::const_iterator& end) const noexcept {
		LinkedList<string>* list = new LinkedList<string>();
		for (string::const_iterator it = begin; it != end; it++) {
			if (*it == ' ' || *it == '\n' || *it == '\t') {
				while (it != end && (*it == ' ' || *it == '\n' || *it == '\t')) it++;//вай
				if (it == end) return list;
				list->Append(string());//добавляем пустую строчку
			}
			if (list->GetLength() == 0) {
				list->Append(string());
			}
			(*(list->end() - 1)).push_back(*it);//к этой строчке добавляем что нашли
		}
		return list;
	}

	void delete_restricted(string& str) {
		for (string::iterator it = str.begin(); it != str.end();) {
			if (restricted.find(*it) != string::npos) {//npos-no position
				str.erase(it);
			}
			else it++;
		}
	}
	void place_to_dict(LinkedList<string>* w_list) {
		for (LinkedList<string>::iterator it = w_list->begin(); it != w_list->end(); it++) {
			delete_restricted(*it);
			if ((*it).size() == 0) continue;
			if (!IsWords) {
				if ((*it).size() > (page_num == 1 ? page_size / 2 : page_num % 10 == 0 ? page_size * 3 / 4 : page_size) && cur_size == 0) page_num--;
				cur_size += (*it).size();
				if (cur_size > (page_num == 1 ? page_size / 2 : page_num % 10 == 0 ? page_size * 3 / 4 : page_size)) {
					page_num++;
					cur_size = (*it).size();
				}
				else cur_size++;
			}
			else {
				cur_size += 1;
				if (cur_size > (page_num == 1 ? page_size / 2 : page_num % 10 == 0 ? page_size * 3 / 4 : page_size)) {//смотри как круто научился от if else избавляться - ЭТУ ХУЙНЮ Я ДЕЛАЛ ЧАС)))
					page_num++;
					cur_size = 1;
				}
			}
			dict.insert(*it,page_num);

		}
	}

public:
	AlpabetPointer() = delete;// а хули хотели, создать от пустоты даже без разбиения сколько слов на странице? ну конечно, выпустите книгу из 0 символов, вот перформанс будет про 
	AlpabetPointer(size_t page_size) : page_size(page_size) {};//ну текст можно и потом вставить, самое главное это все-таки, определиться с размером шрифта -> слов на страницу
	AlpabetPointer(const string& str, size_t page_size) : page_size(page_size) {//во, а это уже по-пацански
		if (str.size() == 0) return;
		text = str;
		if (page_size < 1) throw SetException(IncorrectValue);
		place_to_dict(split(str.cbegin(), str.cend()));//сделал так, чтобы не менялся текст исходный, сначала список, потом в списке удаляем хуйню всякую(reestricted)
	}


	void get_pages(const string& word) noexcept {
		dict.print_values(word);
	}
	void print()  {
		dict.print();
	}

	void add(const string& text) {//а вдруг кто книгу допишет
		if (text.size() == 0) return;
		this->text += " " + text;
		place_to_dict(split(text.cbegin(), text.cend()));
	}
	void remove(const string& text) {//удалить все подстроки такие
		size_t check = this->text.find(text);
		while (check != string::npos) {
			this->text.erase(check, text.size());
			check = this->text.find(text);
		}
		dict.erase();//очищаем все дерево->чистый словарь->можно заполнить заново
		page_num = 1;
		cur_size = 0;
		place_to_dict(split(this->text.cbegin(), this->text.cend()));
	}
};