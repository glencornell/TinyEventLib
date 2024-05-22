#include "QAbstractEventDispatcher.hpp"

QAbstractEventDispatcher* globalEventDispatcher = nullptr;

QAbstractEventDispatcher::QAbstractEventDispatcher(QObject* parent)
    : QObject(parent) {
    if (globalEventDispatcher == nullptr) {
        globalEventDispatcher = this;
    }
}
