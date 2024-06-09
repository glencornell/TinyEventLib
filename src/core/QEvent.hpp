#pragma once

class QEvent {
public:
    using Type = const char*;

    explicit QEvent(Type type) : m_eventType(type) {}
    virtual ~QEvent() = default;

    Type type() const { return m_eventType; }

private:
    Type m_eventType;
};
