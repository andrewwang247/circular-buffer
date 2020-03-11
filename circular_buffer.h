/*
Copyright 2020. Siwei Wang.

A templated Circular Buffer.
*/
#pragma once
#include <algorithm>
#include <array>
#include <exception>
#include <initializer_list>
#include <iterator>
#include <string>
#include <vector>

template <typename T, size_t N>
class Circular_Buffer {
 private:
  /**
   * Underlying data is stored in an array.
   */
  std::array<T, N> data;

  /**
   * Keep track of the head and tail.
   * Valid range is inclusive [head, tail]
   * INVARIANT: head, tail < N.
   */

  size_t head, tail;

  /**
   * Size of container since head == tail could be empty or full.
   */
  size_t num_entries;

  /**
   * Helper function that throw std::out_of_bounds if buffer is empty.
   */
  void check_empty() const {
    if (empty()) {
      throw std::out_of_range("Cannot access elements of empty buffer.");
    }
  }

  /**
   * Helper function that throws std::out_of_bounds if index
   * is out of the buffer's bounds.
   */
  void check_bounds(size_t index) const {
    if (index >= num_entries) {
      std::string message =
          "Index " + std::to_string(index) + " is out of bounds.";
      throw std::out_of_range(message);
    }
  }

 public:
  /* CONSTRUCTORS */

  /**
   * Default initialized empty buffer.
   */
  Circular_Buffer() noexcept : head(0), tail(0), num_entries(0) {}

  /**
   * Initializer list constructor. If li has more elements than N elements,
   *     takes only the last N elements as expected of a circular buffer.
   * @param li: The list with which to initialize the buffer.
   */
  explicit Circular_Buffer(const std::initializer_list<T>& li) : head(0) {
    if (li.size() <= N) {
      num_entries = li.size();
      tail = (li.size() == N) ? 0 : li.size();
      std::copy(li.begin(), li.end(), data.begin());
    } else {
      num_entries = N;
      tail = 0;
      auto modified_start = li.begin() + (li.size() - N);
      std::copy(modified_start, li.end(), data.begin());
    }
  }

  /**
   * Fill constructor.
   * @param filler: The item with which to fill the buffer.
   */
  explicit Circular_Buffer(const T& filler) : head(0), tail(0), num_entries(N) {
    data.fill(filler);
  }

  /**
   * Range constructor.
   * @param start: Iterator specifying start of range.
   * @param finish: Iterator specifying end of range.
   * REQUIRES: If std::distance(start, finish) has more elements than N
   * elements, takes only the last N elements as expected of a circular buffer.
   */
  template <class InputIterator>
  Circular_Buffer(InputIterator start, InputIterator finish) : head(0) {
    auto dist = std::distance(start, finish);
    if (dist < 0) throw std::runtime_error("Range is invalid.");
    auto u_dist = static_cast<size_t>(dist);
    if (u_dist <= N) {
      num_entries = tail = u_dist;
      std::copy(start, finish, data.begin());
    } else {
      num_entries = N;
      tail = 0;
      auto modified_start = start + (u_dist - N);
      std::copy(modified_start, finish, data.begin());
    }
  }

  // Copy, assignment, move, and destructor are default.

  /* CAPACITY */

  /**
   * Checks whether or not the buffer is empty.
   */
  bool empty() const noexcept { return num_entries == 0; }

  /**
   * Returns the number of items stored in the buffer.
   */
  size_t size() const noexcept { return num_entries; }

  /**
   * Returns the maximal number of items the buffer can hold.
   */
  size_t max_size() const noexcept { return N; }

  /* ELEMENT ACCESS */

  /**
   * Element access without bound checks.
   * @param index: the entry to return.
   */
  T& operator[](size_t index) {
    auto mod_index = (head + index) % N;
    return data[mod_index];
  }

  /**
   * Const element access without bound checks.
   * @param index: the entry to return.
   */
  const T& operator[](size_t index) const {
    auto mod_index = (head + index) % N;
    return data[mod_index];
  }

  /**
   * Element access with bound checks.
   * @param index: the entry to return.
   * THROWS: std::out_of_range if index is invalid.
   */
  T& at(size_t index) {
    check_bounds(index);
    return this->operator[](index);
  }

  /**
   * Const element access with bound checks.
   * @param index: the entry to return.
   * THROWS: std::out_of_range if index is invalid.
   */
  const T& at(size_t index) const {
    check_bounds(index);
    return this->operator[](index);
  }

  /**
   * Access the first element.
   * REQUIRES: Buffer is non-empty.
   */
  T& front() {
    check_empty();
    return data[head];
  }

  /**
   * Const access the first element.
   * REQUIRES: Buffer is non-empty.
   */
  const T& front() const {
    check_empty();
    return data[head];
  }

  /**
   * Access the last element.
   * REQUIRES: Buffer is non-empty.
   */
  T& back() {
    check_empty();
    auto adj_ind = (tail == 0) ? N - 1 : tail - 1;
    return data[adj_ind];
  }

  /**
   * Const access the last element.
   * REQUIRES: Buffer is non-empty.
   */
  const T& back() const {
    check_empty();
    auto adj_ind = (tail == 0) ? N - 1 : tail - 1;
    return data[adj_ind];
  }

  /* SEARCHING */

  /**
   * RETURNS: whether or not the buffer contains the x.
   * @param x: The element to search for in this buffer.
   */
  bool contains(const T& x) const noexcept {
    // Simple empty check.
    if (empty()) return false;

    // Find in raw array first.
    auto iter = std::find(data.begin(), data.end(), x);
    if (iter == data.end()) return false;

    // Get the index of iter.
    auto index = static_cast<size_t>(iter - data.begin());

    // Check that index is in the buffer.
    if (index == head) return true;
    if (index > head) return index < head + num_entries;
    // So it must be the case that index < head.
    return index < tail;
  }

  /**
   * Returns a vector with the entries of the buffer in order.
   */
  std::vector<T> range() const {
    if (empty()) return std::vector<T>();

    std::vector<T> entries;
    entries.reserve(num_entries);

    // Split range into two parts.
    const auto ptr = data.data();
    std::copy(ptr + head, ptr + std::min(head + num_entries, N),
              std::back_inserter(entries));
    if (head + num_entries > N)
      std::copy(ptr, ptr + (head + num_entries - N),
                std::back_inserter(entries));

    return entries;
  }

  /* MODIFIERS */

  /**
   * Removes the item at the front of the buffer.
   * REQUIRES: Buffer is non-empty.
   */
  void pop() {
    check_empty();
    --num_entries;
    head = (head + 1 == N) ? 0 : head + 1;
  }

  /**
   * Adds the item to the back of the buffer.
   * @param item: the item to add to the buffer.
   */
  void push(const T& item) noexcept {
    data[tail] = item;

    tail = (tail + 1 == N) ? 0 : tail + 1;
    if (num_entries == N) {
      head = (head + 1 == N) ? 0 : head + 1;
    } else {
      ++num_entries;
    }
  }

  /**
   * Resets the buffer to empty.
   */
  void clear() noexcept { head = tail = num_entries = 0; }
};
