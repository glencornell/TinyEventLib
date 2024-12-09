#include <gtest/gtest.h>
#include <QCoreApplication.hpp>
#include <QObject.hpp>
#include <QEvent.hpp>
#include <QTimer.hpp>

// Custom QObject for testing
class TestObject : public QObject {
public:
    bool eventReceived = false;

    TestObject(QObject *parent = nullptr)
        : QObject(parent) {}

protected:
    bool event(QEvent *event) override {
        if (event->type() == "QEvent::User") {
            eventReceived = true;
            return true; // Event handled
        }
        return QObject::event(event); // Default processing
    }
};

class EventFilter : public QObject {
public:
    // This will be set to true when the eventFilter has been invoked
    bool eventFilterCalled = false;

    // Set this to true to filter out the event
    bool eventFilterReturnValue = false;

    EventFilter(QObject *parent = nullptr)
        : QObject(parent) {}

protected:
    bool eventFilter(QObject *receiver, QEvent *event) override {
        eventFilterCalled = true;
        return eventFilterReturnValue;
    }
};

// Test fixture for setting up QCoreApplication
class QCoreApplicationTest : public ::testing::Test {
protected:
    int argc = 0;
    char **argv = nullptr;
    QCoreApplication app{argc, argv}; // Initialize QCoreApplication
};

TEST_F(QCoreApplicationTest, SimpleNotifyTest) {
    TestObject obj;

    // Create a custom event
    QEvent event("QEvent::User");

    // Call notify to deliver the event
    bool handled = app.notify(&obj, &event);

    // Check that the event was handled
    EXPECT_TRUE(handled);
    EXPECT_TRUE(obj.eventReceived);
}

TEST_F(QCoreApplicationTest, ParentChildNotifyTest) {
    TestObject parent;
    TestObject child(&parent); // Set parent-child relationship

    // Create a custom event
    QEvent event("QEvent::User");

    // Call notify to deliver the event to the child
    bool handledChild = app.notify(&child, &event);

    // Call notify to deliver the event to the parent
    bool handledParent = app.notify(&parent, &event);

    // Verify that both the child and parent received their events
    EXPECT_TRUE(handledChild);
    EXPECT_TRUE(child.eventReceived);
    EXPECT_TRUE(handledParent);
    EXPECT_TRUE(parent.eventReceived);
}

TEST_F(QCoreApplicationTest, EventFilterAllow) {
    TestObject obj;

    // Create a custom event
    QEvent event("QEvent::User");

    // Create a custom event filter and add it to the object
    EventFilter filter;
    filter.eventFilterCalled = false;
    filter.eventFilterReturnValue = false; // allow the event to propagate to the object
    obj.installEventFilter(&filter);

    // Call notify to deliver the event
    bool handled = app.notify(&obj, &event);

    // Check that the event was handled
    EXPECT_TRUE(handled);
    EXPECT_TRUE(filter.eventFilterCalled);
    EXPECT_TRUE(obj.eventReceived);
}

TEST_F(QCoreApplicationTest, EventFilterDisallow) {
    TestObject obj;

    // Create a custom event
    QEvent event("QEvent::User");

    // Create a custom event filter and add it to the object
    EventFilter filter;
    filter.eventFilterCalled = false;
    filter.eventFilterReturnValue = true; // disallow the event to propagate to the object
    obj.installEventFilter(&filter);

    // Call notify to deliver the event
    bool handled = app.notify(&obj, &event);

    // Check that the event was handled
    EXPECT_TRUE(handled);
    EXPECT_TRUE(filter.eventFilterCalled);
    EXPECT_FALSE(obj.eventReceived);
}

TEST_F(QCoreApplicationTest, InstanceReturnsThis) {
    auto instance = app.instance();

    // Check that the event was handled
    EXPECT_EQ(&app, instance);
}

TEST_F(QCoreApplicationTest, SendEvent) {
    TestObject obj;
    QEvent event("QEvent::User");
    EXPECT_EQ(app.sendEvent(nullptr, nullptr), false);
    EXPECT_EQ(app.sendEvent(&obj, nullptr), false);
    EXPECT_EQ(app.sendEvent(nullptr, &event), false);
}

TEST_F(QCoreApplicationTest, NotifyNullPtr) {
    TestObject obj;
    QEvent event("QEvent::User");
    EXPECT_EQ(app.notify(nullptr, nullptr), false);
    EXPECT_EQ(app.notify(&obj, nullptr), false);
    EXPECT_EQ(app.notify(nullptr, &event), false);
}

TEST_F(QCoreApplicationTest, CallExecWhileRunningReturnsError) {
    int rval = 0;
    QTimer timer;
    timer.timeout.connect([&]() {
        rval = app.exec();
        app.quit();
    });

    timer.startSingleShot(0);
    app.exec();
    EXPECT_EQ(rval, -1);
}

TEST_F(QCoreApplicationTest, EventDispatcher) {
    EXPECT_NE(app.eventDispatcher(), nullptr);
}
