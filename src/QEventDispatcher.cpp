#include "QEventDispatcher.hpp"
#include "QTimer.hpp"
#include "QSocketNotifier.hpp"
#include "QObject.hpp"
#include <algorithm>
#include <poll.h>
#include <chrono>

QEventDispatcher* globalEventDispatcher = nullptr;

QEventDispatcher::QEventDispatcher(QObject* parent)
    : QObject(parent) {
    if (globalEventDispatcher == nullptr) {
        globalEventDispatcher = this;
    }
}

void QEventDispatcher::registerTimer(QTimer* timer) {
    m_timers.push_back(timer);
}

void QEventDispatcher::unregisterTimer(QTimer* timer) {
    m_timers.erase(std::remove(m_timers.begin(), m_timers.end(), timer), m_timers.end());
}

void QEventDispatcher::registerSocketNotifier(QSocketNotifier* notifier) {
    m_socketNotifiers[notifier->fd()] = notifier;
}

void QEventDispatcher::unregisterSocketNotifier(int fd) {
    m_socketNotifiers.erase(fd);
}

void QEventDispatcher::postEvent(QEvent* event) {
    m_eventQueue.push(event);
}

void QEventDispatcher::processEvents() {
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
    auto now = std::chrono::steady_clock::now();
    std::vector<QTimer*> expiredTimers;
    for (QTimer* timer : m_timers) {
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
        struct pollfd pfd = {fd, POLLIN, 0};
        pollfds.push_back(pfd);
    }

    int timeout = 100; // 100 ms timeout
    int ret = poll(pollfds.data(), pollfds.size(), timeout);
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
