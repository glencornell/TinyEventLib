#include "QEventDispatcherUNIX.hpp"
#include "QCoreApplication.hpp"
#include "QTimer.hpp"
#include "QSocketNotifier.hpp"
#include "QObject.hpp"
#include <algorithm>
#include <poll.h>
#include <chrono>
#include <unistd.h>
#include <sys/eventfd.h>

QEventDispatcherUNIX globalEventDispatcher(nullptr);

QEventDispatcherUNIX::QEventDispatcherUNIX(QObject* parent)
    : QAbstractEventDispatcher(parent) {
    QCoreApplication::setEventDispatcher(this);
}

QEventDispatcherUNIX::~QEventDispatcherUNIX() {
    QCoreApplication::setEventDispatcher(nullptr);
}

void QEventDispatcherUNIX::registerTimer(QTimer* timer) {
    m_timers[timer->timerId()] = timer;
}

void QEventDispatcherUNIX::unregisterTimer(QTimer* timer) {
    m_timers.erase(timer->timerId());
}

void QEventDispatcherUNIX::registerSocketNotifier(QSocketNotifier* notifier) {
    m_socketNotifiers[notifier->socket()] = notifier;
}

void QEventDispatcherUNIX::unregisterSocketNotifier(int fd) {
    m_socketNotifiers.erase(fd);
}

void QEventDispatcherUNIX::registerDeferredDelete(QObject* obj) {
    m_deferredDeleteObjects.push_back(obj);
}

void QEventDispatcherUNIX::processDeferredDeletes() {
    for (QObject* obj : m_deferredDeleteObjects) {
        delete obj;
    }
    m_deferredDeleteObjects.clear();
}

void QEventDispatcherUNIX::processEvents() {
    // Handle timers
    const auto now = std::chrono::steady_clock::now();
    auto next{now + std::chrono::milliseconds(100)};
    std::vector<QTimer*> expiredTimers;
    for (const auto& [id, timer] : m_timers) {
        next = std::min(next, timer->nextTrigger());
        if (now >= timer->nextTrigger()) {
            expiredTimers.push_back(timer);
        }
    }
    for (QTimer* timer : expiredTimers) {
        timer->timeout();
        timer->updateNextTrigger();
        if (timer->isSingleShot()) {
            unregisterTimer(timer);
        }
    }

    // Create pollfds from enabled notifiers
    m_pollfds.clear();
    for (const auto& [fd, notifier] : m_socketNotifiers) {
        if (notifier->isEnabled()) {
            m_pollfds.push_back({fd, POLLIN, 0});
            m_notifierCache[fd] = notifier;
        }
    }

    // Poll for data on the SocketNotifiers, the next expired timer, or 100ms, whichever comes first.
    const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(next - std::chrono::steady_clock::now());
    int timeout = std::max(0, static_cast<int>(duration.count()));
    if (auto ret = ::poll(m_pollfds.data(), m_pollfds.size(), timeout); ret > 0) {
        for (const auto& pfd : m_pollfds) {
            if (pfd.revents & POLLIN) {
                if (auto it = m_notifierCache.find(pfd.fd); it != m_notifierCache.end()) {
                    it->second->activated();
                }
            }
        }
    }
}
