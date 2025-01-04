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
    template <typename SignalType, typename Callable>
    static void singleShot(uint32_t msec, QObject* receiver, Callable&& slot) {
        auto timer = new QTimer;
        QObject::connect(timer, &QTimer::timeout, receiver, std::forward<Callable>(slot));
        //QObject::connect(timer, &QTimer::timeout, timer, &QObject::deleteLater);
        timer->startSingleShot(msec);
    }

    // Overload for free functions
    static void singleShot(uint32_t msec, void (*function)()) {
        auto timer = new QTimer;
        QObject::connect(timer, &QTimer::timeout, function);
        //QObject::connect(timer, &QTimer::timeout, timer, &QObject::deleteLater);
        timer->startSingleShot(msec);
    }

#if 0
    // Overload for non-const member functions
    template <typename C>
    static void singleShot(uint32_t msec, C& instance, void (C::*method)()) {
        auto timer = new QTimer;
        QObject::connect(timer, &QTimer::timeout, instance, method);
        //QObject::connect(timer, &QTimer::timeout, timer, &QObject::deleteLater);
        timer->startSingleShot(msec);
    }

    // Overload for const member functions
    template <typename C>
    static void singleShot(uint32_t msec, const C& instance, void (C::*method)() const) {
        auto timer = new QTimer;
        QObject::connect(timer, &QTimer::timeout, instance, method);
        //QObject::connect(timer, &QTimer::timeout, timer, &QObject::deleteLater);
        timer->startSingleShot(msec);
    }
#endif

    QSignal<> timeout;

private:
    timerid_t m_timerId;
    uint32_t m_interval;
    bool m_singleShot;
    std::chrono::steady_clock::time_point m_initialTrigger;
    std::chrono::steady_clock::time_point m_nextTrigger;
    static timerid_t s_nextId;
};
