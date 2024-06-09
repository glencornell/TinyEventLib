#include <gtest/gtest.h>
#include "QObject.hpp"

class QObjectTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Code here will be called immediately after the constructor
        // (right before each test).
    }

    void TearDown() override {
        // Code here will be called immediately after each test (right
        // before the destructor).
    }
};

TEST_F(QObjectTest, ParentChildRelationship) {
    QObject parent;
    QObject child;

    child.setParent(&parent);
    EXPECT_EQ(child.parent(), &parent);
    EXPECT_EQ(parent.children().size(), 1);
    EXPECT_EQ(parent.children().at(0), &child);

    child.setParent(nullptr);
    EXPECT_EQ(child.parent(), nullptr);
    EXPECT_EQ(parent.children().size(), 0);
}

TEST_F(QObjectTest, EventHandling) {
    QObject obj;
    QEvent event("QEvent::Timer");

    EXPECT_FALSE(obj.event(&event));
}

class TestEventFilter : public QObject {
public:
    bool filterEventCalled = false;

    bool eventFilter(QObject* watched, QEvent* event) override {
        filterEventCalled = true;
        return true; // Indicate that the event was handled
    }
};

TEST_F(QObjectTest, EventFilterInstallation) {
    QObject obj;
    TestEventFilter filter;
    QEvent event("QEvent::Timer");

    obj.installEventFilter(&filter);
    obj.event(&event);

    EXPECT_TRUE(filter.filterEventCalled);

    filter.filterEventCalled = false;
    obj.removeEventFilter(&filter);
    obj.event(&event);

    EXPECT_FALSE(filter.filterEventCalled);
}

TEST_F(QObjectTest, StaticObjectManagement) {
    const auto count = g_topLevelObjects.size();
    QObject obj1;
    {
        QObject obj2;
        EXPECT_EQ(g_topLevelObjects.size(), count + 2);
    }
    EXPECT_EQ(g_topLevelObjects.size(), count + 1);
    EXPECT_EQ(g_topLevelObjects.at(count), &obj1);
}

TEST_F(QObjectTest, SignalSlotConnection) {
    QObject obj;
    QSignal<> signal;
    bool slotCalled = false;

    auto slot = [&]() { slotCalled = true; };
    QObject::connect(signal, slot);

    signal();
    EXPECT_TRUE(slotCalled);
}
