#include "QAbstractEventDispatcher.hpp"

QAbstractEventDispatcher* QAbstractEventDispatcher::m_instance{nullptr};

QAbstractEventDispatcher::QAbstractEventDispatcher(QObject* parent)
    : QObject(parent) {
    if (m_instance == nullptr) {
        m_instance = this;
    }
}

QAbstractEventDispatcher *QAbstractEventDispatcher::instance() {
    return m_instance;
}
