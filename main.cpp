#include "stack.hpp"
#include <iostream>
#include <memory>
#include <string>
#include <vector>

// Each case runs separately so a crash cannot hide the other test results.
// Usage: ./program [burst|partial|alternating|destroy|empty]
int main(int argc, char** argv)
{
    std::cout << std::unitbuf;
    const std::string mode = argc > 1 ? argv[1] : "burst";
    if (mode != "burst" && mode != "partial" && mode != "alternating" &&
        mode != "destroy" && mode != "empty") {
        std::cerr << "Choose burst, partial, alternating, destroy, or empty\n";
        return 2;
    }
    auto stack = std::make_unique<STACK>(5);
    std::vector<int> expected;
    int pushes = 0, pops = 0, value_errors = 0;
    auto push = [&](int value) {
        stack->push(value);
        expected.push_back(value);
        ++pushes;
    };
    auto pop = [&] {
        const int wanted = expected.back();
        const int actual = stack->pop();
        expected.pop_back();
        ++pops;
        if (actual != wanted) {
            if (value_errors < 3)
                std::cout << "VALUE FAIL: expected=" << wanted << " actual=" << actual << '\n';
            ++value_errors;
        }
    };
    std::cout << "CASE " << mode << " capacity=5\n";
    if (mode == "alternating") {
        for (int i = 1; i <= 1000; ++i) {
            push(i);
            pop();
        }
    } else if (mode != "empty") {
        const int count = mode == "partial" ? 1003 : 1000;
        for (int i = 1; i <= count; ++i) push(i);
        std::cout << "Completed " << pushes << " pushes; starting "
                  << (mode == "destroy" ? "destruction" : "pops") << '\n';
        if (mode != "destroy")
            while (!expected.empty()) pop();
    }
    std::cout << "Before destructor: pushes=" << pushes << " pops=" << pops
              << " value_errors=" << value_errors << '\n';
    stack.reset(); // Calls ~STACK() exactly once, then releases object storage.
    std::cout << "Destructor returned\n";
    return value_errors ? 1 : 0;
}
