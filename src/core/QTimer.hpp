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

    QSignal<> timeout;

private:
    timerid_t m_timerId;
    uint32_t m_interval;
    bool m_singleShot;
    std::chrono::steady_clock::time_point m_initialTrigger;
    std::chrono::steady_clock::time_point m_nextTrigger;
    static timerid_t s_nextId;
};
