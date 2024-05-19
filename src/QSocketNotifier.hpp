#ifndef QSOCKETNOTIFIER_H
#define QSOCKETNOTIFIER_H

#include "QEventDispatcher.hpp"
#include "QObject.hpp"
#include "Signal.hpp"

class QSocketNotifier : public QObject {
public:
    enum Type {
        Read,
        Write,
        Exception
    };

    QSocketNotifier(int fd, Type type, QObject* parent = nullptr)
        : QObject(parent), m_fd(fd), m_type(type) {
        globalEventDispatcher->registerSocketNotifier(this);
    }

    ~QSocketNotifier() {
        globalEventDispatcher->unregisterSocketNotifier(m_fd);
    }

    int fd() const {
        return m_fd;
    }

    Signal<> activated;

private:
    int m_fd;
    Type m_type;
};

#endif // QSOCKETNOTIFIER_H
