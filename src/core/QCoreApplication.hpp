#pragma once

#include "QObject.hpp"
#include "QEvent.hpp"
#include "QAbstractEventDispatcher.hpp"
#include "QEventLoop.hpp"
#include <queue>

class QCoreApplication : QObject {
public:
    QCoreApplication() = delete;
    QCoreApplication(int argc, char* argv[]);
    virtual ~QCoreApplication();

    void exit(int returnCode = 0);
    void quit();

    int exec();
    void processEvents();
    void postEvent(QObject* receiver, QEvent* event);
    bool sendEvent(QObject* receiver, QEvent* event);
    bool notify(QObject* receiver, QEvent* event);
    static QCoreApplication* instance();
    static QAbstractEventDispatcher* eventDispatcher();
    static void setEventDispatcher(QAbstractEventDispatcher* eventDispatcher);

private:
    static QCoreApplication* m_instance;
    static QAbstractEventDispatcher* m_eventDispatcher;
    bool m_running;
    int m_returnCode;
    std::queue<std::pair<QObject *, QEvent*>> m_eventQueue;
};
