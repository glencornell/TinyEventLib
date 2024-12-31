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
        std::erase(g_topLevelObjects, this);
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
        std::erase(g_topLevelObjects, this);
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

bool QObject::event(QEvent* theEvent) {
    return false;
}

void QObject::addChild(QObject* child) {
    m_children.push_back(child);
}

void QObject::removeChild(QObject* child) {
    std::erase(m_children, child);
}

void QObject::installEventFilter(QObject* filter) {
    m_eventFilters.push_back(filter);
}

void QObject::removeEventFilter(QObject* filter) {
    std::erase(m_eventFilters, filter);
}

bool QObject::eventFilter(QObject *watched, QEvent* event) {
    return false;
}
