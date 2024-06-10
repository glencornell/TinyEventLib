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

void QEventDispatcherUNIX::postEvent(QObject *receiver, QEvent* event) {
    m_eventQueue.push(event);
}

void QEventDispatcherUNIX::processEvents() {
    // Process event queue
    while (!m_eventQueue.empty()) {
        QEvent* event = m_eventQueue.front();
        m_eventQueue.pop();

        for (QObject* obj : g_topLevelObjects) {
            obj->event(event);
        }

        delete event;
    }

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
        if (!timer->isSingleShot()) {
            timer->updateNextTrigger();
        } else {
            unregisterTimer(timer);
        }
    }

    // Handle socket notifiers
    std::vector<struct pollfd> pollfds;
    for (const auto& [fd, notifier] : m_socketNotifiers) {
        if (notifier->isEnabled()) {
            struct pollfd pfd = {fd, POLLIN, 0};
            pollfds.push_back(pfd);
        }
    }

    // Now wait for data on the SocketNotifiers, the time of the next
    // expired timer, or 100ms, whichever comes first.
    const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(next - std::chrono::steady_clock::now());
    int timeout = static_cast<int>(duration.count()) < 0 ? 0 : static_cast<int>(duration.count());
    const int ret = poll(pollfds.data(), pollfds.size(), timeout);
    if (ret > 0) {
        for (const auto& pfd : pollfds) {
            if (pfd.revents & POLLIN) {
                auto it = m_socketNotifiers.find(pfd.fd);
                if (it != m_socketNotifiers.end()) {
                    it->second->activated();
                }
            }
        }
    }
}
