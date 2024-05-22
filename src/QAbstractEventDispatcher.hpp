#pragma once

#include "QEvent.hpp"
#include "QObject.hpp"
#include "QSignal.hpp"
#include <functional>
#include <queue>
#include <chrono>
#include <vector>
#include <map>

class QTimer;
class QSocketNotifier;

class QAbstractEventDispatcher : public QObject {
public:
    explicit QAbstractEventDispatcher(QObject* parent = nullptr);

    virtual void registerTimer(QTimer* timer) = 0;
    virtual void unregisterTimer(QTimer* timer) = 0;

    virtual void registerSocketNotifier(QSocketNotifier* notifier) = 0;
    virtual void unregisterSocketNotifier(int fd) = 0;

    virtual void postEvent(QEvent* event) = 0;

    virtual void processEvents() = 0;
    virtual void interrupt() = 0;
    virtual void wakeUp() = 0;
};

extern QAbstractEventDispatcher* globalEventDispatcher;
