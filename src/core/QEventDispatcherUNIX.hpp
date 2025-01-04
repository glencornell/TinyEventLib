#pragma once

#include "QAbstractEventDispatcher.hpp"
#include "QTimer.hpp"
#include <map>
#include <unordered_map>
#include <vector>


class QEventDispatcherUNIX : public QAbstractEventDispatcher {
public:
    explicit QEventDispatcherUNIX(QObject* parent = nullptr);
    ~QEventDispatcherUNIX();

    void registerTimer(QTimer* timer) override;
    void unregisterTimer(QTimer* timer) override;

    void registerSocketNotifier(QSocketNotifier* notifier) override;
    void unregisterSocketNotifier(int fd) override;

    void processEvents() override;

    void registerDeferredDelete(QObject* obj) override;
    void processDeferredDeletes() override;

private:
    void postEvent(QObject *receiver, QEvent* event) override { };

    std::map<QTimer::timerid_t, QTimer*> m_timers;
    std::map<int, QSocketNotifier*> m_socketNotifiers;
    std::vector<struct pollfd> m_pollfds;
    std::unordered_map<int, QSocketNotifier*> m_notifierCache;
    std::vector<QObject *> m_deferredDeleteObjects;
};
