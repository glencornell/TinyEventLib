#include "QAbstractEventDispatcher.hpp"
#include "QTimer.hpp"

QTimer::timerid_t QTimer::s_nextId = 0;

QTimer::QTimer(QObject* parent) :
    QObject(parent),
    m_timerId(++s_nextId),
    m_interval(0),
    m_singleShot(false) {
}

QTimer::~QTimer() {
    stop();
}

void QTimer::start(uint32_t msec) {
    m_interval = msec;
    m_singleShot = false;
    m_initialTrigger = std::chrono::steady_clock::now();
    m_nextTrigger = m_initialTrigger + std::chrono::milliseconds(msec);
    QAbstractEventDispatcher::instance()->registerTimer(this);
}

void QTimer::startSingleShot(uint32_t msec) {
    m_interval = msec;
    m_singleShot = true;
    m_initialTrigger = std::chrono::steady_clock::now();
    m_nextTrigger = m_initialTrigger + std::chrono::milliseconds(msec);
    QAbstractEventDispatcher::instance()->registerTimer(this);
}

void QTimer::stop() {
    QAbstractEventDispatcher::instance()->unregisterTimer(this);
}

QTimer::timerid_t QTimer::timerId() const {
    return m_timerId;
}

std::chrono::steady_clock::time_point QTimer::nextTrigger() const {
    return m_nextTrigger;
}

bool QTimer::isSingleShot() const {
    return m_singleShot;
}

void QTimer::updateNextTrigger() {
    if (m_singleShot) {
        m_nextTrigger = std::chrono::steady_clock::time_point(); // If single shot, clear the trigger
    } else {
        auto now = std::chrono::steady_clock::now();
        uint64_t elapsedIntervals = std::chrono::duration_cast<std::chrono::milliseconds>(now - m_initialTrigger).count() / m_interval;
        m_nextTrigger = m_initialTrigger + std::chrono::milliseconds((elapsedIntervals + 1) * m_interval);
    }
}
