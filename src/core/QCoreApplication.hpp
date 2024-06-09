#pragma once

#include "QObject.hpp"
#include "QEvent.hpp"
#include "QAbstractEventDispatcher.hpp"
#include "QEventLoop.hpp"

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
    static QCoreApplication* instance();
    static QAbstractEventDispatcher* eventDispatcher();
    static void setEventDispatcher(QAbstractEventDispatcher* eventDispatcher);

protected:
    virtual bool event(QEvent* e) override;

private:
    static QCoreApplication* m_instance;
    static QAbstractEventDispatcher* m_eventDispatcher;
    bool m_running;
    int m_returnCode;
};
