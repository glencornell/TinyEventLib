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

    // QTimer::singleShot implementation using QObject::connect
    template <typename Functor>
    static void singleShot(uint32_t msec, Functor&& slot) {
        auto timer = new QTimer;
        timer->timeout.connect(std::forward<Functor>(slot));
        timer->timeout.connect([&timer]() { timer->deleteLater(); });
        timer->startSingleShot(msec);
    }

    // Overload for free functions
    static void singleShot(uint32_t msec, void (*function)()) {
        auto timer = new QTimer;
        timer->timeout.connect(function);
        timer->timeout.connect([&timer]() { timer->deleteLater(); });
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
