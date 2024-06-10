#include "QSocketNotifier.hpp"

QSocketNotifier::QSocketNotifier(int fd, Type type, QObject* parent)
    : QObject(parent), m_fd(fd), m_type(type), m_enabled(true) {
    QAbstractEventDispatcher::instance()->registerSocketNotifier(this);
}

QSocketNotifier::~QSocketNotifier() {
    QAbstractEventDispatcher::instance()->unregisterSocketNotifier(m_fd);
}

int QSocketNotifier::socket() const {
    return m_fd;
}

bool QSocketNotifier::isEnabled() const {
    return m_enabled;
}

void QSocketNotifier::setEnabled(bool enable) {
    m_enabled = enable;
}

QSocketNotifier::Type QSocketNotifier::type() const {
    return m_type;
}
