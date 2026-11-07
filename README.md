# Fragmented stacks

**Status: incomplete / work in progress.** This C++ stack implementation has known memory-safety bugs and is not ready for production use.

The stack stores values in linked blocks of fixed-size arrays. `stack.cpp` and `stack.hpp` contain the implementation; `main.cpp` contains the current stress test.

## Run the stress tests

Build with GCC and AddressSanitizer/UndefinedBehaviorSanitizer:

```bash
g++ -std=c++17 -g -O0 -fsanitize=address,undefined \
  -fno-omit-frame-pointer main.cpp stack.cpp -o stack-test

for mode in burst partial alternating destroy empty; do
  ASAN_OPTIONS=detect_leaks=1 ./stack-test "$mode"
done
```

Each scenario runs in a separate process and uses block capacity 5. Returned values are checked against a `std::vector` reference. The test calls the destructor once through `std::unique_ptr::reset()` if execution reaches that point.

## Current results

| Scenario | Operations | Result |
| --- | --- | --- |
| burst | 1,000 pushes, then pop all | Out-of-bounds access in `pop()` |
| partial | 1,003 pushes, then pop all | Out-of-bounds access in `pop()` |
| alternating | 1,000 push/pop pairs | All returned values correct; destructor crashes |
| destroy | 1,000 pushes, then destruction | Passes with no sanitizer errors or leaks reported |
| empty | Construct, then destroy without pushing | Destructor crashes |

These tests are not exhaustive. A sanitizer failure can stop a scenario before all planned operations or destruction complete. Passing the destroy scenario does not establish destructor safety for other stack states.
