#include "QAbstractEventDispatcher.hpp"

QAbstractEventDispatcher* QAbstractEventDispatcher::m_instance{nullptr};

QAbstractEventDispatcher::QAbstractEventDispatcher(QObject* parent)
    : QObject(parent) {
    m_instance = this;
}

QAbstractEventDispatcher::~QAbstractEventDispatcher() {
    m_instance = nullptr;
}

QAbstractEventDispatcher *QAbstractEventDispatcher::instance() {
    return m_instance;
}
