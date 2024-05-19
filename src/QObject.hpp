#ifndef QOBJECT_H
#define QOBJECT_H

#include <vector>
#include <functional>
#include <unordered_map>
#include <algorithm>
#include "Signal.hpp"
#include "QEvent.hpp"

class QObject {
public:
    QObject(QObject* parent = nullptr);
    virtual ~QObject();

    QObject* parent() const;
    void setParent(QObject* parent);
    const std::vector<QObject*>& children() const;

    virtual bool event(QEvent* event);

    static void connect(Signal<>& signal, Signal<>::SlotType slot);
    static void addObject(QObject* object);
    static void removeObject(QObject* object);

protected:
    void addChild(QObject* child);
    void removeChild(QObject* child);

private:
    QObject* m_parent = nullptr;
    std::vector<QObject*> m_children;
};

extern std::vector<QObject*> g_topLevelObjects;

#endif // QOBJECT_H
