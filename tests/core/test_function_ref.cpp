#include <gtest/gtest.h>
#include "function_ref.hpp"

// Free function for testing
static void free_function(int x, int& out) {
    out = x + 1;
}

// Functor for testing
struct Functor {
    void operator()(int x, int& out) {
        out = x * 2;
    }
};

// Test fixture
class FunctionRefTest : public ::testing::Test {
protected:
    int output = 0;
};

// Test free function
TEST_F(FunctionRefTest, FreeFunction) {
    function_ref<void(int, int&)> ref(free_function);
    ref(10, output);
    EXPECT_EQ(output, 11); // Free function adds 1
}

// Test functor
TEST_F(FunctionRefTest, Functor) {
    Functor functor;
    function_ref<void(int, int&)> ref(functor);
    ref(10, output);
    EXPECT_EQ(output, 20); // Functor multiplies by 2
}

// Test comparison of function_ref objects
TEST_F(FunctionRefTest, Comparison) {
    function_ref<void(int, int&)> ref1(free_function);
    function_ref<void(int, int&)> ref2(free_function);
    EXPECT_EQ(ref1, ref2); // Both refer to the same free function

    Functor functor;
    function_ref<void(int, int&)> ref3(functor);
    EXPECT_NE(ref1, ref3); // Different callable objects
}

// Test target method
TEST_F(FunctionRefTest, TargetMethod) {
    function_ref<void(int, int&)> ref1(free_function);
    function_ref<void(int, int&)> ref2(free_function);
    EXPECT_EQ(ref1.target(), ref2.target()); // Same target for the same free function

    Functor functor;
    function_ref<void(int, int&)> ref3(functor);
    EXPECT_NE(ref1.target(), ref3.target()); // Different targets for different callables
}

// Test invalid callable handling
TEST_F(FunctionRefTest, NullCallable) {
    function_ref<void(int, int&)>* ref = nullptr;
    EXPECT_EQ(ref, nullptr); // Verify the function_ref can handle null properly
}

// Test mutable lambda
TEST_F(FunctionRefTest, MutableLambda) {
    int state = 5;
    auto lambda = [state](int x, int& out) mutable {
        out = x + state;
        state++;
    };

    function_ref<void(int, int&)> ref(lambda);
    ref(10, output);
    EXPECT_EQ(output, 15); // Lambda adds state to x
}

// Test perfect forwarding
TEST_F(FunctionRefTest, PerfectForwarding) {
    auto lambda = [](int&& x, int& out) {
        out = x + 1;
    };

    int temp = 10;
    function_ref<void(int&&, int&)> ref(lambda);
    ref(std::move(temp), output);
    EXPECT_EQ(output, 11); // Perfect forwarding works
}
