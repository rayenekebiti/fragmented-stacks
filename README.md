# Fragmented stacks

**Status: work in progress.** The published source matches the local implementation that passed all five existing sanitizer stress tests. Remaining limitations are documented below.

The stack stores integer values in linked blocks of fixed-size arrays. `stack.cpp` and `stack.hpp` contain the implementation; `main.cpp` contains the stress tests.

## Run the stress tests

Build with GCC, AddressSanitizer, and UndefinedBehaviorSanitizer:

```bash
g++ -std=c++17 -Wall -Wextra -Wpedantic -g -O0 \
  -fsanitize=address,undefined -fno-omit-frame-pointer \
  main.cpp stack.cpp -o stack-test

for mode in burst partial alternating destroy empty; do
  ASAN_OPTIONS=detect_leaks=1 UBSAN_OPTIONS=halt_on_error=1 ./stack-test "$mode"
done
```

Each scenario runs in a separate process with block capacity 5. Returned values are checked against a `std::vector` reference. Destruction runs once through `std::unique_ptr::reset()` if execution reaches that point.

## Latest test results — November 7, 2026

Run: `rerun-20261107-215636`.

| Scenario | Operations | Result |
| --- | --- | --- |
| burst | 1,000 pushes, then 1,000 pops | Pass; zero value mismatches |
| partial | 1,003 pushes, then 1,003 pops | Pass; zero value mismatches |
| alternating | 1,000 push/pop pairs | Pass; zero value mismatches |
| destroy | 1,000 pushes, then destruction | Pass |
| empty | Construct, then destroy without pushing | Pass |

All five processes exited with code 0. No AddressSanitizer, UndefinedBehaviorSanitizer, or LeakSanitizer errors were reported. The build still warns about `#endif STACK_HPP`; the intended spelling is `#endif // STACK_HPP`.

These results were obtained locally from the exact source now published in this repository. In `pop()`, `top` is decremented before accessing and clearing `arr[top]`; the destructor walks the block chain until null. All three source files were compared with the passing test snapshot before publication. These are local test results, not a GitHub Actions run.

## Current limitations

Copy construction and copy assignment are now explicitly deleted. The constructor rejects zero and negative capacities with `std::out_of_range`.

- **Allocation exception safety:** a block can leak if its subsequent array allocation throws, both during construction and growth.
- **Limited coverage:** the five tests use only capacity 5. They do not cover capacity 1, invalid capacities, copying, allocation failures, or repeated drain/refill cycles across block boundaries.
- **Empty-pop behavior is untested:** the `empty` scenario checks destruction only; it does not call `pop()` on an empty stack.
- **Limited interface:** the stack stores only `int` and exposes `push()` and `pop()`, without `size()`, `empty()`, or `peek()`.
- **No performance evidence:** these are correctness and memory-safety checks, not benchmarks. Passing them does not prove correctness for all inputs or establish a speed advantage.
