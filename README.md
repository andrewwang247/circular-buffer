# Circular Buffer

A templated, fixed-size, cyclic FIFO buffer with random access. See <https://en.wikipedia.org/wiki/Circular_buffer> for more details. To use, simply include `circular_buffer.h` in your file. No linking is needed since the buffer is implemented in the header. Use the class `Circular_Buffer` as you would any other standard container.

## Documentation

The template arguments specify a `typename T` for the container value type and a `std::size_t N` for the maximal size of the buffer.

### Construction

The buffer admits 4 constructors for ease of use:

- Default empty buffer.
- Initializer list buffer.
- Fill buffer.
- Range buffer.

If the initializer list or range are greater than the `N`, only the last `N` items are included in the buffer. This is in keeping with the expected behavior if the items were inserted sequentially.

### Capacity

The `empty`, `size`, and `max_size` functions are self-explanatory. Note that `max_size` is fixed at compile time to be `N`.

### Element Access

Entries can be accessed in their insertion order using `operator[]` and `at`. Like the standard library, `at` will throw `std::out_of_range` if the passed index exceeds the number of entries in the buffer.

The first and last entries in the buffer can be queried with `front` and `back`. These require that the buffer is non-empty.

Finally, the buffer can be converted into a `std::vector<T>` using the `range` function. This operation respects the ordering of the entries.

### Searching

The `contains` function returns whether or not the parameter is contained in the buffer.

### Modifiers

The `pop` function requires that the buffer is non-empty. It removes the oldest entry in the buffer. The `push` function has no size requirements. If the number of entries exceeds `N`, the oldest entry is overwritten. Finally, `clear` removes everything from the buffer.

### Defaults

The implementation of `Circular_Buffer` with `std::array` and indices rather than pointers means that copy, move, destruction, assignment, and comparison can all use the compiler default behavior.

## Testing

In `test.cpp`, the `Circular_Buffer` class is unit tested for correctness and performance tested against `std::queue` on large inputs. Tests can be compiled using the provided `Makefile`.
