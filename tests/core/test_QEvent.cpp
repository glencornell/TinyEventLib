#include <gtest/gtest.h>
#include "QCoreApplication.hpp"
#include "QEventLoop.hpp"
#include "QTimer.hpp"

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
    QCoreApplication *app{nullptr};
    void SetUp() override {
        app = new QCoreApplication(0, nullptr);
    }

    void TearDown() override {
        delete app;
    }
};

TEST_F(QEventTest, ProcessEvents) {
    EventReceiver receiver;
    QTimer timer;

    connect(&timer, &QTimer::timeout, &receiver, [&receiver, &app]() {
        app.postEvent(&receiver, new QEvent("QEvent::User"));
        app.quit();
    });
    timer.start(100);

    app.exec();

    EXPECT_TRUE(receiver.eventReceived);
}

TEST_F(QEventTest, CustomEventLoop) {
    EventReceiver receiver;
    QEventLoop eventLoop;
    QTimer timer;

    connect(&timer, &QTimer::timeout, &receiver, [&receiver, &eventLoop]() {
        QCoreApplication::postEvent(&receiver, new QEvent("QEvent::User"));
        eventLoop.quit();
    });
    timer.start(100);

    eventLoop.exec();

    EXPECT_TRUE(receiver.eventReceived);
}
