#ifndef QEVENTDISPATCHER_H
#define QEVENTDISPATCHER_H

#include "QEvent.hpp"
#include "QObject.hpp"
#include "Signal.hpp"
#include <functional>
#include <queue>
#include <chrono>
#include <vector>
#include <map>

class QTimer;
class QSocketNotifier;
class QEventDispatcher;
extern QEventDispatcher* globalEventDispatcher;

class QEventDispatcher : public QObject {
public:
    QEventDispatcher(QObject* parent = nullptr);

    void registerTimer(QTimer* timer);
    void unregisterTimer(QTimer* timer);

    void registerSocketNotifier(QSocketNotifier* notifier);
    void unregisterSocketNotifier(int fd);

    void postEvent(QEvent* event);

    void processEvents();

private:
    std::vector<QTimer*> m_timers;
    std::map<int, QSocketNotifier*> m_socketNotifiers;
    std::queue<QEvent*> m_eventQueue;
};

#endif // QEVENTDISPATCHER_H
