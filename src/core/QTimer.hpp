#pragma once

#include "QObject.hpp"
#include "QSignal.hpp"
#include <cstdint>
#include <chrono>

class QTimer : public QObject {
public:
    explicit QTimer(QObject* parent = nullptr);

    void start(uint32_t msec);
    void startSingleShot(uint32_t msec);
    void stop();

    uint16_t timerId() const;
    std::chrono::steady_clock::time_point nextTrigger() const;
    bool isSingleShot() const;
    void updateNextTrigger();

    QSignal<> timeout;

private:
    uint16_t m_timerId;
    uint32_t m_interval;
    bool m_singleShot;
    std::chrono::steady_clock::time_point m_initialTrigger;
    std::chrono::steady_clock::time_point m_nextTrigger;
    static uint16_t s_nextId;
};
