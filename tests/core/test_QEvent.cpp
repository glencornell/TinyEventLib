#include <gtest/gtest.h>
#include "QCoreApplication.hpp"
#include "QTimer.hpp"

QCoreApplication app{0, nullptr};

class EventReceiver : public QObject {
public:
    EventReceiver() = default;

    bool eventReceived{false};

protected:
    bool event(QEvent *e) override {
        if (e->type() == "QEvent::User") {
            eventReceived = true;
            return true;
        }
        return QObject::event(e);
    }
};

class QEventTest : public ::testing::Test {
protected:
    void SetUp() override {
    }

    void TearDown() override {
    }
};

TEST_F(QEventTest, ProcessEvents) {
    EventReceiver receiver;
    QTimer timer;

    app.postEvent(&receiver, new QEvent("QEvent::User"));

    connect(timer.timeout, [&receiver, this]() {
        app.quit();
    });
    timer.start(100);

    app.exec();

    EXPECT_TRUE(receiver.eventReceived);
}

TEST_F(QEventTest, SendEvent) {
    EventReceiver receiver;
    QTimer timer;

    connect(timer.timeout, [&receiver, this]() {
        QEvent ev("QEvent::User");
        app.sendEvent(&receiver, &ev);
        app.quit();
    });
    timer.start(100);

    app.exec();

    EXPECT_TRUE(receiver.eventReceived);
}
