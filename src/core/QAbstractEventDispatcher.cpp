#include "QAbstractEventDispatcher.hpp"

QAbstractEventDispatcher::QAbstractEventDispatcher(QObject* parent)
    : QObject(parent) {
    if (globalEventDispatcher == nullptr) {
        globalEventDispatcher = this;
    }
}
