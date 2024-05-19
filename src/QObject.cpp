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
    for (auto child : m_children) {
        if (child->event(event)) {
            return true;
        }
    }
    return false;
}

void QObject::connect(Signal<>& signal, Signal<>::SlotType slot) {
    signal.connect(std::move(slot));
}

void QObject::addChild(QObject* child) {
    m_children.push_back(child);
}

void QObject::removeChild(QObject* child) {
    m_children.erase(std::remove(m_children.begin(), m_children.end(), child), m_children.end());
}

void QObject::addObject(QObject* object) {
    g_topLevelObjects.push_back(object);
}

void QObject::removeObject(QObject* object) {
    g_topLevelObjects.erase(std::remove(g_topLevelObjects.begin(), g_topLevelObjects.end(), object), g_topLevelObjects.end());
}
