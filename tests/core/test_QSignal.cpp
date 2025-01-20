#include <gtest/gtest.h>
#include "QSignal.hpp"

// Mock classes for testing
class Receiver {
public:
    void slot(int value) {
        receivedValue = value;
    }

    void constSlot(int value, int &rval) const {
        rval = value;
    }

    virtual void virtualSlot(int value) {
        receivedValue = value;
        baseVirtualSlotCalled = true;
    }

    virtual void virtualConstSlot(int value, int &rval) const {
        rval = value;
    }

    int receivedValue = 0;
    bool baseVirtualSlotCalled = false;
};

class DerivedReceiver : public Receiver {
public:
    void virtualSlot(int value) override {
        receivedValue = value + 1;
        derivedVirtualSlotCalled = true;
    }

    void virtualConstSlot(int value, int &rval) const override {
        rval = value + 1;
    }

    int receivedValue = 0;
    bool derivedVirtualSlotCalled = false;
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

    signal.connect(lambda);

    signal(42);
    EXPECT_EQ(receivedValue, 42);
}

TEST_F(QSignalTest, FreeFunctionSlot) {
    QSignal<int, int&> signal;
    int output = 0;

    signal.connect(freeFunction);

    signal(24, output);
    EXPECT_EQ(output, 24);
}

TEST_F(QSignalTest, MethodSlot) {
    QSignal<int> signal;
    Receiver receiver;

    signal.connect(&receiver, &Receiver::slot);

    signal(24);
    EXPECT_EQ(receiver.receivedValue, 24);
}

TEST_F(QSignalTest, ConstMethodSlot) {
    QSignal<int, int&> signal;
    Receiver receiver;
    int output = 0;

    signal.connect(&receiver, &Receiver::constSlot);

    signal(24, output);
    EXPECT_EQ(output, 24);
}

TEST_F(QSignalTest, DerivedMethodSlot) {
    QSignal<int> signal;
    DerivedReceiver receiver;

    signal.connect(&receiver, &DerivedReceiver::virtualSlot);

    signal(24);
    EXPECT_EQ(receiver.receivedValue, 25);
}

TEST_F(QSignalTest, DerivedConstMethodSlot) {
    QSignal<int, int&> signal;
    DerivedReceiver receiver;
    int output = 0;

    signal.connect(&receiver, &DerivedReceiver::virtualConstSlot);

    signal(24, output);
    EXPECT_EQ(output, 25);
}

TEST_F(QSignalTest, MultipleSlots) {
    QSignal<int> signal;
    int receivedValue1 = 0;
    int receivedValue2 = 0;

    QSignal<int>::connect(signal, [&receivedValue1](int value) { receivedValue1 = value; });
    QSignal<int>::connect(signal, [&receivedValue2](int value) { receivedValue2 = value; });

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
    QSignal<int>::connect(obj1.valueChanged, [&obj2](int value) { obj2.setValue(value); });
    QSignal<int>::connect(obj2.valueChanged, [&obj3](int value) { obj3.setValue(value); });

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

    auto id = signal.connect(lambda);
    signal.disconnect(id);

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
    signal.disconnect(100); // Disconnect a slot that was not connected
    signal(42);

    EXPECT_EQ(result, 42);
    EXPECT_EQ(receiver.receivedValue, 42);
}
