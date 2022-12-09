#pragma once
#include "RBTree.h"

template<typename _Key, typename _Value, class _cmp = MyComparator<_Key>>
class IDictionary : protected RBTree<_Key, _Value, false, _cmp> {
protected:
	_Value& get(const _Key& key) {
		return RBT::get(key)[0];
	}

public:
	using RBT = RBTree<_Key, _Value, false, _cmp>;

	IDictionary() : RBT() {}
	IDictionary(const Dictionary<_Key, _Value, _cmp>& other) : RBT(other) {
	}

	//гет нужно будет прописать


	_Value& operator[](const _Key& key) {
		try {
			return this->get(key);
		}
		catch (SetException e) {
			if (e.id == NoSuchElement) {
				RBTree<_Key, _Value, true, false, _cmp>::insert(key, _Value());
				return this->get(key);
			}
			else {
				throw e;
			}
		}
	}
	~Dictionary() = default;
};