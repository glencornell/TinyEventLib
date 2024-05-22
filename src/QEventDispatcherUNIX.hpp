#pragma once

#include "QAbstractEventDispatcher.hpp"
#include <vector>
#include <map>
#include <queue>
#include <chrono>

class QEventDispatcherUNIX : public QAbstractEventDispatcher {
public:
    explicit QEventDispatcherUNIX(QObject* parent = nullptr);

    void registerTimer(QTimer* timer) override;
    void unregisterTimer(QTimer* timer) override;

    void registerSocketNotifier(QSocketNotifier* notifier) override;
    void unregisterSocketNotifier(int fd) override;

    void postEvent(QEvent* event) override;

    void processEvents() override;
    void interrupt() override;
    void wakeUp() override;

private:
    std::vector<QTimer*> m_timers;
    std::map<int, QSocketNotifier*> m_socketNotifiers;
    std::queue<QEvent*> m_eventQueue;
    bool m_interrupted;
};
