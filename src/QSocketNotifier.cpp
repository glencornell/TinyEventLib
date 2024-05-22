#include "QSocketNotifier.hpp"

QSocketNotifier::QSocketNotifier(int fd, Type type, QObject* parent)
    : QObject(parent), m_fd(fd), m_type(type) {
    globalEventDispatcher->registerSocketNotifier(this);
}

QSocketNotifier::~QSocketNotifier() {
    globalEventDispatcher->unregisterSocketNotifier(m_fd);
}

int QSocketNotifier::fd() const {
    return m_fd;
}
