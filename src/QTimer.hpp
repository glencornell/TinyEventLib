#ifndef QTIMER_H
#define QTIMER_H

#include "QEventDispatcher.hpp"
#include "QObject.hpp"
#include "Signal.hpp"
#include <cstdint>
#include <chrono>

class QTimer : public QObject {
public:
    QTimer(QObject* parent = nullptr) : QObject(parent), m_timerId(++s_nextId), m_interval(0), m_singleShot(false) {}

    void start(uint32_t msec) {
        m_interval = msec;
        m_singleShot = false;
        auto now = std::chrono::steady_clock::now();
        m_initialTrigger = now;
        m_nextTrigger = now + std::chrono::milliseconds(msec);
        globalEventDispatcher->registerTimer(this);
    }

    void startSingleShot(uint32_t msec) {
        m_interval = msec;
        m_singleShot = true;
        auto now = std::chrono::steady_clock::now();
        m_initialTrigger = now;
        m_nextTrigger = now + std::chrono::milliseconds(msec);
        globalEventDispatcher->registerTimer(this);
    }

    void stop() {
        globalEventDispatcher->unregisterTimer(this);
    }

    uint16_t timerId() const {
        return m_timerId;
    }

    std::chrono::steady_clock::time_point nextTrigger() const {
        return m_nextTrigger;
    }

    bool isSingleShot() const {
        return m_singleShot;
    }

    void updateNextTrigger() {
        auto now = std::chrono::steady_clock::now();
        m_nextTrigger = m_initialTrigger + std::chrono::milliseconds(((now - m_initialTrigger) / std::chrono::milliseconds(m_interval) + 1) * m_interval);
    }

    Signal<> timeout;

private:
    uint16_t m_timerId;
    uint32_t m_interval;
    bool m_singleShot;
    std::chrono::steady_clock::time_point m_initialTrigger;
    std::chrono::steady_clock::time_point m_nextTrigger;
    static uint16_t s_nextId;
};

#endif // QTIMER_H
