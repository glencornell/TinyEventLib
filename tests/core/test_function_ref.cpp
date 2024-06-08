#include <gtest/gtest.h>
#include "function_ref.hpp"

// Sample functions and functors for testing
int add(int a, int b) {
    return a + b;
}

struct Multiplier {
    int factor;
    Multiplier(int f) : factor(f) {}
    int operator()(int a) const {
        return a * factor;
    }
};

TEST(test_function_ref, FunctionPointer) {
    function_ref<int(int, int)> ref_add(add);
    EXPECT_EQ(ref_add(2, 3), 5);
}

TEST(test_function_ref, Lambda) {
    auto lambda = [](int a, int b) { return a - b; };
    function_ref<int(int, int)> ref_lambda(lambda);
    EXPECT_EQ(ref_lambda(5, 3), 2);
}

TEST(test_function_ref, Functor) {
    Multiplier multiplier(3);
    function_ref<int(int)> ref_multiplier(multiplier);
    EXPECT_EQ(ref_multiplier(4), 12);
}
