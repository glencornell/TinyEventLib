#ifndef QEVENT_H
#define QEVENT_H

class QEvent {
public:
    enum Type {
        Timer
    };

    explicit QEvent(Type type) : m_eventType(type) {}
    virtual ~QEvent() = default;

    Type type() const { return m_eventType; }

private:
    Type m_eventType;
};

#endif // QEVENT_H
