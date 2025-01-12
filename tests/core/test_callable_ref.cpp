#include <gtest/gtest.h>
#include "callable_ref.hpp"

class MyClass1 {
public:
    int receivedValue = 0;
    void printMessage(const std::string& message) const {
    }
    int calculate(int a, int b) {
        return a + b;
    }
    void calculate2(int a, int& b) const {
        b = a + 1;
    }
    void update(int a) {
        receivedValue = a;
    }
};

class MyClass2 {
public:
    int receivedValue = 0;
    void printMessage(const std::string& message) const {
    }
    int calculate(int a, int b) {
        return a + b;
    }
    void calculate2(int a, int& b) const {
        b = a + 1;
    }
    void update(int a) {
        receivedValue = a;
    }
};

class BaseClass {
public:
    int receivedValue = 0;
    virtual void printMessage(const std::string& message) const {
    }
    virtual int calculate(int a, int b) {
        return a + b;
    }
    virtual void calculate2(int a, int& b) const {
        b = a;
    }
    virtual void update(int a) {
        receivedValue = a;
    }

    virtual ~BaseClass() = default;
};

class DerivedClass : public BaseClass {
public:
    void printMessage(const std::string& message) const override {
    }
    int calculate(int a, int b) override {
        return a + b + 1;
    }
    void calculate2(int a, int& b) const override {
        b = a + 1;
    }
    void update(int a) override {
        receivedValue = a + 1;
    }
};

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
class CallableRefTest : public ::testing::Test {
protected:
    int output = 0;
};

#if 0
// Test comparison of callable_ref objects
TEST_F(CallableRefTest, Comparison) {
    MyClass1 obj1;
    auto ref1 = callable_ref<void(const std::string&)>(&obj1, &MyClass1::printMessage);
    auto ref2 = callable_ref<void(const std::string&)>(&obj1, &MyClass1::printMessage);
    EXPECT_EQ(ref1, ref2); // Both refer to the same method

    auto ref3 = ref1;
    EXPECT_EQ(ref1, ref3); // Both refer to the same method

    MyClass1 obj2;;
    auto ref4 = callable_ref<void(const std::string&)>(&obj2, &MyClass1::printMessage);
    EXPECT_NE(ref1, ref4); // Different callable objects
}
#endif

TEST_F(CallableRefTest, NullCallable) {
    callable_ref<void(int, int&)>* ref = nullptr;
    EXPECT_EQ(ref, nullptr);
}

TEST_F(CallableRefTest, MethodReturnsVoid) {
    MyClass1 obj1;
    auto ref1 = callable_ref<void(int)>(&obj1, &MyClass1::update);
    ref1(15);
    EXPECT_EQ(obj1.receivedValue, 15);
}

TEST_F(CallableRefTest, ConstMethodReturnsVoid) {
    MyClass1 obj1;
    int receivedValue = 0;
    auto ref1 = callable_ref<void(int,int&)>(&obj1, &MyClass1::calculate2);
    ref1(15, receivedValue);
    EXPECT_EQ(receivedValue, 16);
}

TEST_F(CallableRefTest, MethodReturnsValue) {
    MyClass1 obj1;
    int receivedValue = 0;
    auto ref1 = callable_ref<int(int,int)>(&obj1, &MyClass1::calculate);
    receivedValue = ref1(15, 10);
    EXPECT_EQ(receivedValue, 25);
}

TEST_F(CallableRefTest, BaseClassMethodCalled) {
    DerivedClass obj1;
    auto ref1 = callable_ref<void(int)>(static_cast<BaseClass*>(&obj1), &BaseClass::update);
    ref1(15);
    EXPECT_EQ(obj1.receivedValue, 16);
}

TEST_F(CallableRefTest, FreeFunction) {
    callable_ref<void(int, int&)> ref(free_function);
    ref(10, output);
    EXPECT_EQ(output, 11);
}

TEST_F(CallableRefTest, Functor) {
    Functor functor;
    callable_ref<void(int, int&)> ref(functor);
    ref(10, output);
    EXPECT_EQ(output, 20);
}

TEST_F(CallableRefTest, MutableLambda) {
    int state = 5;
    auto lambda = [state](int x, int& out) mutable {
        out = x + state;
        state++;
    };

    callable_ref<void(int, int&)> ref(lambda);
    ref(10, output);
    EXPECT_EQ(output, 15);
}

TEST_F(CallableRefTest, PerfectForwarding) {
    auto lambda = [](int&& x, int& out) {
        out = x + 1;
    };

    int temp = 10;
    callable_ref<void(int&&, int&)> ref(lambda);
    ref(std::move(temp), output);
    EXPECT_EQ(output, 11);
}
