#include "QAbstractEventDispatcher.hpp"
#include "QTimer.hpp"

uint16_t QTimer::s_nextId = 0;

QTimer::QTimer(QObject* parent) :
    QObject(parent),
    m_timerId(++s_nextId),
    m_interval(0),
    m_singleShot(false) {
}

void QTimer::start(uint32_t msec) {
    m_interval = msec;
    m_singleShot = false;
    auto now = std::chrono::steady_clock::now();
    m_initialTrigger = now;
    m_nextTrigger = now + std::chrono::milliseconds(msec);
    globalEventDispatcher->registerTimer(this);
}

void QTimer::startSingleShot(uint32_t msec) {
    m_interval = msec;
    m_singleShot = true;
    auto now = std::chrono::steady_clock::now();
    m_initialTrigger = now;
    m_nextTrigger = now + std::chrono::milliseconds(msec);
    globalEventDispatcher->registerTimer(this);
}

void QTimer::stop() {
    globalEventDispatcher->unregisterTimer(this);
}

uint16_t QTimer::timerId() const {
    return m_timerId;
}

std::chrono::steady_clock::time_point QTimer::nextTrigger() const {
    return m_nextTrigger;
}

bool QTimer::isSingleShot() const {
    return m_singleShot;
}

void QTimer::updateNextTrigger() {
    auto now = std::chrono::steady_clock::now();
    m_nextTrigger = m_initialTrigger + std::chrono::milliseconds(((now - m_initialTrigger) / std::chrono::milliseconds(m_interval) + 1) * m_interval);
}
