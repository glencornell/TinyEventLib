#include <gtest/gtest.h>
#include "method_ref.hpp"

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

// Test fixture
class MethodRefTest : public ::testing::Test {
protected:
    int output = 0;
};

// Test comparison of method_ref objects
TEST_F(MethodRefTest, Comparison) {
    MyClass1 obj1;
    auto ref1 = method_ref<void(const std::string&)>(&obj1, &MyClass1::printMessage);
    auto ref2 = method_ref<void(const std::string&)>(&obj1, &MyClass1::printMessage);
    EXPECT_EQ(ref1, ref2); // Both refer to the same method

    auto ref3 = ref1;
    EXPECT_EQ(ref1, ref3); // Both refer to the same method

    MyClass1 obj2;;
    auto ref4 = method_ref<void(const std::string&)>(&obj2, &MyClass1::printMessage);
    EXPECT_NE(ref1, ref4); // Different callable objects
}

// Test invalid callable handling
TEST_F(MethodRefTest, NullCallable) {
    method_ref<void(int, int&)>* ref = nullptr;
    EXPECT_EQ(ref, nullptr); // Verify the method_ref can handle null properly
}

TEST_F(MethodRefTest, MethodReturnsVoid) {
    MyClass1 obj1;
    auto ref1 = method_ref<void(int)>(&obj1, &MyClass1::update);
    ref1(15);
    EXPECT_EQ(obj1.receivedValue, 15);
}

TEST_F(MethodRefTest, ConstMethodReturnsVoid) {
    MyClass1 obj1;
    int receivedValue = 0;
    auto ref1 = method_ref<void(int,int&)>(&obj1, &MyClass1::calculate2);
    ref1(15, receivedValue);
    EXPECT_EQ(receivedValue, 16);
}

TEST_F(MethodRefTest, MethodReturnsValue) {
    MyClass1 obj1;
    int receivedValue = 0;
    auto ref1 = method_ref<int(int,int)>(&obj1, &MyClass1::calculate);
    receivedValue = ref1(15, 10);
    EXPECT_EQ(receivedValue, 25);
}

TEST_F(MethodRefTest, BaseClassMethodCalled) {
    DerivedClass obj1;
    auto ref1 = method_ref<void(int)>(static_cast<BaseClass*>(&obj1), &BaseClass::update);
    ref1(15);
    EXPECT_EQ(obj1.receivedValue, 16);
}
