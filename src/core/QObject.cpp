#include "QObject.hpp"
#include <algorithm>

// Global container of top-level QObject pointers
std::vector<QObject*> g_topLevelObjects;

QObject::QObject(QObject* parent)
    : m_parent(parent) {
    if (m_parent) {
        m_parent->addChild(this);
    } else {
        g_topLevelObjects.push_back(this);
    }
}

QObject::~QObject() {
    if (m_parent) {
        m_parent->removeChild(this);
    } else {
        g_topLevelObjects.erase(std::remove(g_topLevelObjects.begin(), g_topLevelObjects.end(), this), g_topLevelObjects.end());
    }
    for (auto child : m_children) {
        delete child;
    }
}

QObject* QObject::parent() const {
    return m_parent;
}

void QObject::setParent(QObject* parent) {
    if (m_parent) {
        m_parent->removeChild(this);
    } else {
        g_topLevelObjects.erase(std::remove(g_topLevelObjects.begin(), g_topLevelObjects.end(), this), g_topLevelObjects.end());
    }
    m_parent = parent;
    if (m_parent) {
        m_parent->addChild(this);
    } else {
        g_topLevelObjects.push_back(this);
    }
}

const std::vector<QObject*>& QObject::children() const {
    return m_children;
}

bool QObject::event(QEvent* event) {
    for (auto filter : m_eventFilters) {
        if (filter->eventFilter(this, event)) {
            return true;
        }
    }
    return false;
}

void QObject::connect(QSignal<>& signal, QSignal<>::SlotType slot) {
    signal.connect(std::move(slot));
}

void QObject::addChild(QObject* child) {
    m_children.push_back(child);
}

void QObject::removeChild(QObject* child) {
    m_children.erase(std::remove(m_children.begin(), m_children.end(), child), m_children.end());
}

void QObject::installEventFilter(QObject* filter) {
    m_eventFilters.push_back(filter);
}

void QObject::removeEventFilter(QObject* filter) {
    m_eventFilters.erase(std::remove(m_eventFilters.begin(), m_eventFilters.end(), filter), m_eventFilters.end());
}

bool QObject::eventFilter(QObject *watched, QEvent* event) {
    return false;
}
