#pragma once

#include "QEvent.hpp"
#include "QObject.hpp"
#include "QSignal.hpp"

class QTimer;
class QSocketNotifier;

class QAbstractEventDispatcher : public QObject {
public:
    explicit QAbstractEventDispatcher(QObject* parent = nullptr);
    virtual ~QAbstractEventDispatcher();

    static QAbstractEventDispatcher* instance();

    virtual void registerTimer(QTimer* timer) = 0;
    virtual void unregisterTimer(QTimer* timer) = 0;

    virtual void registerSocketNotifier(QSocketNotifier* notifier) = 0;
    virtual void unregisterSocketNotifier(int fd) = 0;

    virtual void postEvent(QObject *receiver, QEvent* event) = 0;
    virtual void processEvents() = 0;

    virtual void registerDeferredDelete(QObject* obj) = 0;
    virtual void processDeferredDeletes() = 0;
private:
    static QAbstractEventDispatcher* m_instance;
};
