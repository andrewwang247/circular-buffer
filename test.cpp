#include "circular_buffer.h"
#include <iostream>
#include <functional>
using namespace std;

namespace Unit_Test {
	bool Constructor_Test();
	bool Size_Test();
	bool Access_Test();
	bool Modifier_Test();
	bool Range_Test();
}

int main() {
	vector< function<bool()> > test_cases {
		Unit_Test::Constructor_Test,
		Unit_Test::Size_Test,
		Unit_Test::Access_Test,
		Unit_Test::Modifier_Test,
		Unit_Test::Range_Test
	};

	cout << "--- UNIT TESTS ---\n";

	unsigned passed = 0;
	for ( auto& test : test_cases ) {
		if ( test() ) {
			++passed;
			cout << " passed.\n";
		} else {
			cout << " failed.\n";
		}
	}
	cout << "Passed " << passed << " out of " << test_cases.size() << " tests." << endl;
	
}

bool Unit_Test::Constructor_Test() {
	cout << "Constructor test";

	Circular_Buffer<int, 5> def_bf;
	if ( !def_bf.empty() ) return false;
	if ( def_bf.size() != 0 ) return false;
	if ( def_bf.max_size() != 5 ) return false;

	Circular_Buffer<int, 4> init_bf { 4, 6, 2, 7, 8, 1, 0, 3 };
	if ( init_bf.empty() ) return false;
	if ( init_bf.size() != 4 ) return false;
	if ( init_bf.max_size() != 4 ) return false;
	if ( init_bf.range() != vector<int>{ 8, 1, 0, 3 } ) return false;

	init_bf = Circular_Buffer<int, 4> { 1, 2, 3 };
	if ( init_bf.empty() ) return false;
	if ( init_bf.size() != 3 ) return false;
	if ( init_bf.max_size() != 4 ) return false;
	if ( init_bf.range() != vector<int>{ 1, 2, 3 } ) return false;

	Circular_Buffer<int, 3> fill_bf (47);
	if ( fill_bf.empty() ) return false;
	if ( fill_bf.size() != 3 ) return false;
	if ( fill_bf.max_size() != 3 ) return false;
	if ( fill_bf.range() != vector<int> { 47, 47, 47 } ) return false;

	vector<int> v { 4, 6, 2, 7, 8, 1, 0, 3 };
	Circular_Buffer<int, 6> range_bf ( v.begin(), v.begin() + 3 );
	if ( range_bf.empty() ) return false;
	if ( range_bf.size() != 3 ) return false;
	if ( range_bf.max_size() != 6 ) return false;
	if ( range_bf.range() != vector<int> { 4, 6, 2 } ) return false;

	return true;
}

bool Unit_Test::Size_Test() {
	cout << "Size test";

	Circular_Buffer<int, 3> bf { 4, 2 };
	if ( bf.size() != 2 ) return false;
	for ( int i = 5; i < 8; ++i ) {
		bf.push(i);
		if ( bf.size() != 3 ) return false;
	}
	for ( size_t j = 1; j <= bf.max_size(); ++j ) {
		bf.pop();
		if ( bf.size() != 3 - j ) return false;
	}
	if ( !bf.empty() ) return false;

	return true;
}

bool Unit_Test::Access_Test() {
	cout << "Access test";

	vector<int> example { 6, 5, 4, 3, 2, 1 };
	Circular_Buffer<int, 6> bf { 6, 5, 4, 3, 2, 1 };
	if ( bf.front() != 6 ) return false;
	if ( bf.back() != 1 ) return false;

	for ( size_t i = 0; i < bf.size(); ++i ) {
		if ( example[i] != bf[i] ) return false;
		if ( bf[i] != bf.at(i) ) return false;
	}

	bool caught = false;
	try {
		cout << bf.at(6);
	} catch ( std::out_of_range& e ) {
		caught = true;
	}
	return caught;
}

bool Unit_Test::Modifier_Test() {
	cout << "Modifier test";

	Circular_Buffer<int, 5> bf { 2, 4, 6, 8, 10 };
	for ( int i = 12; i <= 16; i += 2 ) bf.push(i);
	if ( bf.front() != 8 ) return false;
	if ( bf.back() != 16 ) return false;

	bf.pop();
	if ( bf.front() != 10 ) return false;
	if ( bf.back() != 16 ) return false;

	bf.pop();
	if ( bf.front() != 12 ) return false;
	if ( bf.back() != 16 ) return false;

	bf.pop();
	if ( bf.front() != 14 ) return false;
	if ( bf.back() != 16 ) return false;

	bf.pop();
	if ( bf.front() != 16 ) return false;
	if ( bf.back() != 16 ) return false;

	return true;
}

bool Unit_Test::Range_Test() {
	cout << "Range test";

	Circular_Buffer<int, 6> bf { 3, 5, 1, 7 };
	if ( bf.range() != vector<int> { 3, 5, 1, 7 } ) return false;

	bf.push(2);
	bf.push(4);
	bf.push(6);
	bf.push(8);
	if ( bf.range() != vector<int> { 1, 7, 2, 4, 6, 8 } ) return false;

	bf.pop();
	bf.pop();
	if ( bf.range() != vector<int> { 2, 4, 6, 8 } ) return false;

	return true;
}