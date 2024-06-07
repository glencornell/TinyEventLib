#pragma once

#include "QSignal.hpp"
#include "QEvent.hpp"
#include <vector>

class QAbstractEventDispatcher;

class QObject {
public:
    explicit QObject(QObject* parent = nullptr);
    virtual ~QObject();

    QObject* parent() const;
    void setParent(QObject* parent);
    const std::vector<QObject*>& children() const;

    virtual bool event(QEvent* event);

    static void connect(QSignal<>& signal, QSignal<>::SlotType slot);
    static void addObject(QObject* object);
    static void removeObject(QObject* object);

    void installEventFilter(QObject* filter);
    void removeEventFilter(QObject* filter);
    bool eventFilter(QEvent* event);

protected:
    void addChild(QObject* child);
    void removeChild(QObject* child);

private:
    QObject* m_parent = nullptr;
    std::vector<QObject*> m_children;
    std::vector<QObject*> m_eventFilters;
};

extern std::vector<QObject*> g_topLevelObjects;
