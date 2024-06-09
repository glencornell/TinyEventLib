#include "QSocketNotifier.hpp"

QSocketNotifier::QSocketNotifier(int fd, Type type, QObject* parent)
    : QObject(parent), m_fd(fd), m_type(type) {
    QAbstractEventDispatcher::instance()->registerSocketNotifier(this);
}

QSocketNotifier::~QSocketNotifier() {
    QAbstractEventDispatcher::instance()->unregisterSocketNotifier(m_fd);
}

int QSocketNotifier::fd() const {
    return m_fd;
}
