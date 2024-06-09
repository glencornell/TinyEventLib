#include <gtest/gtest.h>
#include "QTimer.hpp"
#include "QAbstractEventDispatcher.hpp"
#include "QEventDispatcherUNIX.hpp"
#include <chrono>
#include <thread>

QEventDispatcherUNIX dispatcher{nullptr};

// Fixture class for QTimer tests
class QTimerTest : public ::testing::Test {
protected:
    QTimer* timer{nullptr};

    void SetUp() override {
        timer = new QTimer();
    }

    void TearDown() override {
        delete timer;
    }
};

TEST_F(QTimerTest, ConstructorInitializesValues) {
    EXPECT_FALSE(timer->isSingleShot());
}

TEST_F(QTimerTest, StartSetsCorrectValues) {
    uint32_t interval = 1000;
    timer->start(interval);

    EXPECT_FALSE(timer->isSingleShot());
    EXPECT_NE(timer->nextTrigger(), std::chrono::steady_clock::time_point());
}

TEST_F(QTimerTest, StartSingleShotSetsCorrectValues) {
    uint32_t interval = 1000;
    timer->startSingleShot(interval);

    EXPECT_TRUE(timer->isSingleShot());
    EXPECT_NE(timer->nextTrigger(), std::chrono::steady_clock::time_point());
}

TEST_F(QTimerTest, StopResetsTrigger) {
    auto now = std::chrono::steady_clock::now();
    uint32_t interval = 1000;
    timer->start(interval);
    timer->stop();

    EXPECT_GE(timer->nextTrigger(), now);
}

TEST_F(QTimerTest, TimerIdIncrements) {
    QTimer timer1;
    QTimer timer2;

    EXPECT_EQ(timer1.timerId(), timer->timerId() + 1);
    EXPECT_EQ(timer2.timerId(), timer->timerId() + 2);
}

TEST_F(QTimerTest, UpdateNextTrigger) {
    uint32_t interval = 100;
    timer->start(interval);

    auto initialTrigger = timer->nextTrigger();
    std::this_thread::sleep_for(std::chrono::milliseconds(interval + 50));
    timer->updateNextTrigger();
    auto updatedTrigger = timer->nextTrigger();
    EXPECT_NE(initialTrigger, updatedTrigger);
    EXPECT_GT(updatedTrigger, initialTrigger);
}

TEST_F(QTimerTest, TimeoutSignal) {
    bool timeoutCalled = false;
    timer->timeout.connect([&]() {
        timeoutCalled = true;
    });

    uint32_t interval = 100;
    timer->start(interval);
    std::this_thread::sleep_for(std::chrono::milliseconds(interval + 50));
    QAbstractEventDispatcher::instance()->processEvents();

    EXPECT_TRUE(timeoutCalled);
}
