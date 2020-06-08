/*
Copyright 2020. Siwei Wang.
*/
#include <chrono>
#include <fstream>
#include <functional>
#include <iostream>
#include <queue>
#include <vector>
#include "circular_buffer.h"

using std::cout;
using std::endl;
using std::function;
using std::ifstream;
using std::queue;
using std::string;
using std::vector;
using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;
using time_unit = std::chrono::microseconds;

// Buffer size used in performance testing.
constexpr size_t BUFFER_SIZE = 25;

namespace Unit_Test {
bool Constructor_Test();
bool Size_Test();
bool Access_Test();
bool Find_Test();
bool Modifier_Test();
bool Range_Test();
}  // namespace Unit_Test

/**
 * Checks for equality of underlying elements.
 */
template <typename T>
bool same_items(queue<T>& qu, const Circular_Buffer<T, BUFFER_SIZE>& cb);

/**
 * Read numbers in filename into a vector.
 * Reserve to quantity.
 */
vector<__int16_t> read(string filename, size_t quantity);

/**
 * Prints execution time of running through numbers using a queue.
 * Returns the final queue to ensure correctness.
 */
queue<__int16_t> run_through(const vector<__int16_t>& numbers, size_t buf_size);

/**
 * Prints execution time of running through numbers using circular buffer.
 * Returns the final buffer to ensure correctness.
 */
Circular_Buffer<__int16_t, BUFFER_SIZE> run_through(
    const vector<__int16_t>& numbers);

void print_duration(high_resolution_clock::time_point start,
                    high_resolution_clock::time_point finish);

int main() {
  vector<function<bool()> > test_cases{
      Unit_Test::Constructor_Test, Unit_Test::Size_Test,
      Unit_Test::Access_Test,      Unit_Test::Find_Test,
      Unit_Test::Modifier_Test,    Unit_Test::Range_Test};

  cout << "--- EXECUTING UNIT TESTS ---\n";

  unsigned passed = 0;
  for (auto& test : test_cases) {
    if (test()) {
      ++passed;
      cout << " passed.\n";
    } else {
      cout << " failed.\n";
    }
  }
  cout << "Passed " << passed << " out of " << test_cases.size() << " tests.\n";
  cout << "--- FINISHED UNIT TESTS ---\n" << endl;

  cout << "--- EXECUTING PERFORMANCE TEST ---\n";
  //* Modify unit of measure to match time_unit.
  cout << "All measurements are in microseconds.\n";

  vector<__int16_t> num = read("numbers.txt", 30'000'000);
  auto qu = run_through(num, BUFFER_SIZE);
  auto cb = run_through(num);
  cout << "--- FINISHED PERFORMANCE TEST ---\n" << endl;

  cout << "--- FINAL COMPARSION CHECK ";
  cout << (same_items(qu, cb) ? "PASSED ---\n" : "FAILED ---\n");
}

template <typename T>
bool same_items(queue<T>& qu, const Circular_Buffer<T, BUFFER_SIZE>& cb) {
  const size_t N = cb.size();
  if (qu.size() != N) return false;
  for (size_t i = 0; i < N; ++i) {
    const T& buffer_item = cb[i];
    const T& queue_item = qu.front();
    if (buffer_item != queue_item) return false;
    qu.pop();
  }
  return true;
}

bool Unit_Test::Constructor_Test() {
  cout << "Constructor test";

  Circular_Buffer<int, 5> def_bf;
  if (!def_bf.empty()) return false;
  if (def_bf.size() != 0) return false;
  if (def_bf.max_size() != 5) return false;

  Circular_Buffer<int, 4> init_bf{4, 6, 2, 7, 8, 1, 0, 3};
  if (init_bf.empty()) return false;
  if (init_bf.size() != 4) return false;
  if (init_bf.max_size() != 4) return false;
  if (init_bf.range() != vector<int>{8, 1, 0, 3}) return false;

  init_bf = Circular_Buffer<int, 4>{1, 2, 3};
  if (init_bf.empty()) return false;
  if (init_bf.size() != 3) return false;
  if (init_bf.max_size() != 4) return false;
  if (init_bf.range() != vector<int>{1, 2, 3}) return false;

  Circular_Buffer<int, 3> fill_bf(47);
  if (fill_bf.empty()) return false;
  if (fill_bf.size() != 3) return false;
  if (fill_bf.max_size() != 3) return false;
  if (fill_bf.range() != vector<int>{47, 47, 47}) return false;

  vector<int> v{4, 6, 2, 7, 8, 1, 0, 3};
  Circular_Buffer<int, 6> range_bf(v.begin(), v.begin() + 3);
  if (range_bf.empty()) return false;
  if (range_bf.size() != 3) return false;
  if (range_bf.max_size() != 6) return false;
  if (range_bf.range() != vector<int>{4, 6, 2}) return false;

  return true;
}

bool Unit_Test::Size_Test() {
  cout << "Size test";

  Circular_Buffer<int, 3> bf{4, 2};
  if (bf.size() != 2) return false;
  for (int i = 5; i < 8; ++i) {
    bf.push(i);
    if (bf.size() != 3) return false;
  }
  for (size_t j = 1; j <= bf.max_size(); ++j) {
    bf.pop();
    if (bf.size() != 3 - j) return false;
  }
  if (!bf.empty()) return false;

  return true;
}

bool Unit_Test::Access_Test() {
  cout << "Access test";

  vector<int> example{6, 5, 4, 3, 2, 1};
  Circular_Buffer<int, 6> bf{6, 5, 4, 3, 2, 1};
  if (bf.front() != 6) return false;
  if (bf.back() != 1) return false;

  for (size_t i = 0; i < bf.size(); ++i) {
    if (example[i] != bf[i]) return false;
    if (bf[i] != bf.at(i)) return false;
  }

  bool caught = false;
  try {
    cout << bf.at(6);
  } catch (std::out_of_range& e) {
    caught = true;
  }
  return caught;
}

bool Unit_Test::Find_Test() {
  cout << "Find test";

  Circular_Buffer<string, 5> cb;
  cb.push("hi");
  cb.push("my");
  cb.push("name");
  if (cb.contains("nope")) return false;
  if (!cb.contains("my")) return false;

  cb.push("is");
  cb.push("siwei");
  cb.push("how");
  cb.push("are");
  cb.push("you");
  if (cb.contains("name")) return false;
  if (!cb.contains("is")) return false;
  if (!cb.contains("how")) return false;

  return true;
}

bool Unit_Test::Modifier_Test() {
  cout << "Modifier test";

  Circular_Buffer<int, 5> bf{2, 4, 6, 8, 10};
  for (int i = 12; i <= 16; i += 2) bf.push(i);
  if (bf.front() != 8) return false;
  if (bf.back() != 16) return false;

  bf.pop();
  if (bf.front() != 10) return false;
  if (bf.back() != 16) return false;

  bf.pop();
  if (bf.front() != 12) return false;
  if (bf.back() != 16) return false;

  bf.pop();
  if (bf.front() != 14) return false;
  if (bf.back() != 16) return false;

  bf.pop();
  if (bf.front() != 16) return false;
  if (bf.back() != 16) return false;

  bf.clear();
  if (!bf.empty()) return false;
  if (bf.size() != 0) return false;

  return true;
}

bool Unit_Test::Range_Test() {
  cout << "Range test";

  Circular_Buffer<int, 6> bf{3, 5, 1, 7};
  if (bf.range() != vector<int>{3, 5, 1, 7}) return false;

  bf.push(2);
  bf.push(4);
  bf.push(6);
  bf.push(8);
  if (bf.range() != vector<int>{1, 7, 2, 4, 6, 8}) return false;

  bf.pop();
  bf.pop();
  if (bf.range() != vector<int>{2, 4, 6, 8}) return false;

  return true;
}

vector<__int16_t> read(string filename, size_t quantity) {
  cout << "Reading numbers...\n";
  ifstream fin(filename);
  vector<__int16_t> v;
  v.reserve(quantity);
  for (__int16_t num; fin >> num;) v.push_back(num);
  return v;
}

queue<__int16_t> run_through(const vector<__int16_t>& numbers,
                             size_t buf_size) {
  cout << "Queue run";
  queue<__int16_t> qu;
  auto start = high_resolution_clock::now();
  for (auto num : numbers) {
    // Branch prediction makes this check a non-factor.
    if (qu.size() == buf_size) qu.pop();
    qu.push(num);
  }
  auto finish = high_resolution_clock::now();
  print_duration(start, finish);
  return qu;
}

Circular_Buffer<__int16_t, BUFFER_SIZE> run_through(
    const vector<__int16_t>& numbers) {
  cout << "Buffer run";
  Circular_Buffer<__int16_t, BUFFER_SIZE> cb;
  auto start = high_resolution_clock::now();
  for (auto num : numbers) cb.push(num);
  auto finish = high_resolution_clock::now();
  print_duration(start, finish);
  return cb;
}

void print_duration(high_resolution_clock::time_point start,
                    high_resolution_clock::time_point finish) {
  cout << " duration: " << duration_cast<time_unit>(finish - start).count()
       << endl;
}
