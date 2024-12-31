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

void freeFunctionSlot(int value, int& receivedValue) {
    receivedValue = value;
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
    QSignal<int> signal;
    int receivedValue = 0;

    connect(signal, [&receivedValue](int value) { freeFunctionSlot(value, receivedValue); });

    signal(24);
    EXPECT_EQ(receivedValue, 24);
}

TEST_F(QSignalTest, MemberFunctionSlot) {
    QSignal<int> signal;
    Receiver receiver;

    signal.connect([&receiver](int value) {
        receiver.slot(value);
    });

    signal(15);
    EXPECT_EQ(receiver.receivedValue, 15);
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
    int result = 0;
    auto slot1 = [&result](int value) { result = value; };
    auto slot2 = [](int value) { (void)value; };

    signal.connect(slot1);
    signal.disconnect(slot2); // Disconnect a slot that was not connected
    signal(42);

    EXPECT_EQ(result, 42);
}
