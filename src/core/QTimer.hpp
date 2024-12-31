#pragma once

#include "QObject.hpp"
#include "QSignal.hpp"
#include <cstdint>
#include <chrono>

class QTimer : public QObject {
public:
    using timerid_t = uint16_t;

    explicit QTimer(QObject* parent = nullptr);
    ~QTimer();

    void start(uint32_t msec);
    void startSingleShot(uint32_t msec);
    void stop();

    timerid_t timerId() const;
    std::chrono::steady_clock::time_point nextTrigger() const;
    bool isSingleShot() const;
    void updateNextTrigger();

    static void singleShot(uint32_t msec, const QSignal<>::SlotType& slot) {
        // TODO: this creates a memory leak. Create a deferred deletion object queue in EventDispatcherUNIX
        QTimer *timer = new QTimer;
        connect(timer, &QTimer::timeout, slot);
        timer->startSingleShot(msec);
    }

    template <typename Receiver, typename Slot>
    static void singleShot(uint32_t msec, Receiver* receiver, Slot slot) {
        // TODO: this creates a memory leak. Create a deferred deletion object queue in EventDispatcherUNIX
        QTimer *timer = new QTimer;
        connect(timer, &QTimer::timeout, receiver, slot);
        timer->startSingleShot(msec);
    }

    template <typename Func>
    static void singleShot(uint32_t msec, Func func) {
        // TODO: this creates a memory leak. Create a deferred deletion object queue in EventDispatcherUNIX
        QTimer *timer = new QTimer;
        connect(timer, &QTimer::timeout, func);
        timer->startSingleShot(msec);
    }

    QSignal<> timeout;

private:
    timerid_t m_timerId;
    uint32_t m_interval;
    bool m_singleShot;
    std::chrono::steady_clock::time_point m_initialTrigger;
    std::chrono::steady_clock::time_point m_nextTrigger;
    static timerid_t s_nextId;
};
