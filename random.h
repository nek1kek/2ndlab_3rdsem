#pragma once
#include <string>

template<typename T>
struct random_unit {
	static T generate() {
		return T();
	}
};

template<>
struct random_unit<int> {
	static int generate() {
		return rand() % 2000 - 1000;
	}
};

template<>
struct random_unit<long long> {
	static long long generate() {
		return rand() % 200000 - 100000;
	}
};

template<>
struct random_unit<char> {
	static char generate() {
		return char(rand() % 90 + 32);
	}
};

template<>
struct random_unit<double> {
	static double generate() {
		double a = (double)random_unit<int>::generate() / (double)(rand() % 99 + 1);
		return (double)((int)(a * 100))/100;
	}
};

template<>
struct random_unit<bool> {
	static bool generate() {
		return rand() % 2;
	}
};

template<>
struct random_unit<std::string> {
	static std::string generate() {
		std::string res;
		size_t len = rand() % 30;
		for (int i = 0; i < len; i++) {
			res += random_unit<char>::generate();
		}
		return res;
	}
};

