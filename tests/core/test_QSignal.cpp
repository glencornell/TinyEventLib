#include <gtest/gtest.h>
#include "QSignal.hpp"

class QSignalTest : public ::testing::Test {};

TEST_F(QSignalTest, ConnectAndEmit) {
    QSignal<int> signal;
    int result = 0;

    signal.connect([&result](int value) { result = value; });
    signal(42);

    EXPECT_EQ(result, 42);
}

TEST_F(QSignalTest, MultipleSlots) {
    QSignal<int> signal;
    int result1 = 0;
    int result2 = 0;

    signal.connect([&result1](int value) { result1 = value; });
    signal.connect([&result2](int value) { result2 = value; });
    signal(42);

    EXPECT_EQ(result1, 42);
    EXPECT_EQ(result2, 42);
}

TEST_F(QSignalTest, DisconnectSlot) {
    QSignal<int> signal;
    int result = 0;
    auto slot = [&result](int value) { result = value; };

    signal.connect(slot);
    signal.disconnect(slot);
    signal(42);

    EXPECT_EQ(result, 0); // The slot should not be called
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
