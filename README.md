# Fragmented stacks

**Status: work in progress.** The latest local implementation passes the five existing sanitizer stress tests. The published implementation still contains earlier pop/destructor bugs; the passing results below do not certify the current GitHub source.

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

## Latest local results — November 7, 2026

Run: `rerun-20261107-215136`.

| Scenario | Operations | Result |
| --- | --- | --- |
| burst | 1,000 pushes, then 1,000 pops | Pass; zero value mismatches |
| partial | 1,003 pushes, then 1,003 pops | Pass; zero value mismatches |
| alternating | 1,000 push/pop pairs | Pass; zero value mismatches |
| destroy | 1,000 pushes, then destruction | Pass |
| empty | Construct, then destroy without pushing | Pass |

All five processes exited with code 0. No AddressSanitizer, UndefinedBehaviorSanitizer, or LeakSanitizer errors were reported. The build still warns about `#endif STACK_HPP`; the intended spelling is `#endif // STACK_HPP`.

These results apply to the newer local source, where `pop()` clears `arr[top - 1]` and the destructor walks the block chain until null. The GitHub implementation inspected at commit `0cc3a8eea126715b93f0ead665f280083a06d8ef` still uses `arr[top]` and the older destructor. Running the published source can therefore still reproduce the earlier failures.

Tested source SHA-256 hashes:

```text
stack.cpp  1994054365d1ff896cbc8e0539e1256bf7e29098089cd9ac9a530862a9dd1d00
stack.hpp  d5fc9377fd514710e2a102b7a6f12c5be33cdfb3ba85396f86db80db7151435f
main.cpp   21443d740b44439334280b276e9c7c7471294019fb1cd3bbdea9060700f1a49e
```

## Current limitations

- **Unsafe copying:** the implicit copy constructor copies owning pointers. Two copied stacks can access or destroy the same storage. Copying needs to be disabled or implemented as a deep copy.
- **Invalid capacities:** the constructor does not reject zero or negative capacities. Zero capacity causes repeated block allocation and recursion on push.
- **Allocation exception safety:** a block can leak if its subsequent array allocation throws, both during construction and growth.
- **Limited coverage:** the five tests use only capacity 5. They do not cover capacity 1, invalid capacities, copying, allocation failures, or repeated drain/refill cycles across block boundaries.
- **Empty-pop behavior is untested:** the `empty` scenario checks destruction only; it does not call `pop()` on an empty stack.
- **Limited interface:** the stack stores only `int` and exposes `push()` and `pop()`, without `size()`, `empty()`, or `peek()`.
- **No performance evidence:** these are correctness and memory-safety checks, not benchmarks. Passing them does not prove correctness for all inputs or establish a speed advantage.
