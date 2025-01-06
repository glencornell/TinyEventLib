#include <gtest/gtest.h>
#include "QSignal.hpp"

// Mock classes for testing
class Receiver {
public:
    void slot(int value) {
        receivedValue = value;
    }

    int receivedValue = 0;
};

void freeFunction(int x, int& out) {
    out = x;
}

class QSignalTest : public ::testing::Test {};

TEST_F(QSignalTest, LambdaFunctionSlot) {
    QSignal<int> signal;
    int receivedValue = 0;

    auto lambda = [&receivedValue](int value) {
        receivedValue = value;
    };

    connect(signal, lambda);

    signal(42);
    EXPECT_EQ(receivedValue, 42);
}

TEST_F(QSignalTest, FreeFunctionSlot) {
    QSignal<int, int&> signal;
    int output = 0;

    connect(signal, freeFunction);

    signal(24, output);
    EXPECT_EQ(output, 24);
}

TEST_F(QSignalTest, MemberFunctionSlot) {
    QSignal<int> signal;
    Receiver receiver1;
    Receiver receiver2;

    signal.connect(receiver1, &Receiver::slot);
    connect(signal, receiver2, &Receiver::slot);
    signal(15);
    EXPECT_EQ(receiver1.receivedValue, 15);
    EXPECT_EQ(receiver2.receivedValue, 15);
}

TEST_F(QSignalTest, MultipleSlots) {
    QSignal<int> signal;
    int receivedValue1 = 0;
    int receivedValue2 = 0;

    connect(signal, [&receivedValue1](int value) { receivedValue1 = value; });
    connect(signal, [&receivedValue2](int value) { receivedValue2 = value; });

    signal(55);
    EXPECT_EQ(receivedValue1, 55);
    EXPECT_EQ(receivedValue2, 55);
}

class MyObject {
public:
    QSignal<int> valueChanged;
    int m_value = 0;

    void setValue(int value) {
        m_value = value;
        valueChanged(value);
    }
};

TEST_F(QSignalTest, ChainedSlots) {
    MyObject obj1, obj2, obj3;
    connect(obj1.valueChanged, obj2, &MyObject::setValue);
    connect(obj2.valueChanged, obj3, &MyObject::setValue);

    obj1.setValue(10);
    EXPECT_EQ(obj1.m_value, 10);
    EXPECT_EQ(obj2.m_value, 10);
    EXPECT_EQ(obj3.m_value, 10);
}

TEST_F(QSignalTest, DisconnectSlot) {
    QSignal<int> signal;
    int receivedValue = 0;

    auto lambda = [&receivedValue](int value) {
        receivedValue = value;
    };

    signal.connect(lambda);
    signal.disconnect(lambda);

    signal(99);
    EXPECT_EQ(receivedValue, 0);
}

TEST_F(QSignalTest, DisconnectNonexistentSlot) {
    QSignal<int> signal;
    Receiver receiver;
    int result = 0;
    auto slot1 = [&result](int value) { result = value; };
    auto slot2 = [](int value) { (void)value; };

    signal.connect(slot1);
    signal.connect([&receiver](int value) {
        receiver.slot(value);
    });
    signal.disconnect(slot2); // Disconnect a slot that was not connected
    signal(42);

    EXPECT_EQ(result, 42);
    EXPECT_EQ(receiver.receivedValue, 42);
}
